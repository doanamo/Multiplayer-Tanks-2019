#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

class GameInstance;

class PacketStateSnapshot : public PacketBase
{
    TYPE_DECLARE(PacketStateSnapshot, PacketBase);

public:
    PacketStateSnapshot();
    ~PacketStateSnapshot();

    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

public:
    MemoryStream serializedGameInstance;
};
