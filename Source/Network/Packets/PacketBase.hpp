#pragma once

#include "Precompiled.hpp"

class PacketBase : public Serializable
{
    TYPE_DECLARE(PacketBase);

protected:
    PacketBase();

public:
    virtual ~PacketBase();

    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;
};
