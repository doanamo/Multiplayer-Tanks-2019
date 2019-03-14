#pragma once

class MemoryBuffer;

class Serializable
{
public:
    virtual bool onSerialize(MemoryBuffer& buffer) = 0;
    virtual bool onDeserialize(MemoryBuffer& buffer) = 0;
};
