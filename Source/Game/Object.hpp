#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"
#include "System/Globals.hpp"

class World;

class Object
{
public:
    friend World;

public:
    Object();
    virtual ~Object();

    // Delete default copy constructors.
    Object(const Object& other) = delete;
    Object& operator=(const Object& other) = delete;

    // Mark object to be destroyed.
    void destroy();

    // Object callback virtual methods.
    virtual void onExist();
    virtual void onUpdate(float timeDelta);
    virtual void onDraw(float updateAlpha);
    virtual void onDestroy();

    // Gets handle to this object.
    Handle GetHandle() const;

    // Get world that owns this object.
    World* GetWorld() const;

private:
    // Handle to self.
    Handle m_handle;

    // Assigned world.
    World* m_world;
};
