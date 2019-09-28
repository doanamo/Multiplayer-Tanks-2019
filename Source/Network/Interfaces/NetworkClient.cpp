#include "Precompiled.hpp"
#include "NetworkClient.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"
#include "Game/SnapshotSaveLoad.hpp"
#include "Game/Player/PlayerManager.hpp"
#include "Game/Player/PlayerControllerRemote.hpp"

NetworkClient::NetworkClient() :
    m_hearbeatTimer(0.0f)
{
}

NetworkClient::~NetworkClient()
{
}

bool NetworkClient::initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port)
{
    // Initialize base class.
    if(!NetworkBase::initialize(gameInstance))
        return false;

    // Initialize replication system.
    if(!m_replication.initialize(gameInstance))
        return false;

    // Initialize socket connection.
    if(!m_socket.connect(address, port))
    {
        LOG_ERROR("Could not initialize socket connection!");
        return false;
    }

    // Send connection request to server.
    PacketRequestConnection requestConnectionPacket;
    sendPacket(m_socket, requestConnectionPacket, true);

    // Receive and process connection acceptance.
    while(true)
    {
        // Wait until we receive packet.
        std::unique_ptr<PacketBase> receivedPacket;
        if(!receivePacket(m_socket, receivedPacket))
            continue;

        // Check if packet type matches expected type.
        PacketAcceptConnection* acceptConnectionPacket = receivedPacket->as<PacketAcceptConnection>();
        if(acceptConnectionPacket == nullptr)
            continue;

        // Save player handle assigned to this client.
        m_playerHandle = acceptConnectionPacket->playerHandle;

        // Break out of loop.
        break;
    }

    // Receive and process state snapshot.
    while(true)
    {
        // Wait until we receive packet.
        std::unique_ptr<PacketBase> receivedPacket;
        if(!receivePacket(m_socket, receivedPacket))
            continue;

        // Check if packet type matches expected type.
        PacketServerSnapshot* snapshotPacket = receivedPacket->as<PacketServerSnapshot>();
        if(snapshotPacket == nullptr)
            continue;

        // Load snapshot from packet memory.
        SnapshotSaveLoad snapshotLoad(m_gameInstance);
        if(!snapshotLoad.load(snapshotPacket->serializedGameInstance))
        {
            LOG_ERROR("Could not load snapshot from packet memory!");
            return false;
        }

        // Remove all player controllers loaded into player manager.
        // They may have gotten deserialized from game snapshot.
        PlayerManager& playerManager = gameInstance->getPlayerManager();
        playerManager.removeControllers();

        // Fetch this client's player entry.
        Player* player = playerManager.fetchPlayer(m_playerHandle);

        if(player == nullptr)
        {
            LOG_ERROR("We have been assigned player handle that does not exists!");
            return false;
        }

        // Create player controller for our player entry.
        auto remoteController = std::make_unique<PlayerControllerRemote>();
        player->setPlayerController(std::move(remoteController));

        // Break out of wait loop.
        break;
    }

    // Success!
    return true;
}

void NetworkClient::update(float timeDelta)
{
    NetworkBase::update(timeDelta);

    // Consume player commands and send them before they get processed.
    PlayerControllerRemote* remoteController = this->getRemoteController();
    remoteController->consumePlayerCommands();

    if(remoteController->hasUnacknowledgedPlayerCommands())
    {
        PacketClientInput packetClientInput;
        packetClientInput.playerHandle = m_playerHandle;
        packetClientInput.playerCommands = remoteController->constructPlayerCommandBuffer();

        if(!sendPacket(m_socket, packetClientInput, false))
        {
            LOG_ERROR("Failed to send client input packet!");
        }
    }
}

void NetworkClient::preTick(float timeDelta)
{
    NetworkBase::preTick(timeDelta);

    // Send periodical packet.
    m_hearbeatTimer = std::max(0.0f, m_hearbeatTimer - timeDelta);

    if(m_hearbeatTimer == 0.0f)
    {
        PacketMessage packetMessage;
        packetMessage.text = "Hello server!";

        sendPacket(m_socket, packetMessage, true);

        m_hearbeatTimer = 1.0f;
    }

    // Receive packets.
    std::unique_ptr<PacketBase> receivedPacket;
    bool reliablePacket;

    sf::IpAddress senderAddress;
    unsigned short senderPort;

    while(receivePacket(m_socket, receivedPacket, &reliablePacket, &senderAddress, &senderPort))
    {
        if(receivedPacket->is<PacketServerUpdate>())
        {
            PacketServerUpdate* packetServerUpdate = receivedPacket->as<PacketServerUpdate>();
            ASSERT(packetServerUpdate != nullptr);

            // Process player command acknowledgment.
            PlayerControllerRemote* remoteController = this->getRemoteController();
            remoteController->acknowledgePlayerCommand(packetServerUpdate->acknowledgedPlayerCommand);

            // Process replication commands.
            m_replication.processServerUpdatePacket(*packetServerUpdate, reliablePacket);
        }
        else if(receivedPacket->is<PacketMessage>())
        {
            PacketMessage* packetMessage = receivedPacket->as<PacketMessage>();
            ASSERT(packetMessage != nullptr);

            LOG_INFO("Received message packet with \"%s\" text.", packetMessage->text.c_str());
        }
    }
}

void NetworkClient::postTick(float timeDelta)
{
    NetworkBase::postTick(timeDelta);
}

void NetworkClient::draw()
{
    NetworkBase::draw();

    // Draw ImGui debug window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 100));
    if(ImGui::Begin("Network State (Client)##NetworkState", &cv_showNetworkInfo.value))
    {

    }
    ImGui::End();
    ImGui::PopStyleVar(1);
}

NetworkMode NetworkClient::getMode() const
{
    return NetworkMode::Client;
}

bool NetworkClient::isConnected() const
{
    return false;
}

ReplicationBase& NetworkClient::getReplication()
{
    return m_replication;
}

const PlayerHandle& NetworkClient::getPlayerHandle() const
{
    return m_playerHandle;
}

PlayerControllerRemote* NetworkClient::getRemoteController()
{
    // Retrieve player remote controller.
    ASSERT(m_playerHandle.isValid(), "Client's player hadle is not valid!");
    Player* player = m_gameInstance->getPlayerManager().fetchPlayer(m_playerHandle);
    PlayerControllerRemote* remoteController = player->getPlayerController()->as<PlayerControllerRemote>();
    ASSERT(remoteController != nullptr, "Expected remote controller to exist for local client!");

    return remoteController;
}
