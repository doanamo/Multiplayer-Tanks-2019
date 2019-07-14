#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

class GameInstance;

class PacketServerSnapshot : public PacketBase
{
    TYPE_DECLARE(PacketServerSnapshot, PacketBase);

public:
    PacketServerSnapshot();
    ~PacketServerSnapshot();

    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

public:
    MemoryStream serializedGameInstance;
};
