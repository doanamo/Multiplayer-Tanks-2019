#pragma once

#include "Precompiled.hpp"
#include "Common/HandleMap.hpp"
#include "Replicable.hpp"

// Forward declarations.
class GameInstance;
class World;

// Replication class.
class Replication
{
public:
    // List of replicable network handles.
    // This maps object handles into unique network handles.
    using ReplicableList = HandleMap<ReplicableEntry>;

public:
    Replication();
    ~Replication();

    // Initializes replication.
    bool initialize(GameInstance* gameInstance);

    // Draws debug info.
    void draw();

public:
    // World callback methods for collecting replicables.
    void onObjectCreated(Object& object);
    void onObjectDestroyed(Object& object);

private:
    // World reference.
    World* m_world;

    // List of replicable objects.
    ReplicableList m_replicables;

    // Initialization state.
    bool m_initialized;
};
