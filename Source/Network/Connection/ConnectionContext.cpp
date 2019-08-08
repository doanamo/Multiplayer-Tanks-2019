#include "Precompiled.hpp"
#include "ConnectionContext.hpp"
#include "ConnectionSocket.hpp"
#include "ConnectionSettings.hpp"

#if ENABLE_CONNECTION_CONTEXT_LOG_TRACE
    #define LOG_CONNECTION_CONTEXT_TRACE(format, ...) LOG_TRACE(format, ## __VA_ARGS__)
#else
    #define LOG_CONNECTION_CONTEXT_TRACE(format, ...)
#endif

ConnectionContext::PacketEntry::PacketEntry() :
    address(sf::IpAddress::None),
    port(0)
{
}

ConnectionContext::ConnectionContext(ConnectionSocket* connectionSocket) :
    m_connectionSocket(connectionSocket),
    m_reliableResendTime(),
    m_outgoingSequenceIndex(0),
    m_incomingSequenceIndex(0),
    m_outgoingReliableIndex(0),
    m_immediateIncomingReliableIndex(0),
    m_deferredIncomingReliableIndex(0),
    m_remoteAcknowledgmentIndex(0),
    m_sendAcknowledgment(false),
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

    return this->pushOutgoing_NoLock(packetEntry);
}

bool ConnectionContext::pushOutgoing_NoLock(const PacketEntry & packetEntry)
{
    // Verify packet entry.
    if(packetEntry.header.transportMethod == (uint32_t)TransformMethod::Reliable)
    {
        if(!m_supportsReliability)
        {
            ASSERT(false, "Socket must support reliability to send reliable packets!");
            return false;
        }
    }

    ASSERT(packetEntry.header.sequenceIndex == 0, "Sequence index field should have been left untouched!");
    ASSERT(packetEntry.header.previousReliableIndex == 0, "Previous reliable index field should have been left untouched!");
    ASSERT(packetEntry.header.acknowledgmentIndex == 0, "Acknowedlgment index field should have been left untouched!");
    ASSERT(packetEntry.header.transportExtra == 0, "Transport extra field should have been left untouched!");

    // Add outgoing packet to queue.
    PacketEntry& outgoingPacket = m_outgoingQueue.emplace(packetEntry);

    // Fill packet sequence.
    outgoingPacket.header.sequenceIndex = ++m_outgoingSequenceIndex;

    // Handle reliability.
    if(m_supportsReliability)
    {
        // Fill previous reliable sequence index that was sent.
        // This will be checked on remote side to ensure that reliable messages are ordered.
        outgoingPacket.header.previousReliableIndex = m_outgoingReliableIndex;

        if(outgoingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable)
        {
            m_outgoingReliableIndex = outgoingPacket.header.sequenceIndex;
        }

        // Debug trace.
        LOG_CONNECTION_CONTEXT_TRACE("Pushing outgoing %s packet. (sequence %u, previousReliable %u)",
            outgoingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable ? "reliable" : "unreliable",
            outgoingPacket.header.sequenceIndex, outgoingPacket.header.previousReliableIndex);
    }
    
    // Packet has been added successfully to queue.
    return true;
}

bool ConnectionContext::popOutgoing(PacketEntry* packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if outgoing queue is empty.
    if(m_outgoingQueue.empty())
    {
        // Check if need to send acknowledgment.
        if(m_supportsReliability && m_sendAcknowledgment)
        {
            LOG_CONNECTION_CONTEXT_TRACE("Pushing acknowledgment packet.", m_incomingSequenceIndex);

            // Push empty outgoing acknowledgment packet.
            PacketEntry acknowledgmentPacket;
            acknowledgmentPacket.header.transportMethod = (uint32_t)TransformMethod::Unreliable;
            acknowledgmentPacket.address = m_connectionSocket->getRemoteAddress();
            acknowledgmentPacket.port = m_connectionSocket->getRemotePort();

            // Push outgoing packet and continue this method.
            this->pushOutgoing_NoLock(acknowledgmentPacket);
            ASSERT(!m_outgoingQueue.empty());
        }
        else
        {
            // There are no packets that need to be sent.
            return false;
        }
    }

    // Get next outgoing packet.
    PacketEntry& outgoingPacket = m_outgoingQueue.front();

    // Handle reliability protocol.
    if(m_supportsReliability)
    {
        // Send last processed sequence index received and processed from remote.
        outgoingPacket.header.acknowledgmentIndex = determineAcknowledgmentIndex();
    
        // Acknowledgment already included in this packet.
        m_sendAcknowledgment = false;

        // Add reliable packet to reliability queue.
        if(outgoingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable)
        {
            this->pushReliable_NoLock(outgoingPacket);
        }

        // Debug trace.
        LOG_CONNECTION_CONTEXT_TRACE("Popping outgoing %s packet. (sequence %u, previousReliable %u, acknowledgment %u)",
            outgoingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable ? "reliable" : "unreliable",
            outgoingPacket.header.sequenceIndex, outgoingPacket.header.previousReliableIndex, outgoingPacket.header.acknowledgmentIndex);
    }

    // Copy packet entry.
    if(packetEntry != nullptr)
    {
        *packetEntry = outgoingPacket;
    }

    // Pop read packet.
    m_outgoingQueue.pop();

    // Successfully popped outgoing packet.
    return true;
}

