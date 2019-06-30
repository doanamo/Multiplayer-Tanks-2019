#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"

class PacketConnect : public PacketBase
{
    TYPE_DECLARE(PacketConnect, PacketBase);

public:
    PacketConnect();
    ~PacketConnect();
};
