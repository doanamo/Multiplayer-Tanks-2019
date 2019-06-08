#include "Precompiled.hpp"
#include "GameStateLoading.hpp"
#include "GameStateSession.hpp"
#include "GameStateMainMenu.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameStateLoading::GameStateLoading() :
    m_delay(1.0f)
{
}

GameStateLoading::~GameStateLoading()
{
}

void GameStateLoading::handleEvent(const sf::Event& event)
{
}

void GameStateLoading::update(float timeDelta)
{
    if(m_delay == 0.0f)
    {
        auto gameStateSession = std::make_shared<GameStateSession>();
        if(!getStateMachine()->changeState(gameStateSession))
        {
            auto gameStateMainMenu = std::make_shared<GameStateMainMenu>();
            if(!getStateMachine()->changeState(gameStateMainMenu))
            {
                g_window->close();
            }
        }

        return;
    }

    m_delay = std::max(0.0f, m_delay - timeDelta);
}

void GameStateLoading::tick(float timeDelta)
{
}

void GameStateLoading::draw(float timeAlpha)
{
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoSavedSettings;

    ImGui::SetNextWindowContentWidth(200.0f);
    ImGui::SetNextWindowPosCenter(ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

    if(ImGui::Begin("Loading...", nullptr, windowFlags))
    {
        ImGui::Text("Waiting for %.1f second(s)", m_delay);
    }
    ImGui::End();

    ImGui::PopStyleVar();
}

bool GameStateLoading::onStateEnter(State<GameStateBase>* newState)
{
    return true;
}

bool GameStateLoading::onStateExit(State<GameStateBase>* previousState)
{
    return true;
}
