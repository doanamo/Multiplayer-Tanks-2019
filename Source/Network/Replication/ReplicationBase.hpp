#pragma once

#include "Precompiled.hpp"
#include "Common/HandleMap.hpp"
#include "ReplicationCommand.hpp"
#include "Replicable.hpp"

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
    // World callback methods for collecting replicables.
    virtual bool onObjectCreated(Object& object);
    virtual bool onObjectDestroyed(Object& object);

protected:
    // Game instance reference.
    GameInstance* m_gameInstance;

    // List of replicable objects.
    ReplicableList m_replicables;

    // Initialization state.
    bool m_initialized;
};
