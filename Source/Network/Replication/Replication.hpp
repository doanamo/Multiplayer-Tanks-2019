#pragma once

#include "Precompiled.hpp"

// Forward declarations.
class GameInstance;

// Replication class.
class Replication
{
public:
    Replication();
    ~Replication();

    // Initializes replication.
    bool initialize(GameInstance* gameInstance);

private:
    // Initialization state.
    bool m_initialized;
};
