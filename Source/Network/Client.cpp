#include "Precompiled.hpp"
#include "Network/Client.hpp"
#include "Network/Protocol.hpp"

Client::Client() :
    m_socket(),
    m_serverAddress(),
    m_serverPort(0),
    m_hearbeatTimer(0.0f)
{
}

Client::~Client()
{
}

bool Client::initialize()
{
    // Get server address.
    m_serverAddress = cv_connect.value;

    // Get server port.
    unsigned int serverPort = std::stoi(cv_port.value);

    if(serverPort > std::numeric_limits<unsigned short>::max())
    {
        LOG_ERROR("Server port number %i is outside of valid range!", serverPort);
        return false;
    }

    m_serverPort = (unsigned short)serverPort;

    return true;
}

void Client::onUpdate(float timeDelta)
{
    m_hearbeatTimer = std::max(0.0f, m_hearbeatTimer - timeDelta);

    if(m_hearbeatTimer == 0.0f)
    {
        PacketHeader packetHeader;
        packetHeader.type = PacketType::Network_Heartbeat;

        MemoryBuffer packetBuffer;
        serialize(packetBuffer, packetHeader);

        if(m_socket.send(packetBuffer.data(), packetBuffer.size(),
            m_serverAddress, m_serverPort) != sf::Socket::Done)
        {
            LOG_ERROR("Sending network packet resulted in an error!");
        }

        LOG_TRACE("Sent packe to server.");

        m_hearbeatTimer = 1.0f;
    }
}

void Client::onTick(float timeDelta)
{
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

int Client::getClientIndex() const
{
    return 0;
}

