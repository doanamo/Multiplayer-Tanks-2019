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

void Server::update(float timeDelta)
{
    // Receive packets.
    std::unique_ptr<PacketBase> receivedPacket;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(receivePacket(receivedPacket, senderAddress, senderPort))
    {
        TypeInfo::IdentifierType packetType = getTypeIdentifier(*receivedPacket);

        if(packetType == getTypeIdentifier<PacketMessage>())
        {
            PacketMessage* packetMessage = (PacketMessage*)receivedPacket.get();
            LOG_INFO("Received message packet with \"%s\" text.", packetMessage->text.c_str());
        }

        // Send response packet.
        PacketMessage packetMessage;
        packetMessage.text = "Hello client!";

        sendPacket(packetMessage, senderAddress, senderPort);
    }
}

void Server::tick(float timeDelta)
{
}

void Server::draw()
{
    // Draw ImGui debug window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 100));
    if(ImGui::Begin("Network State (Server)##NetworkState", &cv_showNetwork.value))
    {
        
    }
    ImGui::End();
    ImGui::PopStyleVar(1);
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
