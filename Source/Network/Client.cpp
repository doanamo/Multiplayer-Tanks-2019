#include "Precompiled.hpp"
#include "Network/Client.hpp"
#include "Network/Protocol.hpp"

Client::Client() :
    m_serverAddress(),
    m_serverPort(0),
    m_hearbeatTimer(0.0f)
{
}

Client::~Client()
{
}

bool Client::initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port)
{
    // Initialize base class.
    // Find any available port for client.
    if(!Network::initialize(gameInstance, address, 0))
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

    return true;
}

void Client::update(float timeDelta)
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

void Client::tick(float timeDelta)
{
}

void Client::draw()
{
    // Draw ImGui debug window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 100));
    if(ImGui::Begin("Network State (Client)##NetworkState", &cv_showNetwork.value))
    {

    }
    ImGui::End();
    ImGui::PopStyleVar(1);
}

bool Client::isConnected() const
{
    return false;
}

bool Client::isServer() const
{
    return false;
}

bool Client::isClient() const
{
    return true;
}
