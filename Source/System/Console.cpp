#include "Precompiled.hpp"
#include "System/Console.hpp"
#include "System/Window.hpp"
#include "System/Globals.hpp"

ConsoleVariable<bool> cv_showConsole("showConsole", false);

namespace
{
    void logFunction(std::string arguments)
    {
        LOG_INFO("Log: %s", arguments.c_str());
    }
}

ConsoleFunction cf_log("log", &logFunction);

ConsoleEntry::ConsoleEntry(std::string name) :
    m_name(name)
{
    ConsoleRegistry::getSingleton().registerEntry(m_name, *this);
}

ConsoleEntry::~ConsoleEntry()
{
    ConsoleRegistry::getSingleton().unregisterEntry(m_name, *this);
}

ConsoleRegistry& ConsoleRegistry::getSingleton()
{
    static ConsoleRegistry singleton;
    return singleton;
}

void ConsoleRegistry::registerEntry(std::string name, ConsoleEntry& entry)
{
    // Check for invalid characters.
    bool containsNonLetters = std::find_if(name.begin(), name.end(), [](const char& c)
    {
        return std::isalpha(c) == 0;
    }) != name.end();

    if(containsNonLetters)
    {
        LOG_ERROR("Could not add \"%s\" console entry due to invalid characters!", name.c_str());
        return;
    }

    // Insert console entry.
    auto result = m_entries.emplace(lowerCaseString(name), entry);

    if(!result.second)
    {
        LOG_ERROR("Could not register \"%s\" console entry because this name is already in use!", name.c_str());
    }
}

void ConsoleRegistry::unregisterEntry(std::string name, ConsoleEntry& entry)
{
    auto it = m_entries.find(lowerCaseString(name));

    if(it != m_entries.end() && &it->second == &entry)
    {
        m_entries.erase(it);
    }
    else
    {
        LOG_ERROR("Could not find and unregister \"%s\" console entry!", name.c_str());
    }
}

ConsoleEntry* ConsoleRegistry::findEntry(std::string name)
{
    // Find and return console entry.
    auto it = m_entries.find(lowerCaseString(name));

    if(it != m_entries.end())
    {
        return &it->second;
    }
    else
    {
        return nullptr;
    }
}

Console::Console()
{
}

Console::~Console()
{
}

bool Console::initialize()
{
    ASSERT(g_logger != nullptr, "Logger must be initialized at this point!");

    // Test message colors.
    #if 0
        LOG_TRACE("Test console trace message!");
        LOG_INFO("Test console info message!");
        LOG_WARNING("Test console warning message!");
        LOG_ERROR("Test console error message!");
        LOG_FATAL("Test console fatal message!");
    #endif

    return true;
}

void Console::parseInput(std::string inputString, bool ignoreErrors)
{
    // Parse console input.
    auto entryNameEnd = std::find_if_not(inputString.begin(), inputString.end(),
        [](char c) { return std::isalpha(c) != 0; });

    if(entryNameEnd == inputString.begin())
    {
        if(!ignoreErrors)
        {
            LOG_ERROR("Could not parse console input for entry name!");
        }

        return;
    }

    // Get console entry by its name.
    std::string entryName(inputString.begin(), entryNameEnd);
    ConsoleEntry* consoleEntry = ConsoleRegistry::getSingleton().findEntry(entryName);

    if(consoleEntry == nullptr)
    {
        if(!ignoreErrors)
        {
            LOG_ERROR("Could not find \"%s\" console entry!", entryName.c_str());
        }

        return;
    }

    // Parse additional value or function call argument.
    auto valueBegin = std::find_if_not(entryNameEnd, inputString.end(),
        [](char c) { return std::isspace(c) != 0; });

    if(entryNameEnd != inputString.end() && valueBegin != inputString.end())
    {
        if(*entryNameEnd == '(')
        {
            auto braceBegin = entryNameEnd;
            auto braceEnd = std::find(inputString.rbegin(), inputString.rend(), ')');

            if(braceEnd != inputString.rend())
            {
                std::string argumentString(braceBegin + 1, braceEnd.base() - 1);
                consoleEntry->call(argumentString);
            }
            else
            {
                if(!ignoreErrors)
                {
                    LOG_ERROR("Missing closing brace for an apparent function call!");
                }
            }
        }
        else
        {
            auto valueEnd = std::find_if_not(inputString.rbegin(), inputString.rend(),
                [](char c) { return std::isspace(c) != 0; }).base();

            std::string valueString(valueBegin, valueEnd);
            consoleEntry->write(valueString);
        }
    }
    else
    {
        consoleEntry->read();
    }
}

void Console::display()
{
    if(!cv_showConsole.value)
        return;

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;

    ImVec2 consoleMinSize = ImGui::GetIO().DisplaySize;
    consoleMinSize.y = 108.0f;

    ImVec2 consoleMaxSize = ImGui::GetIO().DisplaySize;
    consoleMaxSize.y = std::max(ImGui::GetIO().DisplaySize.y, consoleMinSize.y);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(consoleMinSize, consoleMaxSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    if(ImGui::Begin("Console", nullptr, consoleMinSize, -1.0f, flags))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();

        // Console message history.
        if(ImGui::BeginChild("ConsoleMessages", ImVec2(0.0f, windowSize.y - 40.0f)))
        {
            const auto& messages = g_logger->getMessages();

            for(const auto& message : messages)
            {
                switch(message.type)
                {
                case LogType::Trace:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
                    break;

                case LogType::Info:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    break;

                case LogType::Warning:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
                    break;

                case LogType::Error:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.0f, 1.0f));
                    break;

                case LogType::Fatal:
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    break;
                }

                ImGui::TextWrapped(message.text.c_str());
                ImGui::PopStyleColor();

                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::EndChild();
        ImGui::Separator();

        // Prepare string buffer.
        if(m_consoleInput.empty())
        {
            m_consoleInput.resize(32, '\0');
        }

        // Console text input.
        ImGui::PushItemWidth(-1);

        if(ImGui::InputText("ConsoleInput", &m_consoleInput, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // Prepare input string.
            ASSERT(m_consoleInput.capacity() != 0);
            std::string inputString(&m_consoleInput[0]);

            // Regain focus on the input field.
            ImGui::SetKeyboardFocusHere();

            // Print entered console command.
            LOG_INFO("> %s", inputString.c_str());

            // Parse console input.
            this->parseInput(inputString);

            // Clear console input.
            m_consoleInput.clear();
            m_consoleInput.resize(1, '\0');
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}
