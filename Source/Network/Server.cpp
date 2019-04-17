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
    MemoryBuffer receivedPacket;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(this->receivePacket(receivedPacket, senderAddress, senderPort))
    {
        // Send response packet.
        PacketHeader packetHeader;
        packetHeader.type = PacketType::Network_Heartbeat;

        MemoryBuffer sentPacket;
        serialize(sentPacket, packetHeader);

        this->sendPacket(sentPacket, senderAddress, senderPort);
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
