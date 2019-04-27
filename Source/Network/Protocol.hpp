#pragma once

// Packet header.
class PacketHeader : public Serializable
{
public:
    // Type declarations.
    using PacketType = TypeInfo::IdentifierType;

public:
    PacketHeader();
    ~PacketHeader();

    // Serialization for packet type.
    bool onSerialize(MemoryBuffer& buffer);
    bool onDeserialize(MemoryBuffer& buffer);

public:
    // todo: add crc
    PacketType type;
};

// Base packet.
class PacketBase : public Serializable
{
    TYPE_DECLARE(PacketBase);

protected:
    PacketBase();

public:
    ~PacketBase();

    bool onSerialize(MemoryBuffer& buffer);
    bool onDeserialize(MemoryBuffer& buffer);
};

// Message packet.
class PacketMessage : public PacketBase
{
    TYPE_DECLARE(PacketMessage, PacketBase);

public:
    PacketMessage();
    ~PacketMessage();

    bool onSerialize(MemoryBuffer& buffer);
    bool onDeserialize(MemoryBuffer& buffer);

public:
    std::string text;
};

/*
enum class PacketType
{
    Invalid,

    // Connection
    Client_RequestConnection,
    Server_NegotiateConnection,
    Client_AcceptConnection,
    Server_StopConnection,
    Client_StopConnection,

    // Game
    Network_Heartbeat,
    Server_StateUpdate,
    Client_PlayerInput,
}
*/
