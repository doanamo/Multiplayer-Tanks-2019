#pragma once

#include "Precompiled.hpp"
#include "ReplicationBase.hpp"

// Forward declarations.
class PacketServerUpdate;

// Client replication class.
class ReplicationClient : public ReplicationBase
{
public:
    ReplicationClient();
    ~ReplicationClient();

    // Processes server update packet.
    void processServerUpdatePacket(const PacketServerUpdate& packet, bool reliable);

private:
    // World object callback methods.
    void onObjectCreated(Object& object) override;
    void onObjectDestroyed(Object& object) override;
    void onObjectDeserialized(Object& object) override;
};
