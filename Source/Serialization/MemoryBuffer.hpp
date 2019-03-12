#include "Precompiled.hpp"

class MemoryBuffer
{
public:
    MemoryBuffer();
    ~MemoryBuffer();

    // Writing methods.
    void writeByte(uint8_t value);
    void writeShort(uint16_t value);
    void writeInteger(uint32_t value);
    void writeWord(uint64_t value);
    void writeFloat(float value);
    void writeDouble(double value);

    // Reading methods.
    uint8_t readByte();
    uint16_t readShort();
    uint32_t readInteger();
    uint64_t readWord();
    float readFloat();
    double readDouble();

private:
    // Resizable stream buffer.
    std::vector<char> m_buffer;

    // Next byte to read or write.
    std::size_t m_index;
};
