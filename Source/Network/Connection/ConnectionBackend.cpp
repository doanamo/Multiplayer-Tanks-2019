#include "Precompiled.hpp"
#include "ConnectionBackend.hpp"
#include "ConnectionSocket.hpp"
#include "Network/Packets/PacketHeader.hpp"

ConnectionBackend::ConnectionBackend() :
    m_localPort(0),
    m_defaultSocket(nullptr),
    m_initialized(false)
{
}

ConnectionBackend::~ConnectionBackend()
{
    // Mark as uninitialized to allow worker thread to exit.
    m_initialized = false;

    // Join thread before destroying it.
    m_workerThread.join();
}

bool ConnectionBackend::initialize(unsigned short localPort)
{
    // Check if backend is already initialized.
    if(m_initialized)
    {
        if(localPort != sf::UdpSocket::AnyPort && localPort != m_localPort)
        {
            LOG_ERROR("Could not initialize connection backend due to port mismatch!");
            return false;
        }

        return true;
    }

    // Bind socket to port.
    if(m_udpSocket.bind(localPort) != sf::Socket::Done)
    {
        LOG_ERROR("Could not bind socket to %hu port!", localPort);
        return false;
    }

    // Save bound socket port.
    m_localPort = m_udpSocket.getLocalPort();
    ASSERT(localPort == 0 || localPort == m_localPort, "Bound to other port than expected!");

    // Disable blocking mode.
    m_udpSocket.setBlocking(false);

    // Mark as initialized before spawning worker thread.
    m_initialized = true;
    
    // Spawn worker thread.
    m_workerThread = std::thread(workerThreadMain, this);

    // Success!
    return true;
}

bool ConnectionBackend::registerSocket(ConnectionSocket* socket)
{
    ASSERT(m_initialized);

    std::scoped_lock<std::mutex> lock(m_socketMutex);

    // Validate argument.
    if(socket == nullptr)
    {
        LOG_ERROR("Invalid argument - \"socket\" is null!");
        return false;
    }

    // Check if socket should be default.
    if(socket->getRemoteAddress() == sf::IpAddress::Any)
    {
        // Check if default socket already exists.
        if(m_defaultSocket == nullptr)
        {
            m_defaultSocket = socket;
            return true;
        }   
        else
        {
            LOG_ERROR("Cannot register more than one default socket!");
            return false;
        }
    }

    // Register socket to address map.
    auto result = m_socketMap.emplace(std::make_pair(socket->getRemoteAddress(), socket->getRemotePort()), socket);

    if(result.second == false)
    {
        LOG_ERROR("Could not register socket due to existing address connection used by another socket!");
        return false;
    }

    // Success!
    return true;
}

void ConnectionBackend::unregisterSocket(ConnectionSocket* socket)
{
    ASSERT(m_initialized);

    std::scoped_lock<std::mutex> lock(m_socketMutex);

    // Unregister default socket.
    if(m_defaultSocket == socket)
    {
        m_defaultSocket = nullptr;
        return;
    }

    // Unregister socket from address map.
    m_socketMap.erase(std::make_pair(socket->getRemoteAddress(), socket->getRemotePort())); 
}

bool ConnectionBackend::hasSocketRegistered(const sf::IpAddress& address, unsigned short port)
{
    ASSERT(m_initialized);

    std::scoped_lock<std::mutex> lock(m_socketMutex);

    // Check if socket with remote address and port has been registered.
    return m_socketMap.find(std::make_pair(address, port)) != m_socketMap.end();
}

ConnectionSocket* ConnectionBackend::getDefaultListenSocket() const
{
    ASSERT(m_initialized);

    return m_defaultSocket;
}

