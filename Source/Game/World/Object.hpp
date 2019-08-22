#pragma once

#include "Precompiled.hpp"
#include "Transform.hpp"
#include "System/Globals.hpp"
#include "Common/HandleMap.hpp"
#include "Game/Player/PlayerCommand.hpp"

// Forward declarations.
class Object;
class World;

// Object entry handle type declaration.
struct ObjectEntry
{
    using ObjectPtr = std::unique_ptr<Object>;

    ObjectEntry() :
        object(nullptr),
        created(false)
    {
    }

    ObjectPtr object;
    bool created;
};

using ObjectHandle = Handle<ObjectEntry>;

// Object class.
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
    virtual void onPlayerCommand(PlayerCommand command);
    virtual void onTick(float timeDelta);
    virtual void onUpdate(float timeDelta);
    virtual void onDraw(float timeAlpha);
    virtual void onDestroy();

    // Gets world that owns this object.
    World* getWorld() const;

    // Gets handle to this object.
    ObjectHandle getHandle() const;

    // Gets object's unique name.
    const std::string& getName() const;

    // Gets object's group name.
    const std::string& getGroup() const;

    // Gets object's transform.
    Transform& getTransform();

protected:
    // Serialization methods.
    virtual bool onSerialize(MemoryStream& buffer) const override;
    virtual bool onDeserialize(MemoryStream& buffer) override;

private:
    // Assigned world.
    World* m_world;

    // Handle to self.
    ObjectHandle m_handle;

    // Unique name.
    std::string m_name;

    // Group name.
    std::string m_group;

protected:
    // Object transform.
    Transform m_transform;

    // Reset interpolation on next tick.
    bool m_resetInterpolation;
};
