#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"
#include "Game/Transform.hpp"
#include "System/Globals.hpp"

class World;

class Object : public Serializable
{
    TYPE_DECLARE(Object);

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
    virtual void onCreate();
    virtual void onUpdate(float timeDelta);
    virtual void onTick(float timeDelta);
    virtual void onDraw(float timeAlpha);
    virtual void onDestroy();

    // Get world that owns this object.
    World* getWorld() const;

    // Gets handle to this object.
    Handle getHandle() const;

    // Gets object's unique name.
    const std::string& getName() const;

    // Gets object's group name.
    const std::string& getGroup() const;

    // Gets object's transform.
    Transform& getTransform();

protected:
    // Serialization methods.
    virtual bool onSerialize(MemoryBuffer& buffer) const override;
    virtual bool onDeserialize(MemoryBuffer& buffer) override;

private:
    // Assigned world.
    World* m_world;

    // Handle to self.
    Handle m_handle;

    // Unique name.
    std::string m_name;

    // Group name.
    std::string m_group;

    // Reset interpolation on next tick.
    bool m_resetInterpolation;

protected:
    // Object transform.
    Transform m_transform;
};
