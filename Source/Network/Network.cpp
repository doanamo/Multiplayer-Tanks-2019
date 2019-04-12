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
