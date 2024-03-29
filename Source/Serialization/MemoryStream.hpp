#pragma once

#include "Precompiled.hpp"
#include "Serialization/Serializable.hpp"

class MemoryStream : public Serializable
{
public:
    MemoryStream();
    MemoryStream(std::size_t size);
    ~MemoryStream();

    // Writing methods.
    bool writeByte(uint8_t* value);
    bool writeShort(uint16_t* value);
    bool writeInteger(uint32_t* value);
    bool writeWord(uint64_t* value);
    bool writeData(const char* data, std::size_t size);

    // Reading methods.
    bool readByte(uint8_t* value);
    bool readShort(uint16_t* value);
    bool readInteger(uint32_t* value);
    bool readWord(uint64_t* value);
    bool readData(char* data, std::size_t size);

    // Buffer manipulation.
    void clear();
    void reset();
    void resize(std::size_t size);
    void replace(const char* data, std::size_t size);

    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

    // Accessors.
    bool empty() const;
    std::size_t index() const;
    std::size_t size() const;
    const char* data() const;
    char* data();

private:
    // Resizable stream buffer.
    std::vector<char> m_buffer;

    // Next byte to read or write.
    std::size_t m_index;
};
