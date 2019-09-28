#include "Precompiled.hpp"
#include "GameStateLoading.hpp"
#include "GameStateSession.hpp"
#include "GameStateMainMenu.hpp"
#include "Game/GameInstance.hpp"
#include "Game/SnapshotSaveLoad.hpp"
#include "Game/World/World.hpp"
#include "Game/Objects/Tank.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerManager.hpp"
#include "Game/Player/PlayerControllerHuman.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameProvisionParams::GameProvisionParams() :
    provisionMode(GameProvisionMode::Regular),
    snapshotFile(""),
    snapshotStream(nullptr),
    connectionAddress("127.0.0.1"),
    connectionPort("2077")
{
}

GameStateLoading::GameStateLoading(GameProvisionParams& params) :
    m_params(params),
    m_delay(1.0f)
{
}

GameStateLoading::~GameStateLoading()
{
}

bool GameStateLoading::provisionSession(std::shared_ptr<GameStateSession>& session)
{
    // Parse port number string.
    unsigned short portNumber = 0;
    if(!parseStringToPort(m_params.connectionPort, portNumber))
        return false;

    // Prepare network parameters.
    NetworkParams networkParams;

    if(m_params.provisionMode == GameProvisionMode::Host)
    {
        networkParams.mode = NetworkMode::Server;
        networkParams.port = portNumber;
    }
    else if(m_params.provisionMode == GameProvisionMode::Connect)
    {
        networkParams.mode = NetworkMode::Client;
        networkParams.address = "127.0.0.1";
        networkParams.port = portNumber;
    }
    else
    {
        networkParams.mode = NetworkMode::Offline;
    }

    // Initialize game state session.
    if(!session->initialize(networkParams))
        return false;

    // Retrieve created game instance.
    GameInstance* gameInstance = session->getGameInstance();
    ASSERT(gameInstance, "Game instance was expected to exist!");

    // Provision created game state session.
    if(m_params.provisionMode == GameProvisionMode::Regular ||
        m_params.provisionMode == GameProvisionMode::Host)
    {
        // #todo: This should be replaced by dedicated game mode logic which spawns tanks for connected players.
        World& world = gameInstance->getWorld();

        for(int i = 0; i < 6; ++i)
        {
            // Create player tank object.
            std::unique_ptr<Tank> tankObject(new Tank());
            tankObject->getTransform().setPosition(sf::Vector2f(2.0f * (i - 3), 0.0f));
            Handle tankHandle = world.addObject(std::move(tankObject));

            // Group and name tank object.
            std::ostringstream playerName;
            playerName << "Player_" << i + 1;

            world.setObjectGroup(tankHandle, "Players");
            world.setObjectName(tankHandle, playerName.str());

            // Create local player with controller.
            if(i == 0)
            {
                std::unique_ptr<PlayerControllerHuman> playerController = std::make_unique<PlayerControllerHuman>();
                playerController->setControlledObject(tankHandle);

                auto& player = gameInstance->getPlayerManager().createPlayer();
                player.setPlayerController(std::move(playerController));
            }
        }
    }

    // Load snapshot file.
    if(m_params.provisionMode == GameProvisionMode::LoadFromFile)
    {
        SnapshotSaveLoad snapshotLoader(gameInstance);
        if(!snapshotLoader.load(m_params.snapshotFile))
            return false;
    }
    else if(m_params.provisionMode == GameProvisionMode::LoadFromStream)
    {
        ASSERT(m_params.snapshotStream);

        SnapshotSaveLoad snapshotLoader(gameInstance);
        if(!snapshotLoader.load(*m_params.snapshotStream))
            return false;
    }

    // Success!
    return true;
}

void GameStateLoading::handleEvent(const sf::Event& event)
{
}

void GameStateLoading::update(float timeDelta)
{
    if(m_delay == 0.0f)
    {
        // Provision new session game state and change to it.
        auto gameStateSession = std::make_shared<GameStateSession>();

        if(!provisionSession(gameStateSession) || !getStateMachine()->changeState(gameStateSession))
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
