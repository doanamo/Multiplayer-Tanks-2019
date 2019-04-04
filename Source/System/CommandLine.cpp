#include "Precompiled.hpp"
#include "System/CommandLine.hpp"
#include "System/Console.hpp"
#include "System/Globals.hpp"

ConsoleVariable<std::string> cv_commandLine("commandLine", "");

CommandLine::CommandLine()
{
}

CommandLine::~CommandLine()
{
}

bool CommandLine::initialize(int argc, char* argv[])
{
    // Aggregate all command line arguments.
    cv_commandLine.value = std::string();

    for(int i = 1; i < argc; ++i)
    {
        if(i != 1)
        {
            cv_commandLine.value += " ";
        }

        cv_commandLine.value += argv[i];
    }

    // Log command line arguments.
    LOG_INFO("Command line arguments: %s", cv_commandLine.value.c_str());

    return true;
}

void CommandLine::parseConsoleVariables()
{
    ASSERT(g_console != nullptr, "Expected global console to not be null!");

    auto argumentBegin = std::find(cv_commandLine.value.begin(), cv_commandLine.value.end(), '-');

    while(argumentBegin != cv_commandLine.value.end())
    {
        auto argumentEnd = std::find(argumentBegin + 1, cv_commandLine.value.end(), '-');
        std::string argumentString(argumentBegin + 1, argumentEnd);

        if(!argumentString.empty())
        {
            g_console->parseInput(argumentString, true);
        }

        argumentBegin = argumentEnd;
    }
}

bool CommandLine::hasArgument(std::string argument)
{
    auto it = cv_commandLine.value.find("-" + argument);
    return it != std::string::npos;
}
