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
    void processServerUpdatePacket(const PacketServerUpdate& packet);

private:
    // World object callback methods.
    bool onObjectCreated(Object& object) override;
    bool onObjectDestroyed(Object& object) override;
    bool onObjectDeserialized(Object& object) override;
};
