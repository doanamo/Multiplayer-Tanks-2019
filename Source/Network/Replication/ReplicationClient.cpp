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

void ReplicationClient::processServerUpdatePacket(const PacketServerUpdate& packet, bool reliable)
{
    LOG_REPLICATION_TRACE("Processing server update packet...");

    // Acquire reference to world system.
    World* world = m_gameInstance->getWorld();

    // Process replication commands from the server.
    for(auto replicationCommand : packet.replicationCommands)
    {
        if(replicationCommand.type == ReplicationCommand::ReplicationType::Create)
        {
            LOG_REPLICATION_TRACE("Processing replication create command...");

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
            auto objectHandle = world->addObject(std::move(object));

            // Retrieve added replicable object back.
            Object* addedObject = world->getObjectByHandle(objectHandle);
            ASSERT(addedObject != nullptr, "Failed to retrieve just added object!");

            Replicable* addedReplicable = addedObject->as<Replicable>();
            ASSERT(addedReplicable != nullptr, "Failed to retrieve just added replicable!");

            // Register replicable object.
            registerReplicable(*addedReplicable);
        }
        else if(replicationCommand.type == ReplicationCommand::ReplicationType::Update)
        {
            LOG_REPLICATION_TRACE("Processing replication update command...");

            // Fetch replication entry.
            auto replicationEntry = m_replicables.fetchHandle(replicationCommand.handle);
            if(replicationEntry.value == nullptr)
            {
                LOG_WARNING("Failed to fetch replication handle that is expect to exist!");
                continue;
            }

            // Retrieve replicable object and check if it is still valid.
            Object* object = world->getObjectByHandle(replicationEntry.value->objectHandle);
            if(object == nullptr)
                continue;

            Replicable* replicable = object->as<Replicable>();
            ASSERT(replicable != nullptr, "Object received from replicable registry is not replicable!");

            // Process replication data by objects.
            if(reliable)
            {
                replicable->deserializeReliableTickReplication(replicationCommand.data);
            }
            else
            {
                replicable->deserializeUnreliableTickReplication(replicationCommand.data);
            }
        }
        else if(replicationCommand.type == ReplicationCommand::ReplicationType::Destroy)
        {
            LOG_REPLICATION_TRACE("Processing replication destroy command...");

            // Fetch replication entry.
            auto replicationEntry = m_replicables.fetchHandle(replicationCommand.handle);
            if(replicationEntry.value == nullptr)
            {
                LOG_WARNING("Failed to fetch replication handle that is expect to exist!");
                continue;
            }

            // Destroy object (if it still exists).
            world->destroyObject(replicationEntry.value->objectHandle);

            // Unregister replicable object immediately.
            // Some time can pass before destroy object commands are flushed,
            // so we cannot rely on onDestroyObject() method for unregistration.
            unregisterReplicable(replicationEntry.handle);
        }
    }
}

void ReplicationClient::onObjectCreated(Object& object)
{
    // Retrieve replicable class.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr)
        return;

    // Check if object has valid replicable handle.
    // This assert is not essential. We should be able to create replicable object that will not be replicated (client side only).
    ASSERT(replicable->getReplicableHandle().isValid(), "Client attempted to create replicable object!");
    
    // Print trace.
    LOG_REPLICATION_TRACE("Replicable object has been created on client side. (%u/%u : %u/%u)",
        replicable->getHandle().getIdentifier(), replicable->getHandle().getVersion(),
        replicable->getReplicableHandle().getIdentifier(), replicable->getReplicableHandle().getVersion());
}

void ReplicationClient::onObjectDestroyed(Object& object)
{
    // Do not unregister replicable handles on object destruction.
    // If some replicable object gets prematurely destroyed by the client,
    // then the replicable entry should remain pointing at an invalid handle.
    
    // Retrieve replicable class.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr)
        return;

    // Check if replicable handle exists.
    // It may no longer be valid in cases where destroy
    // command was received before object was actually destroyed.
    auto handleReference = m_replicables.fetchHandle(replicable->getReplicableHandle());

    // Print trace.
    if(handleReference.valid)
    {
        LOG_REPLICATION_TRACE("Replicable object has been destroyed on client side. (%u/%u : %u/%u)",
            replicable->getHandle().getIdentifier(), replicable->getHandle().getVersion(),
            replicable->getReplicableHandle().getIdentifier(), replicable->getReplicableHandle().getVersion());
    }
}

void ReplicationClient::onObjectDeserialized(Object& object)
{
    // Retrieve replicable class.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr)
        return;

    // Make sure that replicable object that was deserialized has valid replication handle.
    ASSERT(replicable->getReplicableHandle().isValid(), "Deserialized replicable has invalid handle!");

    // Print trace.
    LOG_REPLICATION_TRACE("Replicable object has been deserialized on client side. (%u/%u)",
        replicable->getReplicableHandle().getIdentifier(), replicable->getReplicableHandle().getVersion());

    // Register deserialized replicable object, as we will not get replication create command for it.
    this->registerReplicable(*replicable);
}
