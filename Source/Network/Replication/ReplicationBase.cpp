#include "Precompiled.hpp"
#include "ReplicationBase.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

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
    if(m_gameInstance)
    {
        m_gameInstance->getWorld().replicationObjectCreated = nullptr;
        m_gameInstance->getWorld().replicationObjectDestroyed = nullptr;
    }
}

bool ReplicationBase::initialize(GameInstance* gameInstance)
{
    LOG_REPLICATION_TRACE("Calling ReplicationBase initialization.");

    // Save game instance reference.
    m_gameInstance = gameInstance;
    ASSERT(m_gameInstance);

    // Hook callback methods in world for object creation and destruction.
    m_gameInstance->getWorld().replicationObjectCreated = std::bind(&ReplicationBase::onObjectCreated, this, std::placeholders::_1);
    m_gameInstance->getWorld().replicationObjectDestroyed = std::bind(&ReplicationBase::onObjectDestroyed, this, std::placeholders::_1);
    m_gameInstance->getWorld().replicationObjectDeserialized = std::bind(&ReplicationBase::onObjectDeserialized, this, std::placeholders::_1);

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

void ReplicationBase::onObjectCreated(Object& object)
{
}

void ReplicationBase::onObjectDestroyed(Object& object)
{
}

void ReplicationBase::onObjectDeserialized(Object& object)
{
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
                Object* object = m_gameInstance->getWorld().getObjectByHandle(replicableEntry.value->objectHandle);

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