bool ConnectionContext::pushIncoming(const PacketEntry& packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Verify received reliable packet.
    if(m_supportsReliability)
    {
        if(packetEntry.header.transportMethod == (uint32_t)TransformMethod::Reliable)
        {
            // Verify that incoming packet arrived from expected source.
            ASSERT(packetEntry.address == m_connectionSocket->getRemoteAddress());
            ASSERT(packetEntry.port == m_connectionSocket->getRemotePort());
        }

        // Update acknowledgment index.
        m_remoteAcknowledgmentIndex = std::max(m_remoteAcknowledgmentIndex, packetEntry.header.acknowledgmentIndex);

        // Try to acknowledge received reliable packet early if it was received in expected order.
        if(packetEntry.header.previousReliableIndex == m_immediateIncomingReliableIndex)
        {
            if(packetEntry.header.transportMethod == (uint32_t)TransformMethod::Reliable)
            {
                LOG_CONNECTION_CONTEXT_TRACE("Early acknowledgement of incoming reliable packet. (previousReliable %u == immediateIncomingReliable %u, sequenceIndex %u)",
                    packetEntry.header.previousReliableIndex, m_immediateIncomingReliableIndex, packetEntry.header.sequenceIndex);

                // Update immediate incoming reliable index.
                ASSERT(m_immediateIncomingReliableIndex < packetEntry.header.sequenceIndex,
                    "We are about to set lower immediate incoming reliable index than the current one!");

                m_immediateIncomingReliableIndex = packetEntry.header.sequenceIndex;

                // Send acknowledgment as soon as possible.
                m_sendAcknowledgment = true;
            }
        }

        // Drop incoming packets that do not have any data (e.g. acknowledgment packets).
        if(packetEntry.packet.size() == 0)
        {
            LOG_CONNECTION_CONTEXT_TRACE("Received incoming acknowledgment packet without data. (acknowledgment %u)", packetEntry.header.acknowledgmentIndex);
            return false;
        }

        // Debug trace.
        LOG_CONNECTION_CONTEXT_TRACE("Pushing incoming %s packet. (sequence %u, previousReliable %u, acknowledgment %u)",
            packetEntry.header.transportMethod == (uint32_t)TransformMethod::Reliable ? "reliable" : "unreliable",
            packetEntry.header.sequenceIndex, packetEntry.header.previousReliableIndex, packetEntry.header.acknowledgmentIndex);
    }

    // Add packet to incoming queue.
    m_incomingQueue.emplace(packetEntry);

    // Successfully pushed incoming packet.
    return true;
}

