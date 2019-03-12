#pragma once

class MemoryBuffer;

class Serializable
{
public:
    virtual bool serialize(MemoryBuffer& stream) = 0;
    virtual bool deserialize(MemoryBuffer& stream) = 0;
};
