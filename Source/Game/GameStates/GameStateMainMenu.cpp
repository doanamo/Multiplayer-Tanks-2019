#include "Precompiled.hpp"
#include "GameStateMainMenu.hpp"
#include "GameStateSession.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameStateMainMenu::GameStateMainMenu()
{
}

GameStateMainMenu::~GameStateMainMenu()
{
}

void GameStateMainMenu::handleEvent(const sf::Event& event)
{
}

void GameStateMainMenu::update(float timeDelta)
{
}

void GameStateMainMenu::tick(float timeDelta)
{
}

void GameStateMainMenu::draw(float timeAlpha)
{
    auto getButtonSize = []() -> ImVec2
    {
        return ImVec2(ImGui::GetContentRegionAvailWidth() + 8.0f, 0.0f);
    };

    ImGui::SetNextWindowContentWidth(200.0f);
    ImGui::SetNextWindowPos(ImVec2(50.0f, 50.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

    if(ImGui::Begin("Main Menu", nullptr, windowFlags))
    {
        if(ImGui::Button("Solo Game", getButtonSize()))
        {
            // Temp: Should be replaced by loading screen state.
            auto gameStateSession = std::make_shared<GameStateSession>();

            if(gameStateSession->initialize())
            {
                getStateMachine()->changeState(gameStateSession);
            }
        }

        if(ImGui::Button("Load Game", getButtonSize()))
        {
        }

        if(ImGui::Button("Host Game", getButtonSize()))
        {
        }

        if(ImGui::Button("Join Game", getButtonSize()))
        {
        }

        if(ImGui::Button("Options", getButtonSize()))
        {
        }

        if(ImGui::Button("Credits", getButtonSize()))
        {
        }

        if(ImGui::Button("Exit", getButtonSize()))
        {
            g_window->close();
        }
    }
    ImGui::End();

    ImGui::PopStyleVar();
}

bool GameStateMainMenu::onStateEnter(State<GameStateBase>* newState)
{
    return true;
}

bool GameStateMainMenu::onStateExit(State<GameStateBase>* previousState)
{
    return true;
}
