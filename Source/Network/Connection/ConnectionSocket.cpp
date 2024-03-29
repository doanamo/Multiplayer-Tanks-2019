#include "Precompiled.hpp"
#include "ConnectionSocket.hpp"
#include "ConnectionBackend.hpp"
#include "ConnectionContext.hpp"

ConnectionSocket::ConnectionSocket(std::shared_ptr<ConnectionBackend> connectionBackend) :
    m_connectionBackend(connectionBackend),
    m_connectionContext(this),
    m_remoteAddress(sf::IpAddress::Any),
    m_remotePort(sf::UdpSocket::AnyPort),
    m_initialized(false)
{
    // Create connection backend if none is provided.
    if(connectionBackend == nullptr)
    {
        m_connectionBackend = std::make_shared<ConnectionBackend>();
    }
}

ConnectionSocket::~ConnectionSocket()
{
    // Unregister socket from connection backend.
    m_connectionBackend->unregisterSocket(this);
}

bool ConnectionSocket::connect(const sf::IpAddress& remoteAddress, unsigned short remotePort)
{
    ASSERT(!m_initialized);

    // Check if remote address is valid.
    if(remoteAddress == sf::IpAddress::Any || remoteAddress == sf::IpAddress::Broadcast)
    {
        LOG_ERROR("Specified remote address is not valid!");
        return false;
    }

    // Initialize connection context.
    if(!m_connectionContext.initialize(true))
    {
        LOG_ERROR("Could not initialize connection context!");
        return false;
    }

    // Initialize connection backend.
    if(!m_connectionBackend->initialize(sf::UdpSocket::AnyPort))
    {
        LOG_ERROR("Could not initialize connection backend!");
        return false;
    }

    // Save remote address and port.
    m_remoteAddress = remoteAddress;
    m_remotePort = remotePort;

    // Mark as initialized before registration to multi threaded backend.
    m_initialized = true;

    // Register socket in connection backend.
    if(!m_connectionBackend->registerSocket(this))
    {
        LOG_ERROR("Could not register socket in connection backend!");
        m_initialized = false;
        return false;
    }

    // Success!
    return true;
}

bool ConnectionSocket::listen(unsigned short localPort)
{
    ASSERT(!m_initialized);

    // Initialize connection context.
    if(!m_connectionContext.initialize(false))
    {
        LOG_ERROR("Could not initialize connection context!");
        return false;
    }

    // Initialize connection backend.
    if(!m_connectionBackend->initialize(localPort))
    {
        LOG_ERROR("Could not initialize connection backend!");
        return false;
    }

    // Mark as initialized before registration to multi threaded backend.
    m_initialized = true;

    // Register socket in connection backend.
    if(!m_connectionBackend->registerSocket(this))
    {
        LOG_ERROR("Could not register socket in connection backend!");
        return false;
    }

    // Success!
    return true;
}

bool ConnectionSocket::send(const MemoryStream& packet, bool reliable, const sf::IpAddress* address, const unsigned short* port)
{
    ASSERT(m_initialized);

    // Prepare packet entry.
    ConnectionContext::PacketEntry packetEntry;
    packetEntry.packet = packet;

    if(reliable)
    {
        packetEntry.header.transportMethod = (uint32_t)ConnectionContext::TransformMethod::Reliable;
    }
    else
    {
        packetEntry.header.transportMethod = (uint32_t)ConnectionContext::TransformMethod::Unreliable;
    }

    if(!reliable && address && port)
    {
        packetEntry.address = *address;
        packetEntry.port = *port;
    }
    else
    {
        packetEntry.address = m_remoteAddress;
        packetEntry.port = m_remotePort;
    }

    // Push packet to outgoing queue.
    return m_connectionContext.pushOutgoing(packetEntry);
}

bool ConnectionSocket::receive(MemoryStream& packet, bool* reliable, sf::IpAddress* address, unsigned short* port)
{
    ASSERT(m_initialized);

    // Receive incoming packet.
    ConnectionContext::PacketEntry packetEntry;
    if(!m_connectionContext.popIncoming(&packetEntry))
        return false;

    // Copy packet data and info.
    packet = packetEntry.packet;

    if(reliable)
    {
        *reliable = (packetEntry.header.transportMethod == (uint32_t)ConnectionContext::TransformMethod::Reliable);
    }

    if(address)
    {
        *address = packetEntry.address;
    }

    if(port)
    {
        *port = packetEntry.port;
    }

    // Retrieved and processed incoming packet.
    return true;
}

const std::shared_ptr<ConnectionBackend>& ConnectionSocket::getConnectionBackend()
{
    ASSERT(m_initialized);
    return m_connectionBackend;
}

ConnectionContext& ConnectionSocket::getConnectionContext()
{
    ASSERT(m_initialized);
    return m_connectionContext;
}

const sf::IpAddress& ConnectionSocket::getRemoteAddress() const
{
    ASSERT(m_initialized);
    return m_remoteAddress;
}

unsigned short ConnectionSocket::getRemotePort() const
{
    ASSERT(m_initialized);
    return m_remotePort;
}

unsigned short ConnectionSocket::getLocalPort() const
{
    ASSERT(m_initialized);
    return m_connectionBackend->getLocalPort();
}

bool ConnectionSocket::supportsReliability() const
{
    ASSERT(m_initialized);
    return m_remoteAddress != sf::IpAddress::Any;
}
