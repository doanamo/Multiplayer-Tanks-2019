#include "Precompiled.hpp"
#include "NetworkClient.hpp"
#include "Network/Replication/ReplicationClient.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"
#include "Game/SnapshotSaveLoad.hpp"

NetworkClient::NetworkClient() :
    m_hearbeatTimer(0.0f)
{
}

NetworkClient::~NetworkClient()
{
}

bool NetworkClient::initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port)
{
    // Initialize base class.
    if(!NetworkBase::initialize(gameInstance))
        return false;

    // Initialize replication system.
    m_replication = std::make_unique<ReplicationClient>();
    if(!m_replication->initialize(gameInstance))
        return false;

    // Initialize socket connection.
    if(!m_socket.connect(address, port))
    {
        LOG_ERROR("Could not initialize socket connection!");
        return false;
    }

    // Send connection request to server.
    PacketConnect connectPacket;
    sendPacket(m_socket, connectPacket, true);

    // Receive and process state snapshot.
    while(true)
    {
        // Wait until we receive packet.
        std::unique_ptr<PacketBase> receivedPacket;
        if(!receivePacket(m_socket, receivedPacket))
            continue;

        // Check if packet type matches expected type.
        PacketServerSnapshot* snapshotPacket = receivedPacket->as<PacketServerSnapshot>();
        if(snapshotPacket == nullptr)
            continue;

        // Load snapshot from packet memory.
        SnapshotSaveLoad snapshotLoad(m_gameInstance);
        if(!snapshotLoad.load(snapshotPacket->serializedGameInstance))
        {
            LOG_ERROR("Could not load snapshot from packet memory!");
            continue;
        }

        // Break out of while loop.
        break;
    }

    // Success!
    return true;
}

void NetworkClient::update(float timeDelta)
{
    NetworkBase::update(timeDelta);

    // Send periodical packet.
    m_hearbeatTimer = std::max(0.0f, m_hearbeatTimer - timeDelta);

    if(m_hearbeatTimer == 0.0f)
    {
        PacketMessage packetMessage;
        packetMessage.text = "Hello server!";

        sendPacket(m_socket, packetMessage, true);

        m_hearbeatTimer = 1.0f;
    }

    // Receive packets.
    std::unique_ptr<PacketBase> receivedPacket;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(receivePacket(m_socket, receivedPacket, nullptr, &senderAddress, &senderPort))
    {
        if(receivedPacket->is<PacketMessage>())
        {
            PacketMessage* packetMessage = receivedPacket->as<PacketMessage>();
            LOG_INFO("Received message packet with \"%s\" text.", packetMessage->text.c_str());
        }
    }
}

void NetworkClient::tick(float timeDelta)
{
    NetworkBase::tick(timeDelta);
}

void NetworkClient::draw()
{
    NetworkBase::draw();

    // Draw ImGui debug window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 100));
    if(ImGui::Begin("Network State (Client)##NetworkState", &cv_showNetwork.value))
    {

    }
    ImGui::End();
    ImGui::PopStyleVar(1);
}

NetworkMode NetworkClient::getMode() const
{
    return NetworkMode::Client;
}

bool NetworkClient::isConnected() const
{
    return false;
}
