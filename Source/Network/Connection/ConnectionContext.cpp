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
    ASSERT(packetEntry.sequenceIndex == 0, "Sequence index field should have been left untouched!");
    ASSERT(packetEntry.acknowledgmentIndex == 0, "Acknowedlgment index field should have been left untouched!");
    ASSERT(packetEntry.transportExtra == 0, "Transport extra field should have been left untouched!");

    // Add outgoing packet to queue.
    PacketEntry& addedEntry = m_outgoingQueue.emplace(packetEntry);

    // Fill packet sequence and acknowledgment indices.
    addedEntry.sequenceIndex = ++m_sequenceIndex;
    addedEntry.acknowledgmentIndex = m_acknowledgmentIndex;

    // Packet has been added successfully to queue.
    return true;
}

bool ConnectionContext::popOutgoing(PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if outgoing queue is empty.
    if(m_outgoingQueue.empty())
        return false;

    // Copy packet data.
    packetEntry = m_outgoingQueue.front();

    // Pop read packet.
    m_outgoingQueue.pop();

    // Successfully popped outgoing packet.
    return true;
}

bool ConnectionContext::pushIncoming(const PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Read acknowledgment index for reliable packets.
    if(packetEntry.transportMethod == (uint32_t)TransformMethod::Reliable)
    {
        // Verify that incoming packet arrived from expected source.
        ASSERT(packetEntry.address == m_connectionSocket->getRemoteAddress());
        ASSERT(packetEntry.port == m_connectionSocket->getRemotePort());

        // Increment acknowledgment index.
        m_acknowledgmentIndex = std::max(m_acknowledgmentIndex, packetEntry.acknowledgmentIndex);
    }

    // Add packet to incoming queue.
    m_incomingQueue.push(packetEntry);

    // Successfully pushed incoming packet.
    return true;
}

bool ConnectionContext::peekIncoming(const PacketEntry** packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if incoming queue is empty.
    if(m_incomingQueue.empty())
    {
        *packetEntry = nullptr;
        return false;
    }

    // Save pointer to next incoming packet.
    *packetEntry = &m_incomingQueue.front();

    // There is incoming packet waiting that could be peeked.
    return true;
}

bool ConnectionContext::popIncoming(PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if incoming queue is empty.
    if(m_incomingQueue.empty())
        return false;

    // Retrieve packet from incoming queue.
    packetEntry = m_incomingQueue.front();

    // Pop read packet from queue.
    m_incomingQueue.pop();

    // Packet has been successfully read and popped.
    return true;
}

bool ConnectionContext::supportsReliability() const
{
    return m_supportsReliability;
}
