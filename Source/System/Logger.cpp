#include "Precompiled.hpp"
#include "System/Logger.hpp"

#ifdef WIN32
    #include <Windows.h>
#endif

Logger::Logger() :
    m_nextIndent(0)
{
}

Logger::~Logger()
{
}

bool Logger::initialize()
{
    // Open file log output.
    m_logFile.open(OutputLogFile, std::ios::trunc);
    if(!m_logFile.is_open()) return false;

    return true;
}

void Logger::setNextIndent(int indent)
{
    m_nextIndent = indent;
}

void Logger::write(LogType type, const char* format, ...)
{
    assert(format != nullptr);

    // Truncate message history.
    if(m_messages.size() == MaxMessageCount)
    {
        m_messages.pop();
    }

    // Retrieve the current system time.
    time_t timeData = time(nullptr);
    tm* timeInfo = localtime(&timeData);

    // Print current date and time.
    std::stringstream stream;
    stream << "[";
    stream << std::setfill('0');
    stream << std::setw(4)      << timeInfo->tm_year + 1900 << "-";
    stream << std::setw(2)      << timeInfo->tm_mon + 1 << "-";
    stream << std::setw(2)      << timeInfo->tm_mday << " ";
    stream << std::setw(2)      << timeInfo->tm_hour << ":";
    stream << std::setw(2)      << timeInfo->tm_min << ":";
    stream << std::setw(2)      << timeInfo->tm_sec;
    stream << std::setfill(' ') << std::setw(0);
    stream << "]";

    // Print log message type.
    switch(type)
    {
    case LogType::Trace:
        stream << "[~]";
        break;

    case LogType::Info:
        stream << "[-]";
        break;

    case LogType::Warning:
        stream << "[?]";
        break;

    case LogType::Error:
        stream << "[!]";
        break;

    case LogType::Fatal:
        stream << "[*]";
        break;

    default:
        assert(false && "Unknown log type!");
    }

    // Print current indent.
    for(int i = 0; i <= m_nextIndent; ++i)
    {
        stream << " ";
    }

    m_nextIndent = 0;

    // Print message text.
    va_list arguments;
    va_start(arguments, format);
    stream << formatString(format, arguments);
    va_end(arguments);

    // Print to console.
    std::cout << stream.str() << std::endl;
    
    // Print to file.
    m_logFile << stream.str() << std::endl;

    // Print to debugger.
#ifdef WIN32
    OutputDebugString(stream.str().c_str());
#endif

    // Add new message.
    Message message;
    message.type = type;
    message.text = stream.str();

    m_messages.emplace(std::move(message));
}
