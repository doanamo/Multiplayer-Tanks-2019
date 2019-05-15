#pragma once

class MemoryStream;

class Serializable
{
public:
    virtual bool onSerialize(MemoryStream& buffer) const = 0;
    virtual bool onDeserialize(MemoryStream& buffer) = 0;
};
