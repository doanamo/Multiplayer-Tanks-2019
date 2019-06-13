#include "Precompiled.hpp"
#include "GameStateLoading.hpp"
#include "GameStateSession.hpp"
#include "GameStateMainMenu.hpp"
#include "Game/GameInstance.hpp"
#include "Game/PlayerController.hpp"
#include "Game/SnapshotSaveLoad.hpp"
#include "Game/World/World.hpp"
#include "Game/Objects/Tank.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameProvisionParams::GameProvisionParams() :
    provisionMode(GameProvisionMode::Regular),
    snapshotFile(""),
    snapshotStream(nullptr),
    connectionAddress("127.0.0.1"),
    connectionPort(2077)
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
    // Prepare network parameters.
    NetworkParams networkParams;

    if(m_params.provisionMode == GameProvisionMode::Host)
    {
        networkParams.mode = NetworkMode::Server;
        networkParams.port = 2077;
    }
    else if(m_params.provisionMode == GameProvisionMode::Connect)
    {
        networkParams.mode = NetworkMode::Client;
        networkParams.address = "127.0.0.1";
        networkParams.port = 2077;
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
        // Create player tank object.
        Tank* playerTank = new Tank();
        Handle playerHandle = gameInstance->getWorld()->addObject(playerTank, "Player1_Tank", "Players");
        gameInstance->getPlayerController()->control(playerHandle);

        // Test instantiation from runtime type.
        Object* enemyTank = Object::create(getTypeInfo<Tank>().getIdentifier());
        enemyTank->getTransform().setPosition(sf::Vector2f(0.0f, 2.0f));
        gameInstance->getWorld()->addObject(enemyTank);
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
