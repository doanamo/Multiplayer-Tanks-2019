#include "Precompiled.hpp"
#include "Network/Server.hpp"
#include "Network/Protocol.hpp"

ConsoleVariable<std::string> cv_host("host", "2076");

Server::Server()
{
}

Server::~Server()
{
}

bool Server::initialize()
{
    // Initializes the base network interface.
    if(!Network::initializeSocket(cv_host.value))
        return false;

    return true;
}

void Server::onUpdate(float timeDelta)
{
    // Receive packets.
    MemoryBuffer packetBuffer;
    packetBuffer.resize(sf::UdpSocket::MaxDatagramSize);
    std::size_t bytesReceived;

    sf::IpAddress senderAddress;
    unsigned short senderPort;

    {

    }

    while(true)
    {
        // Check if we received any packets.
        auto status = m_socket.receive(packetBuffer.data(),
            sf::UdpSocket::MaxDatagramSize,
            bytesReceived, senderAddress,
            senderPort);

        ASSERT(status != sf::Socket::Partial);
        ASSERT(status != sf::Socket::Error);

        if(status == sf::Socket::Disconnected)
            break;

        if(status == sf::Socket::NotReady)
            break;
        
        if(status != sf::Socket::Done)
            continue;

        LOG_TRACE("Received packet from %s:%hu.", senderAddress.toString().c_str(), senderPort);

        // Send response packet.
        LOG_TRACE("Sending packet to %s:%hu.", senderAddress.toString().c_str(), senderPort);

        PacketHeader packetHeader;
        packetHeader.type = PacketType::Network_Heartbeat;

        MemoryBuffer packetBuffer;
        serialize(packetBuffer, packetHeader);

        if(m_socket.send(packetBuffer.data(), packetBuffer.size(),
            senderAddress, senderPort) != sf::Socket::Done)
        {
            LOG_ERROR("Sending packet resulted in an error!");
        }
    }
}

void Server::onTick(float timeDelta)
{
}

bool Server::isConnected() const
{
    return true;
}

bool Server::isServer() const
{
    return true;
}

bool Server::isClient() const
{
    return false;
}

int Server::getClientIndex() const
{
    return 0;
}
