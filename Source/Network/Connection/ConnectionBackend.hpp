#pragma once

#include "Precompiled.hpp"

// Forward declarations.
class ConnectionSocket;

// Connection backend class.
class ConnectionBackend
{
public:
    // Type declarations.
    using SocketMap = std::map<std::pair<sf::IpAddress, unsigned short>, ConnectionSocket*>;

public:
    ConnectionBackend();
    ~ConnectionBackend();

    // Initializes connection backend for specific local port.
    // Can be called multiple times for different connection sockets.
    bool initialize(unsigned short localPort = sf::UdpSocket::AnyPort);

    // Registers connection socket.
    bool registerSocket(ConnectionSocket* socket);

    // Unregister connection socket.
    void unregisterSocket(ConnectionSocket* socket);

    // Checks if there is socket with specified remote address and port registered.
    bool hasSocketRegistered(const sf::IpAddress& address, unsigned short port);

    // Gets default listen socket.
    ConnectionSocket* getDefaultListenSocket() const;

    // Gets listen port.
    unsigned short getLocalPort() const;

private:
    // Entry function for worker thread.
    static void workerThreadMain(ConnectionBackend* backend);

private:
    // Worker thread.
    std::thread m_workerThread;

    // Network UDP socket.
    // For sole use of worker thread (no lock required).
    sf::UdpSocket m_udpSocket;
    unsigned short m_localPort;

    // Connection sockets.
    ConnectionSocket* m_defaultSocket;
    SocketMap m_socketMap;

    // Mutex for operations connection sockets.
    std::mutex m_socketMutex;

    // Initialization state.
    std::atomic<bool> m_initialized;
};
