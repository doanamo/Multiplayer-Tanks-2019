#include "Precompiled.hpp"
#include "GameStateSession.hpp"
#include "GameStateLoading.hpp"
#include "GameStateMainMenu.hpp"
#include "Game/GameInstance.hpp"
#include "Game/SnapshotSaveLoad.hpp"
#include "Game/World/World.hpp"
#include "Game/Objects/Tank.hpp"
#include "Game/Level.hpp"
#include "Game/PlayerController.hpp"
#include "Network/Interface/NetworkInterface.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameStateSession::GameStateSession() :
    m_gameMenuOpen(false)
{
}

GameStateSession::~GameStateSession()
{
}
    
bool GameStateSession::initialize(const NetworkParams& networkParams)
{
    // Initialize game instance.
    m_gameInstance = std::make_unique<GameInstance>();
    if(!m_gameInstance->initialize(networkParams))
        return false;

    return true;
}

bool GameStateSession::onStateEnter(State<GameStateBase>* previousState)
{
    return true;
}

bool GameStateSession::onStateExit(State<GameStateBase>* newState)
{
    // Clear game instance.
    m_gameInstance = nullptr;

    return true;
}

void GameStateSession::handleEvent(const sf::Event& event)
{
    // Handle keyboard input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Escape:
            // Open game menu.
            if(!m_gameMenuOpen)
            {
                if(!ImGui::IsAnyItemFocused())
                {
                    m_gameMenuOpen = true;
                }
            }
            else
            {
                m_gameMenuOpen = false;
            }
            break;

        case sf::Keyboard::F5:
            // Save snapshot.
            {
                SnapshotSaveLoad snapshotSaver(m_gameInstance.get());
                snapshotSaver.save("Snapshot.save");
            }
            break;

        case sf::Keyboard::F8:
            // Load snapshot.
            {
                GameProvisionParams provisionParams;
                provisionParams.provisionMode = GameProvisionMode::LoadFromFile;
                provisionParams.snapshotFile = "Snapshot.save";

                auto gameStateLoading = std::make_shared<GameStateLoading>(provisionParams);
                getStateMachine()->changeState(gameStateLoading);
            }
            return;

        case sf::Keyboard::F10:
            g_window->close();
            break;
        }
    }

    // Handle events by game instance.
    m_gameInstance->handleEvent(event);
}

void GameStateSession::update(float timeDelta)
{
    // Update game instance.
    m_gameInstance->update(timeDelta);
}

void GameStateSession::tick(float timeDelta)
{
    // Tick game instance.
    m_gameInstance->tick(timeDelta);
}

void GameStateSession::draw(float timeAlpha)
{
    // Draw game instance.
    m_gameInstance->draw(timeAlpha);

    // State to change into.
    std::shared_ptr<GameStateBase> changeState;

    // Draw game menu modal popup.
    auto getButtonSize = []() -> ImVec2
    {
        return ImVec2(ImGui::GetContentRegionAvailWidth() + 8.0f, 0.0f);
    };

    if(m_gameMenuOpen)
    {
        if(!ImGui::IsPopupOpen("Game Menu"))
        {
            ImGui::OpenPopup("Game Menu");
        }
    }

    ImGui::SetNextWindowContentWidth(200.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));

    if(ImGui::BeginPopupModal("Game Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if(ImGui::Button("Resume", getButtonSize()))
        {
            m_gameMenuOpen = false;
        }

        if(ImGui::Button("Save", getButtonSize()))
        {
            SnapshotSaveLoad snapshotSaver(m_gameInstance.get());
            snapshotSaver.save("Snapshot.save");

            m_gameMenuOpen = false;
        }

        if(ImGui::Button("Load", getButtonSize()))
        {
            GameProvisionParams provisionParams;
            provisionParams.provisionMode = GameProvisionMode::LoadFromFile;
            provisionParams.snapshotFile = "Snapshot.save";

            changeState = std::make_shared<GameStateLoading>(provisionParams);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
        if(ImGui::Button("Options", getButtonSize()))
        {
        }
        ImGui::PopStyleVar();

        if(ImGui::Button("Quit", getButtonSize()))
        {
            changeState = std::make_shared<GameStateMainMenu>();
        }

        if(!m_gameMenuOpen)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar();

    // Change to new state if requested.
    if(changeState)
    {
        getStateMachine()->changeState(changeState);
    }
}

GameInstance* GameStateSession::getGameInstance()
{
    return m_gameInstance.get();
}
