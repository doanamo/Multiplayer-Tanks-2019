#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

class GameInstance;

class PacketStateSave : public PacketBase
{
    TYPE_DECLARE(PacketStateSave, PacketBase);

public:
    PacketStateSave();
    ~PacketStateSave();

    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

public:
    MemoryStream serializedGameInstance;
};
