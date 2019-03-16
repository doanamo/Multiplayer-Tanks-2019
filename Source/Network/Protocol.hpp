#pragma once

enum class PacketType
{
    Invalid,

    // Connection
    Client_RequestConnection,
    Server_NegotiateConnection,
    Client_AcceptConnection,
    Network_Ping,

    // Game
    Server_StateUpdate,
    Client_PlayerInput,
};
