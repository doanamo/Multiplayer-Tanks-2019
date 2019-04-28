#pragma once

#include "Precompiled.hpp"

class PacketBase : public Serializable
{
    TYPE_DECLARE(PacketBase);

protected:
    PacketBase();

public:
    ~PacketBase();

    bool onSerialize(MemoryBuffer& buffer) override;
    bool onDeserialize(MemoryBuffer& buffer) override;
};
