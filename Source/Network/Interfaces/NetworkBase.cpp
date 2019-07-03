#include "Precompiled.hpp"
#include "NetworkBase.hpp"
#include "Network/Packets/PacketHeader.hpp"
#include "Game/GameInstance.hpp"

ConsoleVariable<bool> cv_showNetwork("showNetwork", false);

NetworkBase::NetworkBase() :
    m_socket(),
    m_gameInstance(nullptr)
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
