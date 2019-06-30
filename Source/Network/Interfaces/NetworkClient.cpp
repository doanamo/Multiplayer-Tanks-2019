#include "Precompiled.hpp"
#include "NetworkClient.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"

NetworkClient::NetworkClient() :
    m_socket(),
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
    MemoryStream packetData;

    do
    {
        // Send connection request to server.
        MemoryStream networkData;
        PacketConnect connectPacket;

        if(!serialize(networkData, getTypeIdentifier(connectPacket)))
            return false;

        if(!serialize(networkData, connectPacket))
            return false;

        m_socket.send(networkData, false);

        // Wait to avoid spamming.
        sf::sleep(sf::milliseconds(100));
    }
    while(!m_socket.receive(packetData, nullptr));

    // Read packet type.
    TypeInfo::IdentifierType packetType = 0;

    if(!deserialize(packetData, &packetType))
        return false;

    if(packetType != getTypeIdentifier<PacketStateSnapshot>())
        return false;

    // Deserialize packet packet.
    PacketStateSnapshot stateSnapshot;
    if(!deserialize(packetData, &stateSnapshot))
        return false;

    // Deserialize game state save into game instance.
    if(!deserialize(stateSnapshot.serializedGameInstance, m_gameInstance))
        return false;

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

        MemoryStream packetData;
        if(serialize(packetData, packetMessage))
        {
            m_socket.send(packetData, false);
        }

        m_hearbeatTimer = 1.0f;
    }

    // Receive packets.
    MemoryStream receivedPacketData;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(m_socket.receive(receivedPacketData, nullptr, &senderAddress, &senderPort))
    {
        // Read packet type.
        TypeInfo::IdentifierType packetType = 0;

        if(!deserialize(receivedPacketData, &packetType))
            continue;

        if(packetType != getTypeIdentifier<PacketMessage>())
            continue;

        // Deserialize packet.
        PacketMessage packetMessage;
        if(!deserialize(receivedPacketData, &packetMessage))
            continue;

        LOG_INFO("Received message packet with \"%s\" text.", packetMessage.text.c_str());
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
