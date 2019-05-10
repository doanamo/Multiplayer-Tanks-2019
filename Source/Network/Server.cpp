#include "Precompiled.hpp"
#include "Network/Server.hpp"
#include "Network/Protocol.hpp"

Server::Server()
{
}

Server::~Server()
{
}

bool Server::initialize(const sf::IpAddress& address, unsigned short port)
{
    // Listen for incoming TCP connections.
    m_tcpListener.setBlocking(false);

    if(m_tcpListener.listen(port) != sf::Socket::Done)
    {
        LOG_ERROR("Could not listen to incoming TCP connections!");
        return false;
    }

    // Initializes UDP socket.
    if(!Network::initialize(address, port))
        return false;

    return true;
}

void Server::update(float timeDelta)
{
}

void Server::tick(float timeDelta)
{
    // Listen for incoming connections.
    while(true)
    {
        // Add new potential client to client list.
        ClientEntry& clientEntry = m_clients.emplace_back();
        clientEntry.socket = std::make_unique<sf::TcpSocket>();

        // Attempt to connect a new client.
        if(m_tcpListener.accept(*clientEntry.socket) == sf::Socket::Done)
        {
            LOG_INFO("Client at %s:%hu address has been connected.",
                clientEntry.socket->getRemoteAddress().toString().c_str(),
                clientEntry.socket->getRemotePort());
        }
        else
        {
            // There are no (more) clients that are requesting connection.
            m_clients.pop_back();
            break;
        }
    }

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
