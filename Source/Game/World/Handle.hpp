#pragma once

#include "Precompiled.hpp"

struct Handle
{
    using ValueType = int;

    Handle(ValueType identifier = 0) :
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

    ValueType identifier;
    ValueType version;
};
