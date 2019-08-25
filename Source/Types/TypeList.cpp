#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "Types/RuntimeTypes.hpp"
#include "Game/World/Object.hpp"
#include "Game/Objects/Tank.hpp"
#include "Game/Objects/Projectile.hpp"
#include "Game/Player/PlayerControllerBase.hpp"
#include "Game/Player/PlayerControllerLocal.hpp"
#include "Network/Replication/Replicable.hpp"
#include "Network/Packets/Protocol.hpp"

#define REGISTER_TYPE(type) g_runtimeTypes->registerType(type::staticTypeInfo());

bool registerTypes()
{
    // All new runtime types should be added at the end of this function!
    // Order does not have to be preserved as with previous implementation.
    ASSERT(g_runtimeTypes != nullptr);

    REGISTER_TYPE(Object);
    REGISTER_TYPE(Replicable);
    REGISTER_TYPE(Tank);
    REGISTER_TYPE(Projectile);

    REGISTER_TYPE(PlayerControllerBase);
    REGISTER_TYPE(PlayerControllerLocal);

    REGISTER_TYPE(PacketBase);
    REGISTER_TYPE(PacketMessage);
    REGISTER_TYPE(PacketRequestConnection);
    REGISTER_TYPE(PacketAcceptConnection);
    REGISTER_TYPE(PacketServerSnapshot);
    REGISTER_TYPE(PacketServerUpdate);

    return true;
}
