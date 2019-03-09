#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"
#include "Game/Transform.hpp"
#include "System/Globals.hpp"

class World;

class Object
{
protected:
    // Allow world to set handle and world reference.
    friend World;

    // Do not allow object to be instantiated directly.
    Object();

public:
    // Allow inheriting from this class.
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
    Handle getHandle() const;

    // Get world that owns this object.
    World* getWorld() const;

private:
    // Assigned world.
    World* m_world;

    // Handle to self.
    Handle m_handle;

protected:
    // Object transform.
    Transform m_transform;
};
