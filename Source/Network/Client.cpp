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
        PacketMessage packetMessage;
        packetMessage.text = "Hello server!";

        PacketHeader packetHeader;
        packetHeader.type = getTypeIdentifier<PacketMessage>();

        MemoryBuffer packetBuffer;
        if(serialize(packetBuffer, packetHeader) && serialize(packetBuffer, packetMessage))
        {
            this->sendPacket(packetBuffer, m_serverAddress, m_serverPort);
        }
        else
        {
            LOG_ERROR("Failed to serialize packet!");
        }

        m_hearbeatTimer = 1.0f;
    }

    // Receive packets.
    MemoryBuffer packetBuffer;
    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(this->receivePacket(packetBuffer, senderAddress, senderPort))
    {
        PacketHeader packetHeader;
        if(deserialize(packetBuffer, &packetHeader))
        {
            if(packetHeader.type == getTypeIdentifier<PacketMessage>())
            {
                PacketMessage packetMessage;
                if(deserialize(packetBuffer, &packetMessage))
                {
                    LOG_INFO("Received message packet with \"%s\" text.", packetMessage.text.c_str());
                }
            }
            else
            {
                LOG_ERROR("Unknown packet type received!");
            }
        }
        else
        {
            LOG_ERROR("Failed to deserialize packet!");
        }
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

