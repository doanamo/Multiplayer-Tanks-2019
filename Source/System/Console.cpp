#include "Precompiled.hpp"
#include "System/Console.hpp"
#include "System/Window.hpp"
#include "System/Globals.hpp"

ConsoleVariable<bool> cv_showConsole("showConsole", false);

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

            // Parse command and arguments.
            std::size_t delimeter = inputString.find_first_of(' ');
            std::string command = inputString.substr(0, std::min(inputString.length(), delimeter));
            std::string arguments = inputString.substr(std::min(command.length(), delimeter));

            // Remove leading arguments space.
            if(!arguments.empty() && arguments[0] == ' ')
            {
                arguments = arguments.substr(1);
            }

            // Check if command is a function call.
            bool functionalCall = false;

            if(command.size() >= 3 && command[command.size() - 2] == '(' && command[command.size() - 1] == ')')
            {
                command = command.substr(0, command.size() - 2);
                functionalCall = true;
            }

            // Find console entry.
            ConsoleEntry* consoleEntry = ConsoleRegistry::getSingleton().findEntry(command);

            if(consoleEntry)
            {
                if(functionalCall)
                {
                    consoleEntry->call(arguments);
                }
                else if(!arguments.empty())
                {
                    consoleEntry->write(arguments);
                }
                else
                {
                    consoleEntry->read();
                }
            }
            else
            {
                LOG_ERROR("Could not find \"%s\" console entry!", command.c_str());
            }

            // Clear console input.
            m_consoleInput.clear();
            m_consoleInput.resize(1, '\0');
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}
