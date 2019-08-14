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

public:

};
