#pragma once

#include "Precompiled.hpp"
#include "Common/HandleMap.hpp"
#include "ReplicationCommand.hpp"
#include "Replicable.hpp"

// Debugging trace.
#define ENABLE_REPLICATION_LOG_TRACE false

#if ENABLE_REPLICATION_LOG_TRACE
    #define LOG_REPLICATION_TRACE(format, ...) LOG_TRACE(format, ## __VA_ARGS__)
#else
    #define LOG_REPLICATION_TRACE(format, ...)
#endif

// Forward declarations.
class GameInstance;
class World;
class NetworkBase;

// Replication class.
class ReplicationBase
{
public:
    // List of replicable network handles.
    // This maps object handles into unique network handles.
    using ReplicableList = HandleMap<ReplicableEntry>;

public:
    ReplicationBase();
    virtual ~ReplicationBase();

    // Initializes replication.
    bool initialize(GameInstance* gameInstance);

    // Draws debug info.
    void draw();

protected:
    // Sets replicable handle for replicable object.
    void setReplicableHandle(Replicable& replicable, const ReplicableHandle& handle);

    // Replicable registration and unregistration methods.
    void registerReplicable(Replicable& replicable);
    void unregisterReplicable(const ReplicableHandle& handle);
    
    // World callback methods for collecting replicables.
    virtual void onObjectCreated(Object& object);
    virtual void onObjectDestroyed(Object& object);
    virtual void onObjectDeserialized(Object& object);

protected:
    // Game instance reference.
    GameInstance* m_gameInstance;

    // List of replicable objects.
    // Replicable handles should be in sync between server and client.
    // Objects pointed by replicable handles may not always be valid on client side (e.g. when prematurely destroyed).
    ReplicableList m_replicables;

    // Initialization state.
    bool m_initialized;
};
