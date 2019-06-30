#pragma once

#include "Precompiled.hpp"
#include "ConnectionContext.hpp"

// Forward declarations.
class ConnectionBackend;

// Connections socket class.
class ConnectionSocket
{
public:
    // Constructor with optional backend parameter.
    // Pass existing backend if multiple connection sockets want to use same port.
    ConnectionSocket(std::shared_ptr<ConnectionBackend> connectionBackend = nullptr);
    ~ConnectionSocket();

    // Copy operations.
    ConnectionSocket(const ConnectionSocket& other) = delete;
    ConnectionSocket& operator=(const ConnectionSocket& other) = delete;

    // Connects to remote address and port.
    // Supports reliable connection protocol.
    bool connect(const sf::IpAddress& remoteAddress, unsigned short remotePort);

    // Listens to connections at specified port.
    // Cannot support reliable connection protocol.
    bool listen(unsigned short localPort = sf::UdpSocket::AnyPort);

    // Sends packet.
    // Reliable packet send should not have address and port arguments provided - remote address and port will be used.
    bool send(const MemoryStream& packet, bool reliable, sf::IpAddress* address = nullptr, unsigned short* port = nullptr);
    
    // Receives packet.
    // In all cases address and port will point at remote source of received packet.
    bool receive(MemoryStream& packet, bool* reliable = nullptr, sf::IpAddress* address = nullptr, unsigned short* port = nullptr);

    // Gets connection backend.
    const std::shared_ptr<ConnectionBackend>& getConnectionBackend();

    // Gets connection context.
    ConnectionContext& getConnectionContext();

    // Gets remote address.
    const sf::IpAddress& getRemoteAddress() const;

    // Gets remote port.
    unsigned short getRemotePort() const;

    // Gets local port.
    unsigned short getLocalPort() const;

    // Checks whether connection supports reliability.
    bool supportsReliability() const;

private:
    // Connection backend.
    // Can be shared with other sockets that use same port.
    std::shared_ptr<ConnectionBackend> m_connectionBackend;

    // Connection context.
    ConnectionContext m_connectionContext;

    // Remote address and port.
    sf::IpAddress m_remoteAddress;
    unsigned short m_remotePort;

    // Initialization state.
    bool m_initialized;
};
