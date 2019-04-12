#include "Precompiled.hpp"
#include "Network/Client.hpp"
#include "Network/Protocol.hpp"

ConsoleVariable<std::string> cv_connect("connect", "127.0.0.1:2076");

Client::Client() :
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
    // Parse server address and string.
    auto addressPortSeparator = cv_connect.value.find_first_of(':');

    if(addressPortSeparator == std::string::npos || addressPortSeparator == 0)
    {
        LOG_ERROR("Incorrectly formatted connect variable that should "
            "contain address and port separated by ':' character!");
        return false;
    }

    std::string serverAddressString(cv_connect.value, 0, addressPortSeparator);
    std::string serverPortString(cv_connect.value, addressPortSeparator + 1);

    // Save server address and string.
    m_serverAddress = serverAddressString;

    if(!ParseStringToPort(serverPortString, m_serverPort))
        return false;

    // Initializes the base network interface.
    if(!Network::initializeSocket())
        return false;

    return true;
}

void Client::onUpdate(float timeDelta)
{
    // Send packets.
    m_hearbeatTimer = std::max(0.0f, m_hearbeatTimer - timeDelta);

    if(m_hearbeatTimer == 0.0f)
    {
        LOG_TRACE("Sending packet to %s:%hu.", m_serverAddress.toString().c_str(), m_serverPort);

        PacketHeader packetHeader;
        packetHeader.type = PacketType::Network_Heartbeat;

        MemoryBuffer packetBuffer;
        serialize(packetBuffer, packetHeader);

        if(m_socket.send(packetBuffer.data(), packetBuffer.size(),
            m_serverAddress, m_serverPort) != sf::Socket::Done)
        {
            LOG_ERROR("Sending packet resulted in an error!");
        }

        m_hearbeatTimer = 1.0f;
    }

    // Receive packets.
    MemoryBuffer packetBuffer;
    packetBuffer.resize(sf::UdpSocket::MaxDatagramSize);
    std::size_t bytesReceived;

    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(true)
    {
        // Check if we received any packets.
        auto status = m_socket.receive(packetBuffer.data(),
            sf::UdpSocket::MaxDatagramSize,
            bytesReceived, senderAddress,
            senderPort);

        ASSERT(status != sf::Socket::Partial);
        ASSERT(status != sf::Socket::Error);

        if(status == sf::Socket::Disconnected)
            break;

        if(status == sf::Socket::NotReady)
            break;

        if(status != sf::Socket::Done)
            continue;

        LOG_TRACE("Received packet from %s:%hu.", senderAddress.toString().c_str(), senderPort);
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

