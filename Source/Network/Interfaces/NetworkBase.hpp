#pragma once

#include "Precompiled.hpp"
#include "NetworkInterface.hpp"

// Global declarations.
extern ConsoleVariable<bool> cv_showNetwork;

// Forward declarations.
class GameInstance;

// Network base class.
class NetworkBase : public NetworkInterface
{
protected:
    NetworkBase();

public:
    // Virtual destructor.
    virtual ~NetworkBase();

    // Network methods.
    bool initialize(GameInstance* gameInstance);
    virtual void update(float timeDelta) override;
    virtual void tick(float timeDelta) override;
    virtual void draw() override;

protected:
    // Game instance.
    GameInstance* m_gameInstance;
};
