#include "Precompiled.hpp"
#include "NetworkClient.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"

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

    // Initialize socket connection.
    if(!m_socket.connect(address, port))
    {
        LOG_ERROR("Could not initialize socket connection!");
        return false;
    }

    // Request game state snapshot.
    while(true)
    {
        // Send connection request packet until we get a response.
        std::unique_ptr<PacketBase> receivedPacket;

        do
        {
            // Send connection request to server.
            PacketConnect connectPacket;
            sendPacket(m_socket, connectPacket, false);

            // Wait to avoid spamming.
            sf::sleep(sf::milliseconds(100));
        }
        while(!receivePacket(m_socket, receivedPacket, nullptr));

        // Check if packet type matches expected type.
        PacketStateSnapshot* snapshotPacket = receivedPacket->as<PacketStateSnapshot>();
        if(receivedPacket == nullptr)
            continue;

        // Deserialize game state save into game instance.
        if(!deserialize(snapshotPacket->serializedGameInstance, m_gameInstance))
            continue;

        // Break free.
        break;
    }

    // Success!
    return true;
}

void NetworkClient::update(float timeDelta)
{
    // Send packets.
    m_hearbeatTimer = std::max(0.0f, m_hearbeatTimer - timeDelta);

    if(m_hearbeatTimer == 0.0f)
    {
        PacketMessage packetMessage;
        packetMessage.text = "Hello server!";

        sendPacket(m_socket, packetMessage, false);

        m_hearbeatTimer = 1.0f;
    }

    // Receive packets.
    std::unique_ptr<PacketBase> receivedPacket;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(receivePacket(m_socket, receivedPacket, nullptr, &senderAddress, &senderPort))
    {
        PacketMessage* packetMessage = receivedPacket->as<PacketMessage>();

        if(packetMessage)
        {
            LOG_INFO("Received message packet with \"%s\" text.", packetMessage->text.c_str());
        }
    }
}

void NetworkClient::tick(float timeDelta)
{
}

void NetworkClient::draw()
{
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
