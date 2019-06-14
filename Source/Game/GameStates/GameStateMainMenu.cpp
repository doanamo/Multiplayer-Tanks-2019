#include "Precompiled.hpp"
#include "GameStateMainMenu.hpp"
#include "GameStateLoading.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameStateMainMenu::GameStateMainMenu()
{
    // Set default variables.
    std::string defaultConnectionAddress = "127.0.0.1";
    m_inputConnectionAddress = std::vector<char>(defaultConnectionAddress.begin(), defaultConnectionAddress.end());
    m_inputConnectionAddress.push_back(0);

    std::string defaultConnectionPort = "2077";
    m_inputConnectionPort = std::vector<char>(defaultConnectionPort.begin(), defaultConnectionPort.end());
    m_inputConnectionPort.push_back(0);
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
            GameProvisionParams provisionParams;
            provisionParams.provisionMode = GameProvisionMode::Regular;

            auto gameStateLoading = std::make_shared<GameStateLoading>(provisionParams);
            getStateMachine()->changeState(gameStateLoading);
        }

        if(ImGui::Button("Load Game", getButtonSize()))
        {
            GameProvisionParams provisionParams;
            provisionParams.provisionMode = GameProvisionMode::LoadFromFile;
            provisionParams.snapshotFile = "Snapshot.save";

            auto gameStateLoading = std::make_shared<GameStateLoading>(provisionParams);
            getStateMachine()->changeState(gameStateLoading);
        }

        if(ImGui::Button("Host Game", getButtonSize()))
        {
            ImGui::OpenPopup("Host Game");
        }

        if(ImGui::BeginPopupModal("Host Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Port", &m_inputConnectionPort, ImGuiInputTextFlags_CharsDecimal);
            ImGui::Separator();

            float availableWidth = ImGui::GetContentRegionAvailWidth();

            if(ImGui::Button("Host", ImVec2(availableWidth * 0.5f, 0)))
            {
                GameProvisionParams provisionParams;
                provisionParams.provisionMode = GameProvisionMode::Host;
                provisionParams.connectionAddress = std::string(m_inputConnectionAddress.begin(), m_inputConnectionAddress.end());
                provisionParams.connectionPort = std::string(m_inputConnectionPort.begin(), m_inputConnectionPort.end());

                auto gameStateLoading = std::make_shared<GameStateLoading>(provisionParams);
                getStateMachine()->changeState(gameStateLoading);
            }
            float size = ImGui::GetItemRectSize().x;

            ImGui::SameLine();
            if(ImGui::Button("Back", ImVec2(-1, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if(ImGui::Button("Join Game", getButtonSize()))
        {
            ImGui::OpenPopup("Join Game");
        }

        if(ImGui::BeginPopupModal("Join Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Address", &m_inputConnectionAddress, ImGuiInputTextFlags_CharsDecimal);
            ImGui::InputText("Port", &m_inputConnectionPort, ImGuiInputTextFlags_CharsDecimal);
            ImGui::Separator();

            float availableWidth = ImGui::GetContentRegionAvailWidth();

            if(ImGui::Button("Join", ImVec2(availableWidth * 0.5f, 0)))
            {
                GameProvisionParams provisionParams;
                provisionParams.provisionMode = GameProvisionMode::Connect;
                provisionParams.connectionAddress = std::string(m_inputConnectionAddress.begin(), m_inputConnectionAddress.end());
                provisionParams.connectionPort = std::string(m_inputConnectionPort.begin(), m_inputConnectionPort.end());

                auto gameStateLoading = std::make_shared<GameStateLoading>(provisionParams);
                getStateMachine()->changeState(gameStateLoading);
            }

            ImGui::SameLine();
            if(ImGui::Button("Back", ImVec2(-1, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
        if(ImGui::Button("Options", getButtonSize()))
        {
        }
        ImGui::PopStyleVar();

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.6f);
        if(ImGui::Button("Credits", getButtonSize()))
        {
        }
        ImGui::PopStyleVar();

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
