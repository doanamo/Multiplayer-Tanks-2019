#include "Precompiled.hpp"
#include "NetworkClient.hpp"
#include "Network/Protocol.hpp"
#include "Game/GameInstance.hpp"

NetworkClient::NetworkClient() :
    m_serverAddress(),
    m_serverPort(0),
    m_hearbeatTimer(0.0f)
{
}

NetworkClient::~NetworkClient()
{
}

bool NetworkClient::initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port)
{
    // Initialize base class.
    // Find any available port for client.
    if(!NetworkBase::initialize(gameInstance, address, 0))
        return false;

    // Save server address and port.
    m_serverAddress = address;
    m_serverPort = port;

    // Initialize TCP socket connection.
    if(m_tcpSocket.connect(address, port, sf::seconds(10.0f)) == sf::Socket::Done)
    {
        LOG_INFO("Connected to server at %s:%hu address.",
            address.toString().c_str(), port);
    }
    else
    {
        LOG_ERROR("Connection to server at %s:%hu address timed out!",
            address.toString().c_str(), port);

        return false;
    }

    // Set TCP socket as non blocking after we establish connection.
    m_tcpSocket.setBlocking(false);

    // Receive game state save.
    std::unique_ptr<PacketBase> receivedPacket;

    while(!receiveTcpPacket(receivedPacket, m_tcpSocket))
    {
        sf::sleep(sf::milliseconds(1));
    }

    // Verify that we received expected packet.
    if(getTypeIdentifier(*receivedPacket) != getTypeIdentifier<PacketStateSave>())
    {
        LOG_ERROR("Received unexpected packet instead of game state save!");
        return false;
    }

    PacketStateSave* packetStateSave = (PacketStateSave*)receivedPacket.get();

    // Deserialize game state save into game instance.
    if(!deserialize(packetStateSave->serializedGameInstance, m_gameInstance))
        return false;

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

        sendUdpPacket(packetMessage, m_serverAddress, m_serverPort);

        m_hearbeatTimer = 1.0f;
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
