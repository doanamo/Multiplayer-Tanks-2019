#include "Precompiled.hpp"
#include "NetworkServer.hpp"
#include "Network/Connection/ConnectionBackend.hpp"
#include "Network/Packets/Protocol.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"
#include "Game/SnapshotSaveLoad.hpp"
#include "Game/Player/PlayerManager.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerControllerClient.hpp"

NetworkServer::NetworkServer() :
    m_maxClientCount(16)
{
}

NetworkServer::~NetworkServer()
{
    LOG_TRACE("NetworkServer::~NetworkServer()");
}

bool NetworkServer::initialize(GameInstance* gameInstance, unsigned short port)
{
    // Initialize base class.
    if(!NetworkBase::initialize(gameInstance))
        return false;

    // Initialize replication system.
    if(!m_replication.initialize(gameInstance))
        return false;

    // Initialize socket connection.
    if(!m_socket.listen(port))
    {
        LOG_ERROR("Could not initialize socket connection!");
        return false;
    }
    
    // Preallocate array of clients.
    m_clients.resize(m_maxClientCount);

    return true;
}

void NetworkServer::update(float timeDelta)
{
    NetworkBase::update(timeDelta);
}


PlayerControllerClient* NetworkServer::fetchClientController(const PlayerHandle& playerHandle)
{
    // Retrieve player entry.
    PlayerManager& playerManager = m_gameInstance->getPlayerManager();
    Player* player = playerManager.fetchPlayer(playerHandle);
    if(player == nullptr)
        return nullptr;

    // Retrieve client controller.
    PlayerControllerBase* playerController = player->getPlayerController();
    if(playerController == nullptr)
        return nullptr;

    return playerController->as<PlayerControllerClient>();
}

void NetworkServer::preTick(float timeDelta)
{
    NetworkBase::preTick(timeDelta);

    // Receive packets from connected clients.
    ASSERT(m_clients.size() == m_maxClientCount, "We do not expect client size to change at runtime!");

    for(int i = 0; i < m_clients.size(); ++i)
    {
        // Check if client connection socket exists.
        ClientEntry& clientEntry = m_clients[i];
        if(clientEntry.socket == nullptr)
            continue;

        // Respond to received packet.
        std::unique_ptr<PacketBase> receivedPacket;
        while(receivePacket(*clientEntry.socket, receivedPacket, nullptr))
        {
            if(receivedPacket->is<PacketRequestConnection>())
            {
                // Create new player entry.
                PlayerManager& playerManager = m_gameInstance->getPlayerManager();
                Player& player = playerManager.createPlayer();

                // Assign created player handle to client entry.
                clientEntry.playerHandle = player.getPlayerHandle();

                // Create player controller.
                // #todo: Resolving player name by handle's identifier is a big hack.
                std::ostringstream playerName;
                playerName << "Player_" << player.getPlayerHandle().getIdentifier();

                Object* playerTank = m_gameInstance->getWorld().getObjectByName(playerName.str());
                ASSERT(playerTank != nullptr, "Expected hardcoded player tank to exist!");

                auto playerController = std::make_unique<PlayerControllerClient>();
                playerController->setControlledObject(playerTank->getHandle());

                player.setPlayerController(std::move(playerController));

                // Send accept connection packet.
                // Specify client index which starts from 1, with 0 reserved for server.
                PacketAcceptConnection packetAcceptConnection;
                packetAcceptConnection.playerHandle = player.getPlayerHandle();

                if(!sendPacket(*clientEntry.socket, packetAcceptConnection, true))
                {
                    LOG_ERROR("Failed to send accept connection packet to client!");
                    continue;
                }

                // Save game snapshot into packet memory stream.
                PacketServerSnapshot packetStateSnapshot;
                SnapshotSaveLoad snapshotSave(m_gameInstance);

                if(!snapshotSave.save(packetStateSnapshot.serializedGameInstance))
                {
                    LOG_ERROR("Coult not save snapshot into packet memoty!");
                    continue;
                }

                // Send reliable snapshot packet.
                if(!sendPacket(*clientEntry.socket, packetStateSnapshot, true))
                {
                    LOG_ERROR("Failed to send state snapshot packet to client!");
                    continue;
                }
            }
            else if(receivedPacket->is<PacketClientInput>())
            {
                // Deserialize client input packet.
                PacketClientInput* packetClientInput = receivedPacket->as<PacketClientInput>();
                ASSERT(packetClientInput != nullptr);

                // Fetch client controller and process received player commands.
                // Current design allows packet sender to impersonate client by using their player handle.
                // Additional verification is needed to verify source of the packet.
                PlayerControllerClient* clientController = fetchClientController(packetClientInput->playerHandle);

                if(clientController)
                {
                    clientController->processPlayerCommandBuffer(packetClientInput->playerCommands);
                }
                else
                {
                    Player* player = m_gameInstance->getPlayerManager().fetchPlayer(packetClientInput->playerHandle);
                    ASSERT(player == nullptr, "Player entry exists but it does not have client controller!");
                    LOG_WARNING("Received client input packet with player handle that does not exist.");
                }
            }
            else if(receivedPacket->is<PacketMessage>())
            {
                // Deserialize message packet.
                PacketMessage* packetMessage = receivedPacket->as<PacketMessage>();
                ASSERT(packetMessage != nullptr);

                LOG_INFO("Received message packet with \"%s\" text.", packetMessage->text.c_str());

                // Send response packet.
                {
                    PacketMessage packetMessage;
                    packetMessage.text = "Hello client!";

                    if(!sendPacket(*clientEntry.socket, packetMessage, true))
                    {
                        LOG_ERROR("Failed to send message packet to client!");
                        continue;
                    }
                }
            }
        }
    }
}

