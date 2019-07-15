#include "Precompiled.hpp"
#include "Replication.hpp"
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
    - Client should not be able to create replicated objects. There should be an assert in place that will catch attempts to spawn on client.
    - Sent server snapshot does not contain replicable handles. Objects are saved one by one from the list and then recreated with different handles.
      We need to serialize network handles and HandleMap should be able to recreate handle identifier in any order (not starting sequentially from 0).
      This cam be accomplished by creating new handle identifiers and pushing them to free list until we get the one we want (slow, but we only want this once).
    - Register systems that need onReliableReplication() and onUnreliableReplication() methods called, using common interface
    - Inside onReplication() object should write type of replication and then own data directly into memory stream, just like serialization
    - Replication system that calls onReplication() on each object will first write networkObjectID (base class)
    - Replicate creation and destruction of objects as ordered reliable packets
*/

ConsoleVariable<bool> cv_showReplicationInfo("showReplicationInfo", false);

Replication::Replication() :
    m_world(nullptr),
    m_initialized(false)
{

}

Replication::~Replication()
{
    if(m_world)
    {
        m_world->replicationObjectCreated = nullptr;
        m_world->replicationObjectDestroyed = nullptr;
    }
}

bool Replication::initialize(GameInstance* gameInstance)
{
    // Retrieve world reference.
    m_world = gameInstance->getWorld();
    ASSERT(m_world != nullptr);

    // Hook callback methods in world for object creation and destruction.
    m_world->replicationObjectCreated = std::bind(&Replication::onObjectCreated, this, std::placeholders::_1);
    m_world->replicationObjectDestroyed = std::bind(&Replication::onObjectDestroyed, this, std::placeholders::_1);

    // Success!
    m_initialized = true;
    return true;
}

void Replication::onObjectCreated(Object& object)
{
    // Check if object is replicable.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr) return;

    // Create replicable object entry.
    ReplicableList::HandleEntryRef handleEntry = m_replicables.createHandle();
    ASSERT(handleEntry.value != nullptr, "Failed to create handle entry!");

    // Fill handle entry's data.
    handleEntry.value->objectHandle = replicable->getHandle();
    handleEntry.value->object = &object;

    // Fill object's replicable handle.
    replicable->m_replicableHandle = handleEntry.handle;
}

void Replication::onObjectDestroyed(Object& object)
{
    // Check if object is replicable.
    Replicable* replicable = object.as<Replicable>();
    if(replicable == nullptr) return;

    // Retrieve objects replicable handle.
    ReplicableHandle replicableHandle = replicable->m_replicableHandle;
    ASSERT(replicableHandle.isValid(), "Replicable objects has invalid replicable handle!");

    // Remove replicable object entry.
    bool result = m_replicables.removeHandle(replicableHandle);
    ASSERT(result, "Failed to remove replicable handle that should exist!");
}

void Replication::draw()
{
    // Draw debug window.
    if(cv_showReplicationInfo.value)
    {
        if(ImGui::Begin("Replication Info", &cv_showReplicationInfo.value))
        {
            ImGui::Text("List of replicable entries (%u total):", m_replicables.getValidHandleCount());

            for(auto replicableEntry : m_replicables)
            {
                ImGui::BulletText("%u/%u : %u/%u (%s)",
                    replicableEntry.handle.getIdentifier(),
                    replicableEntry.handle.getVersion(),
                    replicableEntry.value->object->getHandle().getIdentifier(),
                    replicableEntry.value->object->getHandle().getVersion(),
                    replicableEntry.value->object->getTypeInfo().getName()
                );
            }
        }
        ImGui::End();
    }
}
