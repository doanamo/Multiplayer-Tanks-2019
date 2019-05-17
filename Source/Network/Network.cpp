#include "Precompiled.hpp"
#include "Network/Network.hpp"
#include "Network/Packets/PacketHeader.hpp"
#include "Game/GameInstance.hpp"

ConsoleVariable<bool> cv_showNetwork("showNetwork", false);

Network::Network() :
    m_gameInstance(nullptr),
    m_udpSocket()
{
}

Network::~Network()
{
}

bool Network::initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short listenPort)
{
    // Save game instance reference.
    if(gameInstance == nullptr)
    {
        LOG_ERROR("Game instance is needed to initialize network interface!");
        return false;
    }

    m_gameInstance = gameInstance;

    // Bind socket to port.
    if(m_udpSocket.bind(listenPort) != sf::Socket::Done)
    {
        LOG_ERROR("Could not bind socket at %hu port!", listenPort);
        return false;
    }

    // Disable blocking mode.
    m_udpSocket.setBlocking(false);

    return true;
}

void Network::update(float timeDelta)
{
}

void Network::tick(float timeDelta)
{
}

void Network::draw()
{
}

bool Network::sendTcpPacket(const PacketBase& packet, sf::TcpSocket& socket)
{
    // Prepare serialized network data.
    MemoryStream networkData;
    if(!serializePacket(networkData, packet))
        return false;

    // Send data over network.
    return sendTcpData(networkData, socket);
}

bool Network::receiveTcpPacket(std::unique_ptr<PacketBase>& packet, sf::TcpSocket& socket)
{
    // Receive data buffer from over network.
    MemoryStream networkData;
    if(!receiveTcpData(networkData, socket))
        return false;

    // Deserialize packet from network data.
    return deserializePacket(networkData, packet);
}

bool Network::sendUdpPacket(const PacketBase& packet, const sf::IpAddress& address, unsigned short port)
{
    // Prepare serialized network data.
    MemoryStream networkData;
    if(!serializePacket(networkData, packet))
        return false;

    // Send data over network.
    return sendUdpData(networkData, address, port);
}

bool Network::receiveUdpPacket(std::unique_ptr<PacketBase>& packet, sf::IpAddress& address, unsigned short& port)
{
    // Receive data buffer from over network.
    MemoryStream networkData;
    if(!receiveUdpData(networkData, address, port))
        return false;

    // Deserialize packet from network data.
    return deserializePacket(networkData, packet);
}

bool Network::serializePacket(MemoryStream& stream, const PacketBase& packet)
{
    // Serialize packet data.
    MemoryStream packetData;
    if(!serialize(packetData, packet))
    {
        LOG_ERROR("Failed to serialize packet!");
        return false;
    }

    // Create packet container.
    PacketHeader packetHeader;
    packetHeader.packetSize = packetData.size();
    packetHeader.packetType = getTypeIdentifier(packet);
    packetHeader.packetCRC = packetHeader.calculateCRC(packetData.data(), packetData.size());

    // Serialize packet header and data into one stream.
    if(!serialize(stream, packetHeader))
    {
        LOG_ERROR("Failed to serialize packet header!");
        return false;
    }

    if(!serialize(stream, packetData))
    {
        LOG_ERROR("Failed to serialize packet data!");
        return false;
    }

    return true;
}

bool Network::deserializePacket(MemoryStream& stream, std::unique_ptr<PacketBase>& packet)
{
    // Deserialize packet header.
    PacketHeader packetHeader;
    if(!deserialize(stream, &packetHeader))
    {
        LOG_ERROR("Failed to deserialize packet header!");
        return false;
    }

    // Deserialize packet data.
    MemoryStream packetData;
    if(!deserialize(stream, &packetData))
    {
        LOG_ERROR("Failed to deserialize packet data!");
        return false;
    }

    // Verify packet CRC.
    uint32_t packetCRC = packetHeader.calculateCRC(packetData.data(), packetData.size());

    if(packetCRC != packetHeader.packetCRC)
    {
        LOG_WARNING("Received packet CRC does not match actual data CRC!");
        return false;
    }

    // Allocate and deserialize packet.
    packet = std::unique_ptr<PacketBase>(PacketBase::create(packetHeader.packetType));

    if(!deserialize(packetData, packet.get()))
    {
        LOG_ERROR("Failed to deserialize received packet!");
        return false;
    }

    return true;
}

