#include "Precompiled.hpp"
#include "NetworkServer.hpp"
#include "Network/Connection/ConnectionBackend.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

NetworkServer::NetworkServer()
{
}

NetworkServer::~NetworkServer()
{
    LOG_TRACE("NetworkServer::~NetworkServer()");
}

bool NetworkServer::initialize(GameInstance* gameInstance, unsigned short port)
{
    // Initializes UDP socket.
    if(!NetworkBase::initialize(gameInstance))
        return false;

    // Initialize socket connection.
    if(!m_socket.listen(port))
    {
        LOG_ERROR("Could not initialize socket connection!");
        return false;
    }

    return true;
}

void NetworkServer::update(float timeDelta)
{
}

void NetworkServer::tick(float timeDelta)
{
    // Received packet data.
    MemoryStream receivedPacketData;
    sf::IpAddress remoteAddress;
    unsigned short remotePort;

    // Receive connection packets.
    while(m_socket.receive(receivedPacketData, nullptr, &remoteAddress, &remotePort))
    {
        // Check packet type.
        TypeInfo::IdentifierType packetType = 0;

        if(!deserialize(receivedPacketData, &packetType))
            continue;

        if(packetType != getTypeIdentifier<PacketConnect>())
            continue;

        // Check if we already have socket registered with remote address and port.
        if(m_socket.getConnectionBackend()->hasSocketRegistered(remoteAddress, remotePort))
            continue;

        // Add new client connection list.
        std::unique_ptr<ConnectionSocket> clientSocket = std::make_unique<ConnectionSocket>(m_socket.getConnectionBackend());

        if(!clientSocket->connect(remoteAddress, remotePort))
        {
            LOG_ERROR("Could not connect new client socket!");
            continue;
        }

        // Move socket to client list.
        ClientEntry& clientEntry = m_clients.emplace_back();
        clientEntry.socket = std::move(clientSocket);
    }

    // Receive packets from connected clients.
    for(auto& clientEntry : m_clients)
    {
        while(clientEntry.socket->receive(receivedPacketData, nullptr))
        {
            // Check packet type.
            TypeInfo::IdentifierType packetType = 0;
            if(!deserialize(receivedPacketData, &packetType))
                continue;

            // Respond to received packet.
            if(packetType == getTypeIdentifier<PacketConnect>())
            {
                // Prepare world for saving.
                m_gameInstance->getWorld()->flushObjects();

                // Serialize game instance.
                PacketStateSnapshot packetStateSnapshot;
                if(!serialize(packetStateSnapshot.serializedGameInstance, *m_gameInstance))
                    continue;

                // Serialize packet data.
                MemoryStream packetData;
                if(!serialize(packetData, packetStateSnapshot))
                    continue;

                // Send serialized packet.
                if(!clientEntry.socket->send(packetData, false))
                {
                    LOG_ERROR("Failed to send state snapshot packet to client!");
                    continue;
                }
            }
            else if(packetType == getTypeIdentifier<PacketMessage>())
            {
                // Deserialize packet.
                PacketMessage packetMessage;
                if(!deserialize(receivedPacketData, &packetMessage))
                    continue;

                LOG_INFO("Received message packet with \"%s\" text.", packetMessage.text.c_str());

                // Send response packet.
                {
                    PacketMessage packetMessage;
                    packetMessage.text = "Hello server!";

                    MemoryStream packetData;
                    if(serialize(packetData, packetMessage))
                    {
                        if(!clientEntry.socket->send(packetData, false))
                        {
                            LOG_ERROR("Failed to send message packet to client!");
                            continue;
                        }
                    }
                }
            }
        }
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
