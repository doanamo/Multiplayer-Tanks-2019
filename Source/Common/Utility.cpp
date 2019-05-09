#include "Precompiled.hpp"
#include "Common/Utility.hpp"

std::string formatString(const char* format, va_list arguments)
{
    // Attempt to format string using static buffer.
    const int BufferSize = 256;
    char staticBuffer[BufferSize] = { 0 };

    int characters = vsnprintf(&staticBuffer[0], BufferSize, format, arguments);

    if(characters < (int)BufferSize)
    {
        if(characters > 0)
        {
            return std::string(&staticBuffer[0], characters);
        }
        else
        {
            return std::string("String formatting error!");
        }
    }

    // Use dynamic buffer if static buffer is too small.
    std::vector<char> dynamicBuffer;
    dynamicBuffer.resize(characters + 1, 0);

    vsnprintf(&dynamicBuffer[0], dynamicBuffer.size(), format, arguments);
    return std::string(&dynamicBuffer[0], dynamicBuffer.size());
}

std::string lowerCaseString(const std::string& string)
{
    std::string result;

    for(char character : string)
    {
        result.push_back(std::tolower(character));
    }

    return result;
}

std::string trimLeadingString(const std::string& string, std::string characters)
{
    const auto it = string.find_first_not_of(characters);

    if(it != std::string::npos)
    {
        return string.substr(it);
    }
    else
    {
        return "";
    }
}

std::string trimTrailingString(const std::string& string, std::string characters)
{
    const auto it = string.find_last_not_of(characters);

    if(it != std::string::npos)
    {
        return string.substr(0, it + 1);
    }
    else
    {
        return "";
    }
}

uint32_t stringHash(const std::string& string)
{
    // Small and simple hashing function for strings.
    // May need to be replaced in case of collisions.
    // Use only if you can detect possible collisions.
    // It is not cryptographically secure.
    const unsigned int prime = 257;
    unsigned int hash = 0;

    for(char c : string)
    {
        hash = hash * prime + c;
    }

    return hash;
}

uint32_t calculateCRC32(uint32_t crc, const char* data, std::size_t size)
{
    // Implementation based on https://stackoverflow.com/a/27950866

    crc = ~crc;

    while(size--)
    {
        crc ^= (unsigned char)*data++;

        for(int i = 0; i < 8; ++i)
        {
            crc = crc & 1 ? (crc >> 1) ^ 0x82f63b78 : crc >> 1;
        }
    }

    return ~crc;
}

bool parseStringToPort(std::string text, unsigned short& port)
{
    // Determine listen port.
    int portNumber = -1;

    if(!text.empty())
    {
        portNumber = std::stoi(text);
    }

    // Check listen port.
    if(portNumber < 0 || portNumber > std::numeric_limits<unsigned short>::max())
    {
        LOG_ERROR("Requested listen port number \"%s\" is outside of valid range!", text.c_str());
        return false;
    }

    port = (unsigned short)portNumber;

    return true;
}
