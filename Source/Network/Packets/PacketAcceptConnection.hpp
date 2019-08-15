#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"

// Accept connection packet class.
class PacketAcceptConnection : public PacketBase
{
    TYPE_DECLARE(PacketAcceptConnection, PacketBase);

public:
    PacketAcceptConnection();
    ~PacketAcceptConnection();

    bool onSerialize(MemoryStream& buffer) const;
    bool onDeserialize(MemoryStream& buffer);

public:
    int playerIndex;
};
