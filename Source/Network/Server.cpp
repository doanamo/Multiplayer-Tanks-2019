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
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(this->receivePacket(packetBuffer, senderAddress, senderPort))
    {
        PacketHeader packetHeader;
        if(deserialize(packetBuffer, &packetHeader))
        {
            if(packetHeader.type == getTypeIdentifier<PacketMessage>())
            {
                PacketMessage packetMessage;
                if(deserialize(packetBuffer, &packetMessage))
                {
                    LOG_INFO("Received message packet with \"%s\" text.", packetMessage.text.c_str());
                }
            }
            else
            {
                LOG_ERROR("Unknown packet type received!");
            }
        }
        else
        {
            LOG_ERROR("Failed to deserialize packet!");
        }

        // Send response packet.
        {
            PacketMessage packetMessage;
            packetMessage.text = "Hello client!";

            PacketHeader packetHeader;
            packetHeader.type = getTypeIdentifier<PacketMessage>();

            MemoryBuffer packetBuffer;
            if(serialize(packetBuffer, packetHeader) && serialize(packetBuffer, packetMessage))
            {
                this->sendPacket(packetBuffer, senderAddress, senderPort);
            }
            else
            {
                LOG_ERROR("Failed to serialize packet!");
            }
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
