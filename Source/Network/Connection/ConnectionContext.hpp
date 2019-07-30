#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketHeader.hpp"

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
        PacketHeader header;

        // Packet data.
        MemoryStream packet;
    };

    // Outgoing packet priority queue.
    using OutgoingQueue = std::queue<PacketEntry>;

    // Incoming packet priority queue.
    struct IncomingPacketCompare
    {
        bool operator()(const PacketEntry& left, const PacketEntry& right) const
        {
            // Make lower sequence indices appear at the top of queue.
            return left.header.sequenceIndex > right.header.sequenceIndex;
        }
    };

    using IncomingQueue = std::priority_queue<PacketEntry, std::deque<PacketEntry>, IncomingPacketCompare>;

public:
    ConnectionContext(ConnectionSocket* connectionSocket);
    ~ConnectionContext();

    // Initializes connection context.
    bool initialize(bool supportReliability);

    // Pushes outgoing packet that will be processed for sending.
    bool pushOutgoing(const PacketEntry& packetEntry);
    
    // Pops outgoing packet that will be processed for sending.
    bool popOutgoing(PacketEntry* packetEntry = nullptr);

    // Pushes incoming packet that will be processed for receiving.
    bool pushIncoming(const PacketEntry& packetEntry);

    // Pops incoming packet that will be processed for receiving.
    bool popIncoming(PacketEntry* packetEntry = nullptr);
    
    // Pushes reliable packet to queue until it gets acknowledged.
    bool pushReliable(const PacketEntry& packetEntry);

    // Copies all reliable packet that are waiting next for acknowledgment.
    void copyUnacknowledged(std::queue<PacketEntry>& packetQueue);

    // Pops all reliable packet if it has already been acknowledged.
    void popAcknowledged();

    // Checks if context supports reliability.
    bool supportsReliability() const;

private:
    // Lock less versions of public methods.
    bool pushOutgoing_NoLock(const PacketEntry& packetEntry);
    bool pushReliable_NoLock(const PacketEntry& packetEntry);

    // Get current acknowledgment index for packets received from remote.
    uint32_t determineAcknowledgmentIndex() const;

private:
    // Connection socket.
    ConnectionSocket* m_connectionSocket;

    // Instance mutex.
    std::mutex m_mutex;

    // Outgoing and incoming packet queues.
    OutgoingQueue m_outgoingQueue;
    IncomingQueue m_incomingQueue;

    // List of reliable packet that still need to be acknowledged.
    std::deque<PacketEntry> m_reliableQueue;
    
    // Last outgoing sequence index.
    // Incremented with each new outgoing packet.
    uint32_t m_outgoingSequenceIndex;

    // Last incoming sequence index.
    // Most recent sequence index that was received from remote.
    uint32_t m_incomingSequenceIndex;
    
    // Last outgoing sequence index.
    // Most recent reliable index that was sent to remote.
    uint32_t m_outgoingReliableIndex;

    // Last incoming reliable index.
    // Most recent reliable index that was received from remote.
    // Both immediate and deferred values represent the same thing.
    // Difference between both is that they are updated and used at different times.
    // Immediate is used when packet is received to immediately respond with acknowledgment if possible.
    // Deferred is used after incoming queue is filled to verify integrity of reliable sequence in more robust way.
    uint32_t m_immediateIncomingReliableIndex;
    uint32_t m_deferredIncomingReliableIndex;

    // Last acknowledgment index returned from remote.
    // Acknowledgment index must include all past reliable indices, but not necessarily all unreliable.
    uint32_t m_remoteAcknowledgmentIndex;

    // Whether acknowledgment for remote should be sent.
    bool m_sendAcknowledgment;

    // Whether context supports reliability.
    bool m_supportsReliability;

    // Initialization state.
    bool m_initialized;
};
