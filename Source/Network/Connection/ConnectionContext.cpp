#include "Precompiled.hpp"
#include "ConnectionContext.hpp"
#include "ConnectionSocket.hpp"

ConnectionContext::PacketEntry::PacketEntry() :
    address(sf::IpAddress::None),
    port(0),
    sequenceIndex(0),
    acknowledgmentIndex(0),
    transportMethod(0),
    transportExtra(0)
{
}

ConnectionContext::ConnectionContext(ConnectionSocket* connectionSocket) :
    m_connectionSocket(connectionSocket),
    m_mutex(),
    m_sequenceIndex(0),
    m_acknowledgmentIndex(0),
    m_outgoingQueue(),
    m_incomingQueue(),
    m_supportsReliability(false),
    m_initialized(false)
{
}

ConnectionContext::~ConnectionContext()
{
}

bool ConnectionContext::initialize(bool supportReliability)
{
    // Mark context as either supporting reliability or not.
    m_supportsReliability = supportReliability;

    // Success!
    m_initialized = true;
    return true;
}

bool ConnectionContext::pushOutgoing(const PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Verify packet entry.
    if(packetEntry.transportMethod == (uint32_t)TransformMethod::Reliable)
    {
        ASSERT(m_supportsReliability, "Socket must support reliability to send reliable packets!");
    }

    ASSERT(packetEntry.sequenceIndex == 0, "Sequence index field should have been left untouched!");
    ASSERT(packetEntry.acknowledgmentIndex == 0, "Acknowedlgment index field should have been left untouched!");
    ASSERT(packetEntry.transportExtra == 0, "Transport extra field should have been left untouched!");

    // Add outgoing packet to queue.
    PacketEntry& addedEntry = m_outgoingQueue.emplace(packetEntry);

    // Fill packet sequence.
    addedEntry.sequenceIndex = ++m_sequenceIndex;
    
    // Packet has been added successfully to queue.
    return true;
}

bool ConnectionContext::peekOutgoing(PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if outgoing queue is empty.
    if(m_outgoingQueue.empty())
        return false;

    // Copy packet data.
    packetEntry = m_outgoingQueue.front();

    // Outgoing packet has been successfully read.
    return true;
}

bool ConnectionContext::popOutgoing(PacketEntry* packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if outgoing queue is empty.
    if(m_outgoingQueue.empty())
        return false;

    // Get next outgoing packet.
    PacketEntry& outgoingPacket = m_outgoingQueue.front();

    // Update with current acknowledgment index.
    outgoingPacket.acknowledgmentIndex = m_acknowledgmentIndex;

    // Copy packet entry.
    if(packetEntry != nullptr)
    {
        *packetEntry = outgoingPacket;
    }

    // Add packet to reliable queue if it is a reliable packet.
    if(outgoingPacket.transportMethod == (uint32_t)ConnectionContext::TransformMethod::Reliable)
    {
        this->pushReliable_NoLock(outgoingPacket);
    }

    // Pop read packet.
    m_outgoingQueue.pop();

    // Successfully popped outgoing packet.
    return true;
}

bool ConnectionContext::pushIncoming(const PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Read acknowledgment index for reliable packets.
    if(m_supportsReliability)
    {
        if(packetEntry.transportMethod == (uint32_t)TransformMethod::Reliable)
        {
            // Verify that incoming packet arrived from expected source.
            ASSERT(packetEntry.address == m_connectionSocket->getRemoteAddress());
            ASSERT(packetEntry.port == m_connectionSocket->getRemotePort());

            // Drop packets that are already acknowledged.
            if(packetEntry.sequenceIndex <= m_acknowledgmentIndex)
                return false;
        }

        // Increment acknowledgment index.
        m_acknowledgmentIndex = std::max(m_acknowledgmentIndex, packetEntry.sequenceIndex);
    }

    // Add packet to incoming queue.
    m_incomingQueue.push(packetEntry);

    // Successfully pushed incoming packet.
    return true;
}

bool ConnectionContext::peekIncoming(PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if incoming queue is empty.
    if(m_incomingQueue.empty())
        return false;

    // Retrieve packet from incoming queue.
    packetEntry = m_incomingQueue.front();

    // Packet has been successfully read.
    return true;
}

bool ConnectionContext::popIncoming(PacketEntry* packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if incoming queue is empty.
    if(m_incomingQueue.empty())
        return false;

    // Retrieve packet from incoming queue.
    if(packetEntry != nullptr)
    {
        *packetEntry = m_incomingQueue.front();
    }

    // Pop read packet from queue.
    m_incomingQueue.pop();

    // Packet has been successfully read and popped.
    return true;
}

bool ConnectionContext::pushReliable(const PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Call lock less version of the method.
    return this->pushReliable_NoLock(packetEntry);
}

bool ConnectionContext::pushReliable_NoLock(const PacketEntry& packetEntry)
{
    // Check if reliability is supported.
    ASSERT(m_supportsReliability, "Socket does not support reliability!");

    // Check if packet entry is reliable.
    ASSERT(packetEntry.transportMethod == (uint32_t)TransformMethod::Reliable, "Packet entry must be reliable!");

    // Add reliable packet to queue.
    m_reliableQueue.emplace_back(packetEntry);

    // Packet has been added successfully to queue.
    return true;
}

void ConnectionContext::copyUnacknowledged(std::queue<PacketEntry>& packetQueue)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if reliability is supported.
    ASSERT(m_supportsReliability, "Socket does not support reliability!");

    // Copy unacknowledged packets to provided queue.
    for(auto& packetEntry : m_reliableQueue)
    {
        // Update acknowledgment index.
        packetEntry.acknowledgmentIndex = m_acknowledgmentIndex;

        // Push copy to provided queue.
        packetQueue.push(m_reliableQueue.front());
    }
}

void ConnectionContext::popAcknowledged()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // Check if reliability is supported.
    ASSERT(m_supportsReliability, "Socket does not support reliability!");

    // Check if reliable queue is empty.
    // This should be always empty for sockets that do not support reliability.
    while(!m_reliableQueue.empty())
    {
        // Check if packet has already been acknowledged.
        PacketEntry& reliableEntry = m_reliableQueue.front();

        if(reliableEntry.sequenceIndex > m_acknowledgmentIndex)
        {
            // Packet has not been acknowledged yet.
            // Remaining packets are not acknowledged either.
            return;
        }

        // Pop acknowledged packet from queue.
        m_reliableQueue.pop_front();
    }
}

bool ConnectionContext::supportsReliability() const
{
    return m_supportsReliability;
}
