#include "Precompiled.hpp"
#include "NetworkServer.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

NetworkServer::NetworkServer()
{
}

NetworkServer::~NetworkServer()
{
}

bool NetworkServer::initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port)
{
    // Initializes UDP socket.
    if(!NetworkBase::initialize(gameInstance, address, port))
        return false;

    // Listen for incoming TCP connections.
    m_tcpListener.setBlocking(false);

    if(m_tcpListener.listen(port) != sf::Socket::Done)
    {
        LOG_ERROR("Could not start listening to incoming TCP connections!");
        return false;
    }

    return true;
}

void NetworkServer::update(float timeDelta)
{
}

void NetworkServer::tick(float timeDelta)
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

        // Prepare world for saving.
        m_gameInstance->getWorld()->flushObjects();

        // Serialize game instance.
        PacketStateSave packetStateSave;
        if(!serialize(packetStateSave.serializedGameInstance, *m_gameInstance))
            continue;

        // Send current game instance.
        if(!sendTcpPacket(packetStateSave, *clientEntry.socket))
        {
            LOG_ERROR("Could not send game state save packet!");
        }
    }

    // Receive packets.
    std::unique_ptr<PacketBase> receivedPacket;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(receiveUdpPacket(receivedPacket, senderAddress, senderPort))
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

        sendUdpPacket(packetMessage, senderAddress, senderPort);
    }
}

void NetworkServer::draw()
{
    // Draw ImGui debug window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 100));
    if(ImGui::Begin("Network State (Server)##NetworkState", &cv_showNetwork.value))
    {
    }
    ImGui::End();
    ImGui::PopStyleVar(1);
}

NetworkMode NetworkServer::getMode() const
{
    return NetworkMode::Server;
}

bool NetworkServer::isConnected() const
{
    return true;
}
