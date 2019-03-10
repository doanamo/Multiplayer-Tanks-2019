#include "Precompiled.hpp"
#include "Game/Object.hpp"
#include "Game/World.hpp"

Object::Object() :
    m_world(nullptr),
    m_handle()
{
}

Object::~Object()
{
}

void Object::destroy()
{
    // Make sure object belongs to a world.
    assert(m_world != nullptr && "Calling destroy on an object that was not added to a world!");

    // Destroy ourselves using our handle.
    m_world->destroyObject(m_handle);
}

void Object::onExist()
{
}

void Object::onUpdate(float timeDelta)
{
    // Update transform interpolation.
    m_transform.updateInterpolation();
}

void Object::onDraw(float updateAlpha)
{
}

void Object::onDestroy()
{
}

World* Object::getWorld() const
{
    // Calling this before being added to a world does not make sense.
    assert(m_world != nullptr && "Retrieving world from an object that was not added to a world!");

    // Return assigned world.
    return m_world;
}

Handle Object::getHandle() const
{
    // Calling this before being added to a world does not make sense.
    assert(m_world != nullptr && "Attempting to get handle to an object that does not belong to a world!");

    // Return handle to self.
    return m_handle;
}

Transform& Object::getTransform()
{
    return m_transform;
}
