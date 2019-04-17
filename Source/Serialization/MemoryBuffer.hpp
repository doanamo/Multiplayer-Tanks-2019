#pragma once

#include "Precompiled.hpp"

class MemoryBuffer
{
public:
    MemoryBuffer();
    ~MemoryBuffer();

    // Writing methods.
    bool writeByte(uint8_t* value);
    bool writeShort(uint16_t* value);
    bool writeInteger(uint32_t* value);
    bool writeWord(uint64_t* value);

    // Reading methods.
    bool readByte(uint8_t* value);
    bool readShort(uint16_t* value);
    bool readInteger(uint32_t* value);
    bool readWord(uint64_t* value);

    // Buffer manipulation.
    void clear();
    void reset();
    void resize(std::size_t size);
    void copy(const char* data, std::size_t size);

    // Accessors.
    std::size_t size() const;
    const char* data() const;
    char* data();

private:
    // Resizable stream buffer.
    std::vector<char> m_buffer;

    // Next byte to read or write.
    std::size_t m_index;
};
