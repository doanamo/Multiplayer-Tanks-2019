#include "Precompiled.hpp"
#include "ReplicationClient.hpp"
#include "Network/Packets/PacketServerUpdate.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

ReplicationClient::ReplicationClient()
{

}

ReplicationClient::~ReplicationClient()
{

}

void ReplicationClient::processServerUpdatePacket(const PacketServerUpdate& packet)
{
    for(auto replicationCommand : packet.replicationCommands)
    {
        if(replicationCommand.type == ReplicationCommand::ReplicationType::Create)
        {
            // Read object type identifier from command data.
            TypeInfo::IdentifierType typeIdentifier = 0;
            if(!deserialize(replicationCommand.data, &typeIdentifier))
            {
                LOG_ERROR("Failed to deserialize type identifier from received replication command!");
                continue;
            }

            // Instantiate new replicable object.
            std::unique_ptr<Object> object(Replicable::create(typeIdentifier));
            if(object == nullptr)
            {
                LOG_ERROR("Failed to instantiate object from received replication command!");
                continue;
            }

            // Cast object to replicable.
            Replicable* replicable = object->as<Replicable>();
            if(replicable == nullptr)
            {
                ASSERT("Instantiated object type is not replicable!");
                continue;
            }

            // Set replicable handle.
            ASSERT(replicationCommand.handle.isValid(), "Replicable handle must be valid!");
            setReplicableHandle(*replicable, replicationCommand.handle);

            // Read initial object replication.
            if(!replicable->deserializeInitialReplication(replicationCommand.data))
            {
                LOG_ERROR("Failed to deserialize initial replication data for object!");
                continue;
            }

            // Add object to world.
            auto objectHandle = m_gameInstance->getWorld()->addObject(std::move(object));

            // Retrieve added replicable object back.
            Object* addedObject = m_gameInstance->getWorld()->getObjectByHandle(objectHandle);
            ASSERT(addedObject != nullptr, "Failed to retrieve just added object!");

            Replicable* addedReplicable = addedObject->as<Replicable>();
            ASSERT(addedReplicable != nullptr, "Failed to retrieve just added replicable!");

            // Register replicable object.
            registerReplicable(*addedReplicable);
        }
        else if(replicationCommand.type == ReplicationCommand::ReplicationType::Update)
        {
            // #todo
        }
        else if(replicationCommand.type == ReplicationCommand::ReplicationType::Destroy)
        {
            // Fetch replication entry.
            auto replicationEntry = m_replicables.fetchHandle(replicationCommand.handle);
            if(replicationEntry.value == nullptr)
            {
                LOG_WARNING("Failed to fetch replication handle that is expect to exist!");
                continue;
            }

            // Destroy object (if it still exists).
            m_gameInstance->getWorld()->destroyObject(replicationEntry.value->objectHandle);

            // Unregister replicable object now.
            // Some time can pass before destroy object commands are flushed,
            // so we cannot rely on onDestroyObject() method for unregistration.
            unregisterReplicable(replicationEntry.handle);
        }
    }
}

bool ReplicationClient::onObjectCreated(Object& object)
{
    // Retrieve replicable class.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr)
        return false;

    // Check if object has valid replicable handle.
    ASSERT(replicable->getReplicableHandle().isValid(), "Client attempted to create replicable object!");
    
    // Success!
    return true;
}

bool ReplicationClient::onObjectDestroyed(Object& object)
{
    // Do not unregister replicable handles on object destruction.
    // If some replicable object gets prematurely destroyed by the client,
    // then the replicable entry should remain pointing at an invalid handle.

    // Success!
    return false;
}

bool ReplicationClient::onObjectDeserialized(Object& object)
{
    // Make sure that replicable object that was deserialized has valid replication handle.
    Replicable* replicable = object.as<Replicable>();

    if(replicable != nullptr)
    {
        ASSERT(replicable->getReplicableHandle().isValid(), "Deserialized replicable has invalid handle!");
    }

    return true;
}
