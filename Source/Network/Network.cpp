#include "Precompiled.hpp"
#include "Network/Network.hpp"

bool ParseStringToPort(std::string text, unsigned short& port)
{
    // Determine listen port.
    int portNumber = -1;

    if(!text.empty())
    {
        portNumber = std::stoi(text);
    }

    // Check listen port.
    if(portNumber < 0 || portNumber > std::numeric_limits<unsigned short>::max())
    {
        LOG_ERROR("Requested listen port number \"%s\" is outside of valid range!", text.c_str());
        return false;
    }

    port = (unsigned short)portNumber;

    return true;
}

Network::Network() :
    m_socket(),
    m_listenPort(0)
{
}

Network::~Network()
{
}

bool Network::initializeSocket(std::string listenPort)
{
    // Parse port number.
    if(!ParseStringToPort(listenPort, m_listenPort))
        return false;

    // Bind socket to port.
    if(m_socket.bind(m_listenPort) != sf::Socket::Done)
        return false;

    // Disable blocking mode.
    m_socket.setBlocking(false);

    return true;
}

bool Network::sendPacket(const MemoryBuffer& buffer, const sf::IpAddress& address, unsigned short port)
{
    LOG_TRACE("Sending packet to %s:%hu (%u size).", address.toString().c_str(), port, buffer.size());

    // Send packet read from memory buffer.
    if(m_socket.send(buffer.data(), buffer.size(), address, port) != sf::Socket::Done)
    {
        LOG_ERROR("Sending packet resulted in an error!");
        return false;
    }

    return true;
}

bool Network::receivePacket(MemoryBuffer& buffer, sf::IpAddress& address, unsigned short& port)
{
    // Receive packet and write it into memory buffer.
    char datagramBuffer[sf::UdpSocket::MaxDatagramSize] = { 0 };
    std::size_t bytesReceived = 0;

    auto status = m_socket.receive(&datagramBuffer[0], sf::UdpSocket::MaxDatagramSize, bytesReceived, address, port);

    switch(status)
    {
        case sf::Socket::Partial:
            VERIFY(false, "Received partial packet while using UDP socket!");
            return false;

        case sf::Socket::Error:
            LOG_ERROR("Failed to receive packets from socket!");
            return false;

        case sf::Socket::Disconnected:
        case sf::Socket::NotReady:
            return false;
    }

    LOG_TRACE("Received packet from %s:%hu (%u size).", address.toString().c_str(), port, bytesReceived);

    // Write datagram buffer into provided memory buffer.
    buffer.copy(&datagramBuffer[0], bytesReceived);

    return true;
}