bool ConnectionContext::popIncoming(PacketEntry* packetEntry)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Get first usable incoming packet.
    while(!m_incomingQueue.empty())
    {
        // Get next incoming packet from queue.
        PacketEntry incomingPacket = m_incomingQueue.top();

        // Verify popped packets in terms of reliability.
        if(m_supportsReliability)
        {
            // Drop packets that are already acknowledged or obsolete.
            if(incomingPacket.header.sequenceIndex <= m_incomingSequenceIndex)
            {
                if(incomingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable)
                {
                    LOG_CONNECTION_CONTEXT_TRACE("Dropping already processed reliable incoming packet! (sequence %u <= incomingSequence %u)",
                        incomingPacket.header.sequenceIndex, m_incomingSequenceIndex);
                }
                else
                {
                    LOG_CONNECTION_CONTEXT_TRACE("Dropping obsolete unreliable incoming packet! (sequence %u <= incomingSequence %u)",
                        incomingPacket.header.sequenceIndex, m_incomingSequenceIndex);
                }

                if(incomingPacket.header.sequenceIndex == m_incomingSequenceIndex)
                {
                    LOG_CONNECTION_CONTEXT_TRACE("Requesting another acknowledgment for reliable incoming packet! (sequence %u == incomingSequence %u)",
                        incomingPacket.header.sequenceIndex, m_incomingSequenceIndex);

                    // Request new acknowledgment packet to be sent.
                    // We only want to resend acknowledgment for the last resent incoming packet.
                    m_sendAcknowledgment = true;
                }

                // Drop packet.
                m_incomingQueue.pop();
                continue;
            }

            // Check if we are missing a reliable packet in our sequence.
            if(incomingPacket.header.previousReliableIndex != m_deferredIncomingReliableIndex)
            {
                LOG_CONNECTION_CONTEXT_TRACE("Missing previous reliable incoming packet! (previousReliable %u != deferredIncomingReliable %u)",
                    incomingPacket.header.previousReliableIndex, m_deferredIncomingReliableIndex);

                // Do not drop packet and wait until needed reliable packet arrives.
                return false;
            }
            else
            {
                // Update reliable index.
                if(incomingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable)
                {
                    m_deferredIncomingReliableIndex = incomingPacket.header.sequenceIndex;
                }
            }

            // Update sequence index.
            m_incomingSequenceIndex = incomingPacket.header.sequenceIndex;

            // Send acknowledgment as soon as possible.
            m_sendAcknowledgment = true;

            // Debug trace.
            LOG_CONNECTION_CONTEXT_TRACE("Popping incoming %s packet. (sequence %u, previousReliable %u, acknowledgment %u)",
                incomingPacket.header.transportMethod == (uint32_t)TransformMethod::Reliable ? "reliable" : "unreliable",
                incomingPacket.header.sequenceIndex, incomingPacket.header.previousReliableIndex, incomingPacket.header.acknowledgmentIndex);
        }

        // Retrieve packet from incoming queue.
        if(packetEntry != nullptr)
        {
            *packetEntry = incomingPacket;
        }

        // Pop read packet from queue.
        m_incomingQueue.pop();

        // Packet has been successfully read and popped.
        return true;
    }

    // Ensure that immediate and deferred incoming reliable indices match.
    ASSERT(m_deferredIncomingReliableIndex == m_immediateIncomingReliableIndex,
        "Mismatch of immediate and deferred incoming reliable indices!");

    // We did not succeed in getting any usable packet.
    return false;
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
    ASSERT(packetEntry.header.transportMethod == (uint32_t)TransformMethod::Reliable, "Packet entry must be reliable!");

    // Schedule reliable resend on new reliable packet after queue has been emptied.
    if(m_reliableQueue.empty())
    {
        scheduleReliableResend_NoLock();
    }

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

    // Check if it is time to resend reliable packages.
    // Do not proceed with pushing unacknowledged packets if we are still waiting for the right time.
    auto currentTime = std::chrono::high_resolution_clock::now();
    if(currentTime < m_reliableResendTime)
        return;

    // Copy unacknowledged packets to provided queue.
    uint32_t packetsPushed = 0;

    for(auto& packetEntry : m_reliableQueue)
    {
        // Update acknowledgment index.
        packetEntry.header.acknowledgmentIndex = determineAcknowledgmentIndex();

        // Push packet entry copy to provided queue.
        LOG_CONNECTION_CONTEXT_TRACE("Pushing unacknowledged reliable packet. (sequence %u, previousReliable %u, acknowledgment %u)",
            packetEntry.header.sequenceIndex, packetEntry.header.previousReliableIndex, packetEntry.header.acknowledgmentIndex);

        packetQueue.push(packetEntry);
        packetsPushed++;

        // Hard limit of packets resent at once to prevent congestion.
        // This value should be dynamic depending on network conditions.
        if(packetsPushed >= ConnectionSettings::MaxReliableResendCount)
            break;
    }

    // Schedule next reliable packet resend.
    if(!m_reliableQueue.empty())
    {
        scheduleReliableResend_NoLock();
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

        if(reliableEntry.header.sequenceIndex > m_remoteAcknowledgmentIndex)
        {
            // Packet has not been acknowledged yet.
            // Remaining packets are not acknowledged either.
            return;
        }

        LOG_CONNECTION_CONTEXT_TRACE("Popping acknowledged reliable packet. (sequence %u > acknowledgment %u)",
            reliableEntry.header.sequenceIndex, m_remoteAcknowledgmentIndex);

        // Pop acknowledged packet from queue.
        m_reliableQueue.pop_front();
    }

    // Stop scheduled reliable resent if queue has been emptied.
    if(m_reliableQueue.empty())
    {
        m_reliableResendTime = std::chrono::high_resolution_clock::time_point();
    }
}

void ConnectionContext::scheduleReliableResend_NoLock()
{
    // Get current high performance clock time.
    auto currentTime = std::chrono::high_resolution_clock::now();
    ASSERT(m_reliableResendTime < currentTime, "Reliable packet resend is already scheduled!");
    
    // Calculate reliable packet resend using logarithmic scale.
    float reliableQueueRatio = std::clamp((float)m_reliableQueue.size() / ConnectionSettings::HighReliableResendCount, 0.0f, 1.0f);
   
    float minimumCoefficient = std::pow(ConnectionSettings::MinimumReliableResendDelayMs, 1.0f - reliableQueueRatio);
    float maximumCoefficient = std::pow(ConnectionSettings::MaximumReliableResendDelayMs, reliableQueueRatio);
    float calculatedWaitTime = minimumCoefficient * maximumCoefficient;

    ASSERT(calculatedWaitTime >= ConnectionSettings::MinimumReliableResendDelayMs);
    ASSERT(calculatedWaitTime <= ConnectionSettings::MaximumReliableResendDelayMs);

    // Set time at which we will perform next reliable packet resend.
    m_reliableResendTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds((uint32_t)(calculatedWaitTime));
}

uint32_t ConnectionContext::determineAcknowledgmentIndex() const
{
    // This can either be most recent incoming packet popped or most recent incoming packet pushed.
    return std::max(m_incomingSequenceIndex, m_immediateIncomingReliableIndex);
}

bool ConnectionContext::supportsReliability() const
{
    return m_supportsReliability;
}
