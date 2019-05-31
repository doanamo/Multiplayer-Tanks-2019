#pragma once

#include "Precompiled.hpp"

struct Handle
{
    Handle(int identifier = -1) :
        identifier(identifier),
        version(0)
    {
    }

    bool operator==(const Handle& other)
    {
        return identifier == other.identifier && version == other.version;
    }

    bool operator!=(const Handle& other)
    {
        return identifier != other.identifier || version != other.version;
    }

    int identifier;
    int version;
};
