#pragma once

#include "Precompiled.hpp"

// Player command enumeration.
enum class PlayerCommand : uint16_t
{
    Invalid,

    Shoot,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,

    Count,
};

// Player command acknowledgment index type.
using PlayerCommandIndex = uint32_t;

// Indexed player command entry.
struct PlayerCommandEntry
{
    PlayerCommandIndex index;
    PlayerCommand command;
};
