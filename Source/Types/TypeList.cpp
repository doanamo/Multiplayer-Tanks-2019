#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "Types/RuntimeTypes.hpp"
#include "Game/Object.hpp"
#include "Game/Tank.hpp"
#include "Game/Projectile.hpp"
#include "Game/Level.hpp"
#include "Network/Protocol.hpp"

#define REGISTER_TYPE(type) g_runtimeTypes->registerType(type::staticTypeInfo());

bool registerTypes()
{
    // All new runtime types should be added at the end of this function!
    // Order does not have to be preserved as with previous implementation.
    ASSERT(g_runtimeTypes != nullptr);

    REGISTER_TYPE(Object);
    REGISTER_TYPE(Tank);
    REGISTER_TYPE(Projectile);

    REGISTER_TYPE(PacketBase);
    REGISTER_TYPE(PacketMessage);
    REGISTER_TYPE(PacketStateSave);

    return true;
}
