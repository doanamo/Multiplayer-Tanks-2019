#pragma once

#include "Precompiled.hpp"
#include "Network/PacketBase.hpp"

class PacketMessage : public PacketBase
{
    TYPE_DECLARE(PacketMessage, PacketBase);

public:
    PacketMessage();
    ~PacketMessage();

    bool onSerialize(MemoryBuffer& buffer) const override;
    bool onDeserialize(MemoryBuffer& buffer) override;

public:
    std::string text;
};
