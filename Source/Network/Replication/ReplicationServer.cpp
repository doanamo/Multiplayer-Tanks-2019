#include "Precompiled.hpp"
#include "ReplicationServer.hpp"
#include "Game/GameInstance.hpp"
#include "Network/Interfaces/NetworkBase.hpp"

ReplicationServer::ReplicationServer()
{
}

ReplicationServer::~ReplicationServer()
{
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
    command.type = ReplicationCommand::Create;
    command.handle = replicable->getReplicableHandle();

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
    command.type = ReplicationCommand::Destroy;
    command.handle = replicable->getReplicableHandle();

    // Add command to reliable queue.
    m_reliableCommands.push_back(command);

    // Call base method to unregister replicable object.
    bool result = ReplicationBase::onObjectDestroyed(object);
    ASSERT(result, "Expected to return true as object is derived from Replicable class!");

    // Success!
    return true;
}
