#pragma once

#include "Precompiled.hpp"

// Forward declarations.
class ConnectionSocket;

// Connection context class.
class ConnectionContext
{
public:
    // Transport method.
    // This should be a bit flag in the future.
    enum class TransformMethod
    {
        Unreliable,
        Reliable,
    };

    // Packet structure.
    struct PacketEntry
    {
        PacketEntry();

        // Packet source.
        sf::IpAddress address;
        unsigned short port;

        // Packet header.
        uint32_t sequenceIndex;
        uint32_t acknowledgmentIndex;
        uint32_t transportMethod;
        uint32_t transportExtra;

        // Packet data.
        MemoryStream packet;
    };

public:
    ConnectionContext(ConnectionSocket* connectionSocket);
    ~ConnectionContext();

    // Initializes connection context.
    bool initialize(bool supportReliability);

    // Pushes outgoing packet that will be processed for sending.
    bool pushOutgoing(const PacketEntry& packetEntry);
    
    // Peeks outgoing packet that will be processed next for sending.
    bool peekOutgoing(PacketEntry& packetEntry);

    // Pops outgoing packet that will be processed for sending.
    bool popOutgoing(PacketEntry* packetEntry = nullptr);

    // Pushes incoming packet that will be processed for receiving.
    bool pushIncoming(const PacketEntry& packetEntry);

    // Peeks incoming packet that will be processed next for receiving.
    bool peekIncoming(PacketEntry& packetEntry);

    // Pops incoming packet that will be processed for receiving.
    bool popIncoming(PacketEntry* packetEntry = nullptr);
    
    // Pushes reliable packet to queue until it gets acknowledged.
    bool pushReliable(const PacketEntry& packetEntry);

    // Peeks reliable packet that is waiting next for acknowledgment.
    bool peekReliable(PacketEntry& packetEntry);

    // Pops reliable packet if it has already been acknowledged.
    bool popReliable(PacketEntry* packetEntry = nullptr);

    // Checks if context supports reliability.
    bool supportsReliability() const;

private:
    // Connection socket.
    ConnectionSocket* m_connectionSocket;

    // Instance mutex.
    std::mutex m_mutex;

    // Outgoing and incoming packet queues.
    std::queue<PacketEntry> m_outgoingQueue;
    std::queue<PacketEntry> m_incomingQueue;

    // Whether connection using this context supports reliability.
    bool m_supportsReliability;

    // Current sequence indices.
    uint32_t m_sequenceIndex;
    uint32_t m_acknowledgmentIndex;

    // Reliable packets that need to be acknowledged.
    std::queue<PacketEntry> m_reliableQueue;
    
    // Initialization state.
    bool m_initialized;
};