void NetworkServer::postTick(float timeDelta)
{
    NetworkBase::postTick(timeDelta);

    // Collect replicable object data.
    m_replication.collectReplicationData();

    // Prepare reliable server update packet.
    PacketServerUpdate reliableUpdatePacket;
    reliableUpdatePacket.tickFrame = m_gameInstance->getTickFrame();
    reliableUpdatePacket.replicationCommands = m_replication.getReliableCommands();

    // Prepare unreliable server update packet.
    PacketServerUpdate unreliableUpdatePacket;
    unreliableUpdatePacket.tickFrame = m_gameInstance->getTickFrame();
    unreliableUpdatePacket.replicationCommands = m_replication.getUnreliableCommands();

    // Clear processed replication commands.
    m_replication.clearReplicationCommands();

    // Send reliable update packet to connected clients.
    if(!reliableUpdatePacket.replicationCommands.empty())
    {
        LOG_REPLICATION_TRACE("Sending reliable update packet...");

        for(auto& clientEntry : m_clients)
        {
            if(clientEntry.socket == nullptr)
                continue;

            // Fetch client controller.
            PlayerControllerClient* clientController = fetchClientController(clientEntry.playerHandle);
            ASSERT(clientController != nullptr, "Client player entry has incorrect controller!");

            // Fill client specific data.
            reliableUpdatePacket.acknowledgedPlayerCommand = clientController->getAcknowledgedIndex();
            
            // Send reliable server update.
            if(!sendPacket(*clientEntry.socket, reliableUpdatePacket, true))
            {
                LOG_ERROR("Failed to send reliable server update packet to client!");
                continue;
            }
        }
    }

    // Send unreliable update packet to connected clients.
    if(!unreliableUpdatePacket.replicationCommands.empty())
    {
        LOG_REPLICATION_TRACE("Sending unreliable update packet...");

        for(auto& clientEntry : m_clients)
        {
            if(clientEntry.socket == nullptr)
                continue;

            // Fetch client controller.
            PlayerControllerClient* clientController = fetchClientController(clientEntry.playerHandle);
            ASSERT(clientController != nullptr, "Client player entry has incorrect controller!");

            // Fill client specific data.
            unreliableUpdatePacket.acknowledgedPlayerCommand = clientController->getAcknowledgedIndex();

            // Send unreliable server update.
            if(!sendPacket(*clientEntry.socket, unreliableUpdatePacket, false))
            {
                LOG_ERROR("Failed to send unreliable server update packet to client!");
                continue;
            }
        }
    }

    // Receive packets from default socket.
    // This needs another solution, either simplify or creating ConnectionSwitch for handling connections.
    ConnectionContext& socketContext = m_socket.getConnectionContext();
    ConnectionContext::PacketEntry packetEntry;

    while(socketContext.popIncoming(&packetEntry))
    {
        // Read received packet.
        std::unique_ptr<PacketBase> receivedPacket;
        if(!readPacket(packetEntry.packet, receivedPacket))
            continue;

        // Retrieve connect packet.
        PacketRequestConnection* requestConnectionPacket = receivedPacket->as<PacketRequestConnection>();
        if(requestConnectionPacket == nullptr)
            continue;

        // Check if we already have socket registered with remote address and port.
        auto socketBackend = m_socket.getConnectionBackend();
        if(socketBackend->hasSocketRegistered(packetEntry.address, packetEntry.port))
            continue;

        // Find empty client entry.
        auto clientEntryIt = m_clients.begin();
        while(clientEntryIt != m_clients.end())
        {
            if(clientEntryIt->socket == nullptr)
                break;

            ++clientEntryIt;
        }

        if(clientEntryIt == m_clients.end())
        {
            LOG_WARNING("Cannot connect new client due to connection limit being reached.");
            continue;
        }

        // Add new client connection list.
        std::unique_ptr<ConnectionSocket> clientSocket = std::make_unique<ConnectionSocket>(socketBackend);

        if(!clientSocket->connect(packetEntry.address, packetEntry.port))
        {
            LOG_ERROR("Could not connect new client socket!");
            continue;
        }

        // Move socket to client list.
        ClientEntry& clientEntry = *clientEntryIt;
        clientEntry.socket = std::move(clientSocket);

        // Reset received memory stream index so it can be read again.
        packetEntry.packet.reset();

        // Push received packet to new socket.
        clientEntry.socket->getConnectionContext().pushIncoming(packetEntry);
    }
}

void NetworkServer::draw()
{
    NetworkBase::draw();

    // Draw ImGui debug window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 100));
    if(ImGui::Begin("Network State (Server)##NetworkState", &cv_showNetworkInfo.value))
    {
    }
    ImGui::End();
    ImGui::PopStyleVar(1);
}

NetworkMode NetworkServer::getMode() const
{
    return NetworkMode::Server;
}

bool NetworkServer::isConnected() const
{
    return true;
}

ReplicationBase& NetworkServer::getReplication()
{
    return m_replication;
}
