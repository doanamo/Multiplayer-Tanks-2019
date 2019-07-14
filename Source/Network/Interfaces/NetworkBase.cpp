#include "Precompiled.hpp"
#include "NetworkBase.hpp"
#include "Network/Replication/Replication.hpp"
#include "Network/Packets/PacketHeader.hpp"
#include "Game/GameInstance.hpp"

ConsoleVariable<bool> cv_showNetwork("showNetwork", false);

NetworkBase::NetworkBase()
{
}

NetworkBase::~NetworkBase()
{
    LOG_TRACE("NetworkBase::~NetworkBase()");
}

bool NetworkBase::initialize(GameInstance* gameInstance)
{
    // Save game instance reference.
    if(gameInstance == nullptr)
    {
        LOG_ERROR("Game instance is needed to initialize network interface!");
        return false;
    }

    m_gameInstance = gameInstance;

    // Initialize replication system.
    m_replication = std::make_unique<Replication>();
    if(!m_replication->initialize(gameInstance))
        return false;
    
    // Success!
    return true;
}

void NetworkBase::update(float timeDelta)
{
}

void NetworkBase::tick(float timeDelta)
{
}

void NetworkBase::draw()
{
    // Draw replication debug.
    m_replication->draw();
}

bool NetworkBase::sendPacket(ConnectionSocket& socket, PacketBase& packet, bool reliable, const sf::IpAddress* address, const unsigned short* port)
{
    // Serialize packet along with its type.
    MemoryStream packetData;

    if(!serialize(packetData, getTypeIdentifier(packet)))
    {
        LOG_ERROR("Could not serialize packet type!");
        return false;
    }

    if(!serialize(packetData, packet))
    {
        LOG_ERROR("Could not serialize packet data!");
        return false;
    }

    // Send packet over network.
    if(!socket.send(packetData, reliable, address, port))
    {
        LOG_ERROR("Could not send packet over network!");
        return false;
    }

    // Success!
    return true;
}

bool NetworkBase::receivePacket(ConnectionSocket& socket, std::unique_ptr<PacketBase>& packet, bool* reliable, sf::IpAddress* address, unsigned short* port)
{
    // Receive packet data.
    MemoryStream packetData;
    if(!socket.receive(packetData, reliable, address, port))
        return false;

    // Deserialize packet.
    if(!readPacket(packetData, packet))
        return false;

    // Success!
    return true;
}

bool NetworkBase::readPacket(MemoryStream& packetData, std::unique_ptr<PacketBase>& packet)
{
    // Deserialize packet type.
    TypeInfo::IdentifierType packetType = 0;
    if(!deserialize(packetData, &packetType))
    {
        LOG_ERROR("Could not deserialize received packet type!");
        return false;
    }

    // Create packet instance.
    std::unique_ptr<PacketBase> receivedPacket(PacketBase::create(packetType));
    if(receivedPacket == nullptr)
    {
        LOG_ERROR("Could not instantiate received packet type!");
        return false;
    }

    // Deserialize packet data.
    if(!deserialize(packetData, receivedPacket.get()))
    {
        LOG_ERROR("Could not deserialize received packet data!");
        return false;
    }

    // Move packet to provided unique pointer.
    packet = std::move(receivedPacket);

    // Success!
    return true;
}

Replication* NetworkBase::getReplication()
{
    return m_replication.get();
}
