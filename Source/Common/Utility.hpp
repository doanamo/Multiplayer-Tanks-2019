#pragma once

std::string formatString(const char* format, va_list arguments);
std::string lowerCaseString(const std::string& string);
std::string trimLeadingString(const std::string& string, std::string characters);
std::string trimTrailingString(const std::string& string, std::string characters);