bool Network::sendTcpData(const MemoryStream& buffer, sf::TcpSocket& socket)
{
    LOG_TRACE("Sending network data to %s:%hu (%u bytes).",
        socket.getRemoteAddress().toString().c_str(),
        socket.getRemotePort(), buffer.size());

    // Sends packet over TCP socket.
    // This should be done asynchronously instead. Here we will get
    // blocked by one connection for prolonged amount of time.
    std::size_t totalBytesSent = 0;

    while(totalBytesSent < buffer.size())
    {
        std::size_t bytesSent = 0;
        auto socketStatus = socket.send(buffer.data() + totalBytesSent, buffer.size() - totalBytesSent, bytesSent);
        totalBytesSent += bytesSent;

        if(socketStatus != sf::Socket::Partial && socketStatus != sf::Socket::Done)
            return false;
    }

    ASSERT(totalBytesSent == buffer.size());

    return true;
}

bool Network::receiveTcpData(MemoryStream& buffer, sf::TcpSocket& socket)
{
    // Create stream and allocate data for packet header.
    MemoryStream networkData;
    networkData.resize(sizeof(PacketHeader));

    // Receive packet header first.
    std::size_t totalBytesReceived = 0;

    while(totalBytesReceived < networkData.size())
    {
        std::size_t bytesReceived = 0;
        auto socketStatus = socket.receive(networkData.data(), networkData.size(), bytesReceived);
        totalBytesReceived += bytesReceived;

        if(socketStatus != sf::Socket::Partial && socketStatus != sf::Socket::Done)
            return false;
    }

    ASSERT(totalBytesReceived == sizeof(PacketHeader));

    // Peek packet header.
    PacketHeader packetHeader;
    if(!deserialize(networkData, &packetHeader))
    {
        LOG_ERROR("Could not deserialize packet header!");
        return false;
    }

    networkData.reset();

    // Receive packet data.
    networkData.resize(sizeof(PacketHeader) + packetHeader.packetSize);

    while(totalBytesReceived < networkData.size())
    {
        std::size_t bytesReceived = 0;
        auto socketStatus = socket.receive(
            networkData.data() + sizeof(PacketHeader),
            packetHeader.packetSize, bytesReceived);

        totalBytesReceived += bytesReceived;

        if(socketStatus != sf::Socket::Partial && socketStatus != sf::Socket::Done)
            return false;
    }

    ASSERT(totalBytesReceived == networkData.size());

    // Copy received network data to buffer.
    buffer.replace(networkData.data(), networkData.size());

    LOG_TRACE("Received network data from %s:%hu (%u bytes).",
        socket.getRemoteAddress().toString().c_str(),
        socket.getRemotePort(), networkData.size());

    return true;
}

bool Network::sendUdpData(const MemoryStream& buffer, const sf::IpAddress& address, unsigned short port)
{
    LOG_TRACE("Sending network data to %s:%hu (%u bytes).",
        address.toString().c_str(), port, buffer.size());

    // Send packet read from memory buffer.
    if(m_udpSocket.send(buffer.data(), buffer.size(), address, port) != sf::Socket::Done)
    {
        LOG_ERROR("Sending network data resulted in an error!");
        return false;
    }

    return true;
}

bool Network::receiveUdpData(MemoryStream& buffer, sf::IpAddress& address, unsigned short& port)
{
    // Receive packet and write it into memory buffer.
    std::vector<char> datagramBuffer;
    datagramBuffer.resize(sf::UdpSocket::MaxDatagramSize);

    std::size_t bytesReceived = 0;
    auto status = m_udpSocket.receive(&datagramBuffer[0], sf::UdpSocket::MaxDatagramSize, bytesReceived, address, port);
    VERIFY(status != sf::Socket::Partial, "Received partial network data while using UDP socket!");

    if(status != sf::Socket::Done)
        return false;

    LOG_TRACE("Received network data from %s:%hu (%u bytes).", address.toString().c_str(), port, bytesReceived);

    // Write datagram buffer into provided memory buffer.
    buffer.replace(&datagramBuffer[0], bytesReceived);

    return true;
}

bool Network::isConnected() const
{
    return false;
}

bool Network::isServer() const
{
    return false;
}

bool Network::isClient() const
{
    return false;
}
