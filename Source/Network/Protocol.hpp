#pragma once

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
};

struct PacketHeader
{
    PacketHeader();

    char magic[8];
    PacketType type;
};

bool serialize(MemoryBuffer& buffer, const PacketHeader& header);
bool deserialize(MemoryBuffer& buffer, PacketHeader* header);
