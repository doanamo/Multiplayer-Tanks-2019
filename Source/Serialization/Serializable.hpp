#pragma once

class MemoryBuffer;

class Serializable
{
public:
    virtual bool onSerialize(MemoryBuffer& stream) = 0;
    virtual bool onDeserialize(MemoryBuffer& stream) = 0;
};
