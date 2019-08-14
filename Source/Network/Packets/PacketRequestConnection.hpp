#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"

class PacketRequestConnection : public PacketBase
{
    TYPE_DECLARE(PacketRequestConnection, PacketBase);

public:
    PacketRequestConnection();
    ~PacketRequestConnection();
};
