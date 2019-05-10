#pragma once

class MemoryBuffer;

class Serializable
{
public:
    virtual bool onSerialize(MemoryBuffer& buffer) const = 0;
    virtual bool onDeserialize(MemoryBuffer& buffer) = 0;
};
