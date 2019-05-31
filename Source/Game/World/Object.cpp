#include "Precompiled.hpp"
#include "Object.hpp"
#include "World.hpp"

Object::Object() :
    m_world(nullptr),
    m_handle(),
    m_name(),
    m_group(),
    m_resetInterpolation(false)
{
}

Object::~Object()
{
}

void Object::destroy()
{
    // Make sure object belongs to a world.
    ASSERT(m_world != nullptr, "Calling destroy on an object that was not added to a world!");

    // Destroy ourselves using our handle.
    m_world->destroyObject(m_handle);
}

void Object::onCreate()
{
}

void Object::onUpdate(float timeDelta)
{
    m_resetInterpolation = true;
}

void Object::onTick(float timeDelta)
{
    if(m_resetInterpolation)
    {
        // Update transform interpolation.
        m_transform.resetInterpolation();
        m_resetInterpolation = false;
    }
}

void Object::onDraw(float timeAlpha)
{
}

void Object::onDestroy()
{
}

World* Object::getWorld() const
{
    // Calling this before being added to a world does not make sense.
    ASSERT(m_world != nullptr, "Retrieving world from an object that was not added to a world!");

    // Return assigned world.
    return m_world;
}

Handle Object::getHandle() const
{
    // Calling this before being added to a world does not make sense.
    ASSERT(m_world != nullptr, "Attempting to get handle to an object that does not belong to a world!");

    // Return handle to self.
    return m_handle;
}

const std::string& Object::getName() const
{
    return m_name;
}

const std::string& Object::getGroup() const
{
    return m_group;
}

Transform& Object::getTransform()
{
    return m_transform;
}

bool Object::onSerialize(MemoryStream& buffer) const
{
    if(!serialize(buffer, m_transform))
        return false;

    return true;
}

bool Object::onDeserialize(MemoryStream& buffer)
{
    if(!deserialize(buffer, &m_transform))
        return false;

    return true;
}
