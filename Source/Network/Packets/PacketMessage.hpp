#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

class PacketMessage : public PacketBase
{
    TYPE_DECLARE(PacketMessage, PacketBase);

public:
    PacketMessage();
    ~PacketMessage();

    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

public:
    std::string text;
};
