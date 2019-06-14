#include "Precompiled.hpp"
#include "GameStateEntry.hpp"
#include "GameStateMainMenu.hpp"
#include "GameStateLoading.hpp"
#include "System/Globals.hpp"
#include "System/CommandLine.hpp"

ConsoleVariable<std::string> cv_address("address", "127.0.0.1");
ConsoleVariable<std::string> cv_port("port", "2076");

GameStateEntry::GameStateEntry()
{
}

GameStateEntry::~GameStateEntry()
{
}

bool GameStateEntry::initialize()
{
    return true;
}

void GameStateEntry::handleEvent(const sf::Event& event)
{
}

void GameStateEntry::update(float timeDelta)
{
}

void GameStateEntry::tick(float timeDelta)
{
}

void GameStateEntry::draw(float timeAlpha)
{
}

bool GameStateEntry::onStateEnter(State<GameStateBase>* previousState)
{
    // Parse port number string.
    unsigned short portNumber = 0;
    if(!parseStringToPort(cv_port.value, portNumber))
        return false;

    // Transition to next state based on launch arguments (or their lack). 
    std::shared_ptr<State<GameStateBase>> nextState;

    if(g_commandLine->hasArgument("host"))
    {
        GameProvisionParams provisionParams;
        provisionParams.provisionMode = GameProvisionMode::Host;
        provisionParams.connectionAddress = cv_address.value;
        provisionParams.connectionPort = portNumber;

        nextState = std::make_shared<GameStateLoading>(provisionParams);
    }
    else if(g_commandLine->hasArgument("connect"))
    {
        GameProvisionParams provisionParams;
        provisionParams.provisionMode = GameProvisionMode::Connect;
        provisionParams.connectionAddress = cv_address.value;
        provisionParams.connectionPort = portNumber;

        nextState = std::make_shared<GameStateLoading>(provisionParams);
    }
    else
    {
        nextState = std::make_shared<GameStateMainMenu>();
    }

    // Transition game state.
    auto stateMachine = getStateMachine();
    if(stateMachine == nullptr)
        return false;

    return stateMachine->changeState(nextState);
}

bool GameStateEntry::onStateExit(State<GameStateBase>* newState)
{
    return true;
}
