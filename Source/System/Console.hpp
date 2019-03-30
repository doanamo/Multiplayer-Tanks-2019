#pragma once

#include "Precompiled.hpp"

class ConsoleEntry
{
protected:
    ConsoleEntry(std::string name);
    virtual ~ConsoleEntry();

public:
    // Calls console entry.
    virtual void call(std::string arguments)
    {
        LOG_ERROR("Console entry \"%s\" cannot have be called!", m_name.c_str());
    }

    // Sets console entry value.
    virtual void write(std::string arguments)
    {
        LOG_ERROR("Console entry \"%s\" cannot have its variable written!", m_name.c_str());
    }

    // Reads console entry value.
    virtual void read() const
    {
        LOG_ERROR("Console entry \"%s\" cannot have its variable read!", m_name.c_str());
    }

    // Gets console entry name.
    const std::string& getName() const
    {
        return m_name;
    }

private:
    // Console entry name.
    std::string m_name;
};

template<typename Type>
class ConsoleVariable;

template<>
class ConsoleVariable<bool> : public ConsoleEntry
{
public:
    ConsoleVariable(std::string name, bool value) :
        ConsoleEntry(name), value(value)
    {
    }

    void write(std::string arguments) override
    {
        if(arguments == "1" || arguments == "true")
        {
            value = true;
        }
        else if(arguments == "0" || arguments == "false")
        {
            value = false;
        }
        else
        {
            LOG_WARNING("Invalid arguments when setting bool console variable!");
        }
    }

    void read() const override
    {
        LOG_INFO("%s = %s", getName().c_str(), value ? "true" : "false");
    }

public:
    bool value;
};

template<>
class ConsoleVariable<std::string> : public ConsoleEntry
{
public:
    ConsoleVariable(std::string name, std::string value) :
        ConsoleEntry(name), value(value)
    {
    }

    void write(std::string arguments) override
    {
        value = arguments;
    }

    void read() const override
    {
        LOG_INFO("%s = %s", getName().c_str(), value.c_str());
    }

public:
    std::string value;
};

class ConsoleFunction : public ConsoleEntry
{
public:
    ConsoleFunction(std::string name, void(*function)(std::string)) :
        ConsoleEntry(name), function(function)
    {
    }

    void call(std::string arguments) override
    {
        if(function != nullptr)
        {
            function(arguments);
        }
        else
        {
            LOG_WARNING("Could not call console function because it is null!");
        }
    }

public:
    void (*function)(std::string);
};

class ConsoleRegistry
{
    // Type declarations.
    using EntryMap = std::map<std::string, ConsoleEntry&>;

public:
    // Returns registry singleton.
    static ConsoleRegistry& getSingleton();

    // Registers console entry.
    void registerEntry(std::string name, ConsoleEntry& entry);

    // Unregisters console entry.
    void unregisterEntry(std::string name, ConsoleEntry& entry);

    // Finds entry by name.
    ConsoleEntry* findEntry(std::string name);

private:
    // List of console entries.
    EntryMap m_entries;
};

class Console
{
public:
    Console();
    ~Console();

    // Initializes the console.
    bool initialize();

    // Parse console input.
    void parseInput(std::string inputString, bool ignoreErrors = false);

    // Displays console window if "showConsole" variable is true.
    void display();

private:
    // Console input buffer.
    std::vector<char> m_consoleInput;
};
