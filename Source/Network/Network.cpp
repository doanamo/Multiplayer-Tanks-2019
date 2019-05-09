#include "Precompiled.hpp"
#include "Network/Network.hpp"
#include "Network/PacketContainer.hpp"

ConsoleVariable<bool> cv_showNetwork("showNetwork", false);

Network::Network() :
    m_udpSocket()
{
}

Network::~Network()
{
}

bool Network::initialize(const sf::IpAddress& address, unsigned short listenPort)
{
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

bool Network::sendPacket(PacketBase& packet, const sf::IpAddress& address, unsigned short port)
{
    // Create packet container.
    PacketContainer packetContainer;
    packetContainer.packetType = getTypeIdentifier(packet);

    if(!serialize(packetContainer.packetBuffer, packet))
    {
        LOG_ERROR("Failed to serialize packet!");
        return false;
    }

    // Serialize packet container.
    MemoryBuffer dataBuffer;
    if(!serialize(dataBuffer, packetContainer))
    {
        LOG_ERROR("Failed to serialize network data!");
        return false;
    }

    // Send data over network.
    return sendData(dataBuffer, address, port);
}

bool Network::receivePacket(std::unique_ptr<PacketBase>& packet, sf::IpAddress& address, unsigned short& port)
{
    // Receive data buffer from over network.
    MemoryBuffer dataBuffer;
    if(!receiveData(dataBuffer, address, port))
        return false;
    
    // Deserialize packet container.
    PacketContainer packetContainer;
    if(!deserialize(dataBuffer, &packetContainer))
    {
        LOG_ERROR("Failed to deserialize received network data!");
        return false;
    }

    // Allocate and deserialize packet.
    packet = std::unique_ptr<PacketBase>(PacketBase::create(packetContainer.packetType));
    if(!deserialize(packetContainer.packetBuffer, packet.get()))
    {
        LOG_ERROR("Failed to deserialize received packet!");
        return false;
    }

    return true;
}

bool Network::sendData(const MemoryBuffer& buffer, const sf::IpAddress& address, unsigned short port)
{
    LOG_TRACE("Sending network data to %s:%hu (%u size).", address.toString().c_str(), port, buffer.size());

    // Send packet read from memory buffer.
    if(m_udpSocket.send(buffer.data(), buffer.size(), address, port) != sf::Socket::Done)
    {
        LOG_ERROR("Sending network data resulted in an error!");
        return false;
    }

    return true;
}

bool Network::receiveData(MemoryBuffer& buffer, sf::IpAddress& address, unsigned short& port)
{
    // Receive packet and write it into memory buffer.
    char datagramBuffer[sf::UdpSocket::MaxDatagramSize] = { 0 };
    std::size_t bytesReceived = 0;

    auto status = m_udpSocket.receive(&datagramBuffer[0], sf::UdpSocket::MaxDatagramSize, bytesReceived, address, port);

    switch(status)
    {
        case sf::Socket::Partial:
            VERIFY(false, "Received partial network data while using UDP socket!");
            return false;

        case sf::Socket::Error:
            LOG_ERROR("Failed to receive network data from socket!");
            return false;

        case sf::Socket::Disconnected:
        case sf::Socket::NotReady:
            return false;
    }

    LOG_TRACE("Received network data from %s:%hu (%u size).", address.toString().c_str(), port, bytesReceived);

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
