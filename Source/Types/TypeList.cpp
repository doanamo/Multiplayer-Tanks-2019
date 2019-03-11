#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "Types/RuntimeTypes.hpp"

#include "Game/Object.hpp"
#include "Game/Tank.hpp"
#include "Game/Projectile.hpp"
#include "Game/Level.hpp"

bool registerTypes()
{
    // All new runtime types should be added at the end of this function!
    // Order must be preserved for serialization to remain binary compatible.
    g_runtimeTypes->registerType(Object::Type());
    g_runtimeTypes->registerType(Tank::Type());
    g_runtimeTypes->registerType(Projectile::Type());
    g_runtimeTypes->registerType(Level::Type());

    return true;
}
