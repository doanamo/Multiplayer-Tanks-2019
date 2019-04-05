#include "Precompiled.hpp"
#include "Network/Server.hpp"

Server::Server() :
    m_socket()
{
}

Server::~Server()
{
}

bool Server::initialize()
{
    // Get server port number.
    unsigned int serverPort = std::stoi(cv_port.value);

    if(serverPort > std::numeric_limits<unsigned short>::max())
    {
        LOG_ERROR("Server port number %i is outside of valid range!", serverPort);
        return false;
    }

    // Bind socket to port.
    if(m_socket.bind((unsigned short)serverPort) != sf::Socket::Done)
        return false;

    // Disable blocking mode.
    m_socket.setBlocking(false);

    return true;
}

void Server::onUpdate(float timeDelta)
{
    MemoryBuffer packetBuffer;
    packetBuffer.resize(sf::UdpSocket::MaxDatagramSize);
    std::size_t bytesReceived;

    sf::IpAddress clientAddress;
    unsigned short clientPort;

    while(m_socket.receive(packetBuffer.data(), sf::UdpSocket::MaxDatagramSize,
        bytesReceived, clientAddress, clientPort) == sf::Socket::Done)
    {
        LOG_TRACE("Received packet from client.");
    }
}

void Server::onTick(float timeDelta)
{
}

bool Server::isConnected() const
{
    return true;
}

bool Server::isServer() const
{
    return true;
}

bool Server::isClient() const
{
    return false;
}

int Server::getClientIndex() const
{
    return 0;
}
