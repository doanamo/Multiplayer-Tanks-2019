#include "Precompiled.hpp"
#include "System/Console.hpp"
#include "System/Window.hpp"
#include "System/Globals.hpp"

Console::Console() :
    m_display(false)
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

void Console::toggle()
{
    m_display = !m_display;
}

void Console::display()
{
    if(!m_display)
        return;

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoScrollbar;

    ImVec2 consoleMinSize = ImGui::GetIO().DisplaySize;
    consoleMinSize.y = 108.0f;

    ImVec2 consoleMaxSize = ImGui::GetIO().DisplaySize;
    consoleMaxSize.y = std::max(ImGui::GetIO().DisplaySize.y - 40.0f, consoleMinSize.y);

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints(consoleMinSize, consoleMaxSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    if(ImGui::Begin("Console", nullptr, consoleMinSize, -1.0f, flags))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();

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

        std::string consoleInput;

        ImGui::PushItemWidth(-1);
        if(ImGui::InputText("ConsoleInput", &consoleInput, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            LOG_INFO("> %s", consoleInput.c_str());
            ImGui::SetKeyboardFocusHere();
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
}
