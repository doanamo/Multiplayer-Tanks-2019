#pragma once

#include "Precompiled.hpp"

class MemoryBuffer
{
public:
    MemoryBuffer();
    ~MemoryBuffer();

    // Writing methods.
    void writeByte(uint8_t* value);
    void writeShort(uint16_t* value);
    void writeInteger(uint32_t* value);
    void writeWord(uint64_t* value);

    // Reading methods.
    bool readByte(uint8_t* value);
    bool readShort(uint16_t* value);
    bool readInteger(uint32_t* value);
    bool readWord(uint64_t* value);

private:
    // Resizable stream buffer.
    std::vector<char> m_buffer;

    // Next byte to read or write.
    std::size_t m_index;
};
