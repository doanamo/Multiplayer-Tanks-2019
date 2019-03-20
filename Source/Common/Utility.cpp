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
