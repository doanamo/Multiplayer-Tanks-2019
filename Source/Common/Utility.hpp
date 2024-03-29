#pragma once

template<typename Type, std::size_t Size>
constexpr std::size_t staticArraySize(const Type(&)[Size])
{
    return Size;
}

std::string formatString(const char* format, va_list arguments);
std::string lowerCaseString(const std::string& string);
std::string trimLeadingString(const std::string& string, std::string characters);
std::string trimTrailingString(const std::string& string, std::string characters);
unsigned int stringHash(const std::string& string);

uint32_t calculateCRC32(uint32_t crc, const char* data, std::size_t size);
bool parseStringToPort(std::string text, unsigned short& port);
