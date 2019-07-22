#include "Precompiled.hpp"
#include "ReplicationServer.hpp"
#include "Replicable.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"
#include "Network/Interfaces/NetworkBase.hpp"

ReplicationServer::ReplicationServer()
{
}

ReplicationServer::~ReplicationServer()
{
}

void ReplicationServer::collectReplicationData()
{
    // Gather update commands for replicable objects.
    for(auto& replicableEntry : m_replicables)
    {
        // Retrieve replicable object.
        ASSERT(replicableEntry.valid, "Handle map range iteration expected to skip invalid elements!");
        Object* object = m_gameInstance->getWorld()->getObjectByHandle(replicableEntry.value->objectHandle);
        ASSERT(object != nullptr, "Valid replication entry contains invalid handle!");

        // Cast object entry to replicable class.
        Replicable* replicable = object->as<Replicable>();
        ASSERT(replicable != nullptr, "Valid replication entry contains object that is not replicable!");

        // Add reliable replication command if there is any data that needs replication.
        ReplicationCommand reliableCommand;
        reliableCommand.type = ReplicationCommand::ReplicationType::Update;
        reliableCommand.handle = replicable->getReplicableHandle();

        if(replicable->serializeReliableTickReplication(reliableCommand.data))
        {
            if(!reliableCommand.data.empty())
            {
                m_reliableCommands.push_back(reliableCommand);
            }
        }
        else
        {
            LOG_ERROR("Failed to serialize reliable tick replication for object!");
        }

        // Add unreliable replication command if there is any data that needs replication.
        ReplicationCommand unreliableCommand;
        unreliableCommand.type = ReplicationCommand::ReplicationType::Update;
        unreliableCommand.handle = replicable->getReplicableHandle();

        if(replicable->serializeUnreliableTickReplication(unreliableCommand.data))
        {
            if(!unreliableCommand.data.empty())
            {
                m_unreliableCommands.push_back(unreliableCommand);
            }
        }
        else
        {
            LOG_ERROR("Failed to serialize unreliable tick replication for object!");
        }
    }
}

void ReplicationServer::clearReplicationCommands()
{
    m_reliableCommands.clear();
    m_unreliableCommands.clear();
}

const std::vector<ReplicationCommand>& ReplicationServer::getReliableCommands() const
{
    return m_reliableCommands;
}

const std::vector<ReplicationCommand>& ReplicationServer::getUnreliableCommands() const
{
    return m_unreliableCommands;
}

bool ReplicationServer::onObjectCreated(Object& object)
{
    // Call base method to register replicable object.
    if(!ReplicationBase::onObjectCreated(object))
        return false;

    // Retrieve replicable class.
    Replicable* replicable = object.as<Replicable>();
    ASSERT(replicable != nullptr, "Object is not replicable despite base method returning true!");

    // Create a replication command.
    ReplicationCommand command;
    command.type = ReplicationCommand::ReplicationType::Create;
    command.handle = replicable->getReplicableHandle();

    // Write initial replication data.
    if(!replicable->serializeInitialReplication(command.data))
    {
        LOG_ERROR("Failed to serialize initial replication of created object!");
    }

    // Add command to reliable queue.
    m_reliableCommands.push_back(command);

    // Success!
    return true;
}

bool ReplicationServer::onObjectDestroyed(Object& object)
{
    // Retrieve replicable class.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr)
        return false;

    // Create a replication command.
    ReplicationCommand command;
    command.type = ReplicationCommand::ReplicationType::Destroy;
    command.handle = replicable->getReplicableHandle();

    // Add command to reliable queue.
    m_reliableCommands.push_back(command);

    // Call base method to unregister replicable object.
    bool result = ReplicationBase::onObjectDestroyed(object);
    ASSERT(result, "Expected to return true as object is derived from Replicable class!");

    // Success!
    return true;
}
