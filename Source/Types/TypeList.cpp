#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "Types/RuntimeTypes.hpp"

#include "Game/Object.hpp"
#include "Game/Tank.hpp"
#include "Game/Projectile.hpp"

bool registerTypes()
{
    // All new runtime types should be added here!
    g_runtimeTypes->registerType(Object::Type());
    g_runtimeTypes->registerType(Tank::Type());
    g_runtimeTypes->registerType(Projectile::Type());

    return true;
}