void ConnectionBackend::workerThreadMain(ConnectionBackend* backend)
{
    ASSERT(backend);

    // Work only while backend is initialized.
    // There is a matter of whether receiving or sending should be done first, here we decide to receive first.
    // #todo: Error handling with flags so errors can be received from worker thread.
    while(backend->m_initialized)
    {
        // Get start time of this iteration.
        auto startTime = std::chrono::high_resolution_clock::now();

        // Max number of received packets to process in single iteration loop.
        // This is done to prevent infinite loops when sender keeps spamming us with packets faster than we can process them.
        // If we ever reach this point, we should raise an error and abort the connection.
        // We can elegantly implement this by checking sizes of outgoing and incoming buffers.
        const uint32_t MaxReceivedPacketCount = 100;

        // Count number of collected packets.
        uint32_t receivedPacketCount = 0;

        // Receive packets from UDP socket and redirect as incoming packets to connection sockets.
        while(true)
        {
            // Create packet entry that will be filled throughout the loop.
            ConnectionContext::PacketEntry packetEntry;

            // Receive datagram from UDP socket.
            // Only worker thread is allowed to use UDP socket.
            MemoryStream datagramBuffer(sf::UdpSocket::MaxDatagramSize);
            std::size_t bytesReceived = 0;

            sf::Socket::Status status = backend->m_udpSocket.receive(datagramBuffer.data(), 
                sf::UdpSocket::MaxDatagramSize, bytesReceived, packetEntry.address, packetEntry.port);
            VERIFY(status != sf::Socket::Partial, "Received partial network data while using UDP socket!");

            // Check socket status.
            // Break out of the loop if there are no new datagrams.
            switch(status)
            {
            case sf::Socket::Partial:
                LOG_ERROR("Received partial packet in UDP socket!");
                break;

            case sf::Socket::Disconnected:
                // This state is normal if target socket does not exist anymore.
                // Reproducible by shutting down one side of a connection (e.g. server).
                // No idea how socket knows that it disconnected.
                LOG_WARNING("Could not receive from UDP socket due to socket disconnection!");
                break;

            case sf::Socket::Error:
                LOG_ERROR("Could not receive from UDP secket due to unknown error!");
                break;
            }

            if(status != sf::Socket::Done)
                break;

            // Shrink datagram buffer to received data size.
            datagramBuffer.resize(bytesReceived);

            // Deserialize packet header.
            if(!deserialize(datagramBuffer, &packetEntry.header))
            {
                LOG_WARNING("Could not deserialize packet header! Possible packet corruption.");
                continue;
            }

            // Deserialize packet data.
            if(!deserialize(datagramBuffer, &packetEntry.packet))
            {
                LOG_WARNING("Could not deserialize packet data! Possible packet corruption.");
                continue;
            }

            // Check CRC checksum.
            if(packetEntry.header.checksum != packetEntry.header.calculateChecksum(packetEntry.packet.data(), packetEntry.packet.size()))
            {
                LOG_WARNING("Invalid packet checksum detected! Dropping corrupted packet.");
                continue;
            }

            // Find socket connected to source and push incoming packet entry.
            {
                // Lock registering and unregistering connection sockets.
                std::scoped_lock<std::mutex> lock(backend->m_socketMutex);

                // Iterate over all registered connection sockets.
                auto it = backend->m_socketMap.find(std::make_pair(packetEntry.address, packetEntry.port));

                if(it != backend->m_socketMap.end())
                {
                    // Push incoming packet entry to connection socket.
                    // Push does not have to succeed if reliable packet was duplicated during transport.
                    it->second->getConnectionContext().pushIncoming(packetEntry);
                }
                else
                {
                    // Push incoming packet to default connection socket.
                    // Only unreliable packets can be redirected to default socket.
                    if(backend->m_defaultSocket)
                    {
                        // Reliable packets can be stored in default socket, but should
                        // be pushed to new connection socket in order for reliability to work.
                        backend->m_defaultSocket->getConnectionContext().pushIncoming(packetEntry);
                    }
                }

                // Increment received packet count.
                receivedPacketCount++;

                // Check if we reached maximum number of packets that we can process in single iteration.
                if(receivedPacketCount >= MaxReceivedPacketCount)
                {
                    LOG_TRACE("Reached maximum received packet count in single iteration. (receivedPacketCount %u)", receivedPacketCount);
                    break;
                }
            }
        }

        if(receivedPacketCount != 0)
        {
            LOG_TRACE("Received incoming packets. (%u packets)", receivedPacketCount);
        }

        // Send outgoing packets from connections sockets via UDP socket.
        {
            // Collect outgoing packets that will be sent.
            // We do not want to lock for extensive amount of time.
            std::queue<ConnectionContext::PacketEntry> outgoingPackets;

            {
                // Lock registering and unregistering connection sockets.
                std::scoped_lock<std::mutex> lock(backend->m_socketMutex);

                // Collect packets from default connection socket.
                if(backend->m_defaultSocket)
                {
                    // Pop outgoing packet and save it.
                    ConnectionSocket* socket = backend->m_defaultSocket;
                    ConnectionContext& context = socket->getConnectionContext();
                    ConnectionContext::PacketEntry packetEntry;

                    while(context.popOutgoing(&packetEntry))
                    {
                        outgoingPackets.push(packetEntry);
                    }
                }

                // Collect packets from registered connection sockets.
                for(const auto& socketEntry : backend->m_socketMap)
                {
                    // Pop outgoing packet and save it.
                    ConnectionSocket* socket = socketEntry.second;
                    ConnectionContext& context = socket->getConnectionContext();
                    ConnectionContext::PacketEntry packetEntry;

                    // Clear reliable queue from acknowledged packets.
                    context.popAcknowledged();

                    // Copy still unacknowledged packets before new unreliable/reliable packets.
                    context.copyUnacknowledged(outgoingPackets);

                    // Collect packets that need to be sent.
                    while(context.popOutgoing(&packetEntry))
                    {
                        // Add packet to be sent out.
                        outgoingPackets.push(packetEntry);
                    }
                }
            }

            // Send collected outgoing packets.
            if(!outgoingPackets.empty())
            {
                LOG_TRACE("Sending outgoing packets. (%u packets)", outgoingPackets.size());
            }
            
            while(!outgoingPackets.empty())
            {
                // Extract packet entry from queue.
                ConnectionContext::PacketEntry packetEntry = outgoingPackets.front();
                outgoingPackets.pop();

                // Calculate packet CRC checksum.
                packetEntry.header.checksum = packetEntry.header.calculateChecksum(packetEntry.packet.data(), packetEntry.packet.size());

                // Serialize packet header and data.
                MemoryStream datagramBuffer;

                if(!serialize(datagramBuffer, packetEntry.header))
                {
                    LOG_ERROR("Could not serialize packet header!");
                    continue;
                }

                if(!serialize(datagramBuffer, packetEntry.packet))
                {
                    LOG_ERROR("Could not serialize packet data!");
                    continue;
                }

                if(datagramBuffer.size() > 1400)
                {
                    LOG_WARNING("Sending packet with size over 1400 bytes! Datagram will be fragmented.");
                }

                // Send packet via UDP socket.
                if(backend->m_udpSocket.send(datagramBuffer.data(), datagramBuffer.size(), packetEntry.address, packetEntry.port) != sf::Socket::Done)
                {
                    LOG_ERROR("Could not send datagram over UDP socket!");
                    continue;
                }
            }
        }

        // Sleep thread for some time.
        // We do not want this running too quick.
        std::this_thread::sleep_until(startTime + std::chrono::duration<int, std::ratio<1, 120>>(1));
        std::this_thread::yield();
    }
}

unsigned short ConnectionBackend::getLocalPort() const
{
    return m_udpSocket.getLocalPort();
}
