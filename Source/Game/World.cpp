#include "Precompiled.hpp"
#include "World.hpp"

World::World()
{
}

World::~World()
{
    // Delete all objects without calling on destroy methods.
    for(std::size_t i = 0; i < m_objects.size(); ++i)
    {
        ObjectEntry& objectEntry = m_objects[i];

        if(objectEntry.object != nullptr)
        {
            delete objectEntry.object;
        }
    }

    m_objects.clear();
}

bool World::initialize()
{
    return true;
}

void World::update(float timeDelta)
{
    // Process objects waiting for creation and destruction.
    processPendingObjects();

    // Update all objects.
    for(std::size_t i = 0; i < m_objects.size(); ++i)
    {
        ObjectEntry& objectEntry = m_objects[i];

        if(objectEntry.exists)
    
        {
            assert(objectEntry.object != nullptr && "Existing object entry does not have an object set!");

            // Call on update method.
            objectEntry.object->onUpdate(timeDelta);
        }
    }
}

void World::draw(float updateAlpha)
{
    // Draw all objects.
    for(std::size_t i = 0; i < m_objects.size(); ++i)
    {
        ObjectEntry& objectEntry = m_objects[i];

        if(objectEntry.exists)
        {
            assert(objectEntry.object != nullptr && "Existing object entry does not have an object set!");

            // Call on update method.
            objectEntry.object->onDraw(updateAlpha);
        }
    }
}

Handle World::addObject(Object* object)
{
    // Makes no sense to call this function with null.
    assert(object != nullptr && "Cannot add nullptr object!");

    // Find a free entry that we can use to store our new object.
    ObjectEntry* freeEntry = nullptr;

    for(std::size_t i = 0; i < m_objects.size(); ++i)
    {
        ObjectEntry& objectEntry = m_objects[i];

        if(objectEntry.object == nullptr)
        {
            freeEntry = &objectEntry;
            break;
        }
    }

    // Allocate a new object entry if there are no free ones.
    if(freeEntry == nullptr)
    {
        m_objects.emplace_back((int)m_objects.size());
        freeEntry = &m_objects.back();
    }

    assert(!freeEntry->exists && "Free object entry should not be marked as existing yet!");
    assert(!freeEntry->destroy && "Free object entry should not be marked as pending destruction!");

    // Increment handle version.
    freeEntry->handle.version++;

    // Assign new object to entry.
    freeEntry->object = object;

    // Assign handle and world to new object.
    assert(object->m_world == nullptr && "Object is already assigned to a world!");

    object->m_handle = freeEntry->handle;
    object->m_world = this;

    // Return object handle.
    return freeEntry->handle;
}

Object* World::getObject(Handle handle)
{
    // Make sure identifier is within objects array range and return null otherwise.
    if(handle.identifier < 0 && handle.identifier >= (int)m_objects.size())
        return nullptr;

    // Make sure versions are matching.
    ObjectEntry& objectEntry = m_objects[handle.identifier];

    if(handle.version == objectEntry.handle.version)
    {
        return objectEntry.object;
    }
    else
    {
        return nullptr;
    }
}

void World::destroyObject(Handle handle)
{
    // Make sure identifier is within objects array range and do nothing otherwise.
    if(handle.identifier < 0 && handle.identifier >= (int)m_objects.size())
        return;

    // Mark object entry for destruction.
    ObjectEntry& objectEntry = m_objects[handle.identifier];

    if(handle.version == objectEntry.handle.version)
    {
        objectEntry.destroy = true;
    }
}

void World::processPendingObjects()
{
    // Destroy objects marked for destruction.
    for(std::size_t i = 0; i < m_objects.size(); ++i)
    {
        ObjectEntry& objectEntry = m_objects[i];

        if(objectEntry.destroy)
        {
            assert(objectEntry.object != nullptr && "Object entry marked for destruction does not have an object set!");

            // Call on destroy method.
            objectEntry.object->onDestroy();

            // Delete object from memory.
            delete objectEntry.object;
            objectEntry.object = nullptr;

            // Reset object entry state.
            objectEntry.exists = false;
            objectEntry.destroy = false;
        }
    }

    // Bring created objects to existence.
    for(std::size_t i = 0; i < m_objects.size(); ++i)
    {
        ObjectEntry& objectEntry = m_objects[i];

        if(!objectEntry.exists && objectEntry.object != nullptr)
        {
            // Mark object as existing.
            objectEntry.exists = true;

            // Call on exist method.
            objectEntry.object->onExist();
        }
    }
}

bool World::onSerialize(MemoryBuffer& buffer)
{
    // Process pending objects before serialization.
    processPendingObjects();

    // Function for checking if object is eligible for serialization.
    auto ShouldSerializeObject = [](const ObjectEntry& entry)
    {
        assert(entry.exists && "Pending object were not been flushed before serialization!");
        assert(!entry.destroy && "Pending object were not been flushed before serialization!");
        return entry.object != nullptr;
    };

    // Count object that are going to be written.
    int objectCount = std::count_if(m_objects.begin(), m_objects.end(), ShouldSerializeObject);

    if(!serialize(buffer, objectCount))
        return false;

    // Serialize existing objects.
    // All objects marked for destruction should be already flushed at this point.
    // All created objects should be already marked as existing at this point.
    for(ObjectEntry& objectEntry : m_objects)
    {
        if(!ShouldSerializeObject(objectEntry))
            continue;

        if(!serialize(buffer, objectEntry.object->getType().getIdentifier()))
            return false;

        if(!serialize(buffer, *objectEntry.object))
            return false;
    }

    return true;
}

bool World::onDeserialize(MemoryBuffer& buffer)
{
    uint32_t objectCount;
    if(!deserialize(buffer, &objectCount))
        return false;

    for(uint32_t i = 0; i < objectCount; i++)
    {
        TypeInfo::IdentifierType objectType;
        if(!deserialize(buffer, &objectType))
            return false;

        Object* object = Object::create(objectType);
        if(!deserialize(buffer, object))
            return false;

        this->addObject(object);
    }

    return true;
}
