#pragma once

class CommandLine
{
public:
    CommandLine();
    ~CommandLine();

    // Initializes command line.
    bool initialize(int argc, char* argv[]);

    // Tries to parse arguments as console variables.
    void parseConsoleVariables();

    // Checks if specified argument is present.
    bool hasArgument(std::string argument);
};
