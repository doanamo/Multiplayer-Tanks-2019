#include "Precompiled.hpp"
#include "Game/Object.hpp"
#include "Game/World.hpp"

Object::Object() :
    m_handle(),
    m_world(nullptr)
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
}

void Object::onDraw(float updateAlpha)
{
}

void Object::onDestroy()
{
}

Handle Object::GetHandle() const
{
    // Calling this before being added to a world does not make sense.
    assert(m_world != nullptr && "Attempting to get handle to an object that does not belong to a world!");

    // Return handle to self.
    return m_handle;
}

World* Object::getWorld() const
{
    // Calling this before being added to a world does not make sense.
    assert(m_world != nullptr && "Retrieving world from an object that was not added to a world!");

    // Return assigned world.
    return m_world;
}
