#include "Precompiled.hpp"
#include "ReplicationBase.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

/*
    Replication Design Draft
    - DONE: Extract versioned handles functionality from World and make it as a custom container that can be used here as well
    - DONE: Implement registration and unregistration of replicable objects in Replication system
    - DONE: Objects should inherit from Replicated interface and have following methods isReplicated() and onReplication()
    - DONE: There should be a way for objects/systems to do both onReliableReplication() and onUnreliableReplication(), whetever they choose
    - DONE: Register world for replication and subscribe to important events such as object creation and destruction
    - DONE: Create a replicated object map, we need our own network IDs as every client will have different object IDs
    - DONE: Serialize network handles in world serialization, along with objects? Not ideal. Make replication system do that. But how do refer back to those objects???
      Make serializable interface serialize/deserialize that. But how do we put those back in replication system? In OnCreatw!!!! :D:D:D
    - DONE: Sent server snapshot does not contain replicable handles. Objects are saved one by one from the list and then recreated with different handles.
      We need to serialize network handles and HandleMap should be able to recreate handle identifier in any order (not starting sequentially from 0).
      This cam be accomplished by creating new handle identifiers and pushing them to free list until we get the one we want (slow, but we only want this once).
    - Implement sending of gathered commands (we want replication form server to client to work at minimum, will not be playable).
    - Client should not be able to create replicated objects. There should be an assert in place that will catch attempts to spawn on client.
    - Sending create/destroy (always reliable) and tick (either reliable or unreliable) commands to clients.
    - Register systems that need onReliableReplication() and onUnreliableReplication() methods called, using common interface
    - Inside onReplication() object should write type of replication and then own data directly into memory stream, just like serialization.
    - Replication system that calls onReplication() on each object will first write networkObjectID (base class).
    - Replicate creation and destruction of objects as ordered reliable packets.
    - One way or either way replication. What if player object is updated and simulated on client, and server only checks validity.
      This is very complex, as we have to ensure that we are perfectly syncing between frames.
    - Latency measurement and calculating packet delay.-
    - Based on latency, buffer packets and run deterministic simulation (e.g. why send projectile destroy, if we know it collided at this frame).
    - Preventing some logic from running on clients, e.g. dealing damage. Handled by DamageSystem? Some systems should not exist on clients?
      What about objects that destroy and modify themselves?
*/

ConsoleVariable<bool> cv_showReplicationInfo("showReplicationInfo", false);

ReplicationBase::ReplicationBase() :
    m_gameInstance(nullptr),
    m_initialized(false)
{
}

ReplicationBase::~ReplicationBase()
{
    LOG_REPLICATION_TRACE("Calling ReplicationBase destructor.");

    // Unregister callbacks.
    if(m_gameInstance && m_gameInstance->getWorld())
    {
        m_gameInstance->getWorld()->replicationObjectCreated = nullptr;
        m_gameInstance->getWorld()->replicationObjectDestroyed = nullptr;
    }
}

bool ReplicationBase::initialize(GameInstance* gameInstance)
{
    LOG_REPLICATION_TRACE("Calling ReplicationBase initialization.");

    // Save game instance reference.
    m_gameInstance = gameInstance;
    ASSERT(m_gameInstance);

    // Hook callback methods in world for object creation and destruction.
    m_gameInstance->getWorld()->replicationObjectCreated = std::bind(&ReplicationBase::onObjectCreated, this, std::placeholders::_1);
    m_gameInstance->getWorld()->replicationObjectDestroyed = std::bind(&ReplicationBase::onObjectDestroyed, this, std::placeholders::_1);
    m_gameInstance->getWorld()->replicationObjectDeserialized = std::bind(&ReplicationBase::onObjectDeserialized, this, std::placeholders::_1);

    // Success!
    m_initialized = true;
    return true;
}

void ReplicationBase::registerReplicable(Replicable& replicable)
{
    LOG_REPLICATION_TRACE("Registering replicable object... (%u/%u : %u/%u)",
        replicable.getHandle().getIdentifier(), replicable.getHandle().getVersion(),
        replicable.getReplicableHandle().getIdentifier(), replicable.getReplicableHandle().getVersion());

    // Check if object has valid handle.
    // This can occur when prematurely registering object that has not been added to world yet.
    ASSERT(replicable.getHandle().isValid(), "Replicable object does not have a valid handle!");
    
    // Create replicable object entry.
    ReplicableHandle requestedHandle = replicable.getReplicableHandle();
    ReplicableList::HandleEntryRef replicableEntry = m_replicables.createHandle(requestedHandle);
    ASSERT(replicableEntry.value != nullptr, "Failed to create handle entry!");

    // Fill replicable entry data.
    replicableEntry.value->objectHandle = replicable.getHandle();

    // Set object replicable handle, unless it already had one after being deserialized.
    if(!replicable.getReplicableHandle().isValid())
    {
        setReplicableHandle(replicable, replicableEntry.handle);
    }
}

void ReplicationBase::unregisterReplicable(const ReplicableHandle& handle)
{   
    LOG_REPLICATION_TRACE("Unregistering replicable handle... (%u/%u)",
        handle.getIdentifier(), handle.getVersion());

    // Retrieve objects replicable handle.
    ASSERT(handle.isValid(), "Replicable objects has invalid replicable handle!");

    // Remove replicable object entry.
    bool result = m_replicables.removeHandle(handle);
    ASSERT(result, "Could not remove replicable handle!");
}

bool ReplicationBase::onObjectCreated(Object& object)
{
    return true;
}

bool ReplicationBase::onObjectDestroyed(Object& object)
{
    return true;
}

bool ReplicationBase::onObjectDeserialized(Object& object)
{
    return true;
}

void ReplicationBase::setReplicableHandle(Replicable& replicable, const ReplicableHandle& handle)
{
    LOG_REPLICATION_TRACE("Set replicable handle for object. (%u/%u : %u/%u)",
        replicable.getHandle().getIdentifier(), replicable.getHandle().getVersion(),
        handle.getIdentifier(), handle.getVersion());

    replicable.m_replicableHandle = handle;
}

void ReplicationBase::draw()
{
    // Draw debug window.
    if(cv_showReplicationInfo.value)
    {
        if(ImGui::Begin("Replication Info", &cv_showReplicationInfo.value))
        {
            ImGui::Text("List of replicable entries (%u total):", m_replicables.getValidHandleCount());

            for(auto replicableEntry : m_replicables)
            {
                // Retrieve object bound to replicable entry (it can be null).
                Object* object = m_gameInstance->getWorld()->getObjectByHandle(replicableEntry.value->objectHandle);

                // Write replicable entry.
                ImGui::BulletText("%u/%u : %u/%u (%s)",
                    replicableEntry.handle.getIdentifier(),
                    replicableEntry.handle.getVersion(),
                    replicableEntry.value->objectHandle.getIdentifier(),
                    replicableEntry.value->objectHandle.getVersion(),
                    object != nullptr ? object->getTypeInfo().getName() : "Invalid"
                );
            }
        }
        ImGui::End();
    }
}
