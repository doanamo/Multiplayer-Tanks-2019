#include "Precompiled.hpp"
#include "World.hpp"

World::World()
{
}

World::~World()
{
    // Delete all objects without calling on destroy methods.
    for(ObjectEntry& objectEntry : m_objects)
    {
        if(objectEntry.object != nullptr)
        {
            delete objectEntry.object;
        }
    }
}

bool World::initialize()
{
    return true;
}

void World::processPendingObjects()
{
    // Destroy objects marked for destruction.
    {
        std::size_t index = 0;
        while(index < m_objects.size())
        {
            ObjectEntry& objectEntry = m_objects[index];

            if(objectEntry.destroy)
            {
                ASSERT(objectEntry.object != nullptr, "Object entry marked for destruction does not have an object set!");

                // Call on destroy method.
                objectEntry.object->onDestroy();

                // Delete object from memory.
                delete objectEntry.object;
                objectEntry.object = nullptr;

                // Reset object entry state.
                objectEntry.created = false;
                objectEntry.destroy = false;

                // Add entry index to free list.
                m_freeList.push(index);
            }

            index++;
        }
    }
    
    // Mark objects added last frame as created.
    {
        std::size_t index = 0;
        while(index < m_objects.size())
        {
            ObjectEntry& objectEntry = m_objects[index];

            if(!objectEntry.created && objectEntry.object != nullptr)
            {
                // Mark object as created.
                objectEntry.created = true;

                // Call on create method.
                objectEntry.object->onCreate();
            }

            index++;
        }
    }
}

void World::update(float timeDelta)
{
    // Update all objects.
    std::size_t index = 0;
    while(index < m_objects.size())
    {
        ObjectEntry& objectEntry = m_objects[index++];

        if(objectEntry.created)
        {
            ASSERT(objectEntry.object != nullptr, "Created object entry does not have an object set!");

            // Call on update method.
            objectEntry.object->onUpdate(timeDelta);
        }
    }
}

void World::tick(float timeDelta)
{
    // Process objects waiting for creation and destruction.
    processPendingObjects();

    // Tick all objects.
    std::size_t index = 0;
    while(index < m_objects.size())
    {
        ObjectEntry& objectEntry = m_objects[index++];

        if(objectEntry.created)
        {
            ASSERT(objectEntry.object != nullptr, "Created object entry does not have an object set!");

            // Call on tick method.
            objectEntry.object->onTick(timeDelta);
        }
    }
}

void World::draw(float timeAlpha)
{
    // Draw all objects.
    std::size_t index = 0;
    while(index < m_objects.size())
    {
        ObjectEntry& objectEntry = m_objects[index++];

        if(objectEntry.created)
        {
            ASSERT(objectEntry.object != nullptr, "Created object entry does not have an object set!");

            // Call on update method.
            objectEntry.object->onDraw(timeAlpha);
        }
    }
}

Handle World::addObject(Object* object)
{
    // Makes no sense to call this function with null.
    ASSERT(object != nullptr, "Cannot add nullptr object!");

    // Create new object entry if free list is empty.
    if(m_freeList.empty())
    {
        int handleIndex = (int)m_objects.size() + 1;
        m_objects.emplace_back(handleIndex);
        m_freeList.push(m_objects.size() - 1);
    }

    // Get free object entry from free list queue.
    ObjectEntry& freeEntry = m_objects[m_freeList.front()];
    m_freeList.pop();

    ASSERT(!freeEntry.created, "Free object entry should not be marked as already created!");
    ASSERT(!freeEntry.destroy, "Free object entry should not be marked as pending destruction!");

    // Increment handle version.
    freeEntry.handle.version++;

    // Assign new object to entry.
    freeEntry.object = object;

    // Assign handle and world to new object.
    ASSERT(object->m_world == nullptr, "Object is already assigned to a world!");

    object->m_handle = freeEntry.handle;
    object->m_world = this;

    // Return object handle.
    return freeEntry.handle;
}

void World::destroyObject(Handle handle)
{
    // Make sure identifier is within objects array range and do nothing otherwise.
    if(handle.identifier <= 0 && handle.identifier > (int)m_objects.size())
        return;

    // Mark object entry for destruction.
    ObjectEntry& objectEntry = m_objects[handle.identifier - 1];

    if(handle.version == objectEntry.handle.version)
    {
        objectEntry.destroy = true;
    }
}

Object* World::getObject(Handle handle)
{
    // Make sure identifier is within objects array range and return null otherwise.
    if(handle.identifier <= 0 && handle.identifier > (int)m_objects.size())
        return nullptr;

    // Make sure versions are matching.
    ObjectEntry& objectEntry = m_objects[handle.identifier - 1];

    if(handle.version == objectEntry.handle.version)
    {
        return objectEntry.object;
    }
    else
    {
        return nullptr;
    }
}

bool World::onSerialize(MemoryBuffer& buffer)
{
    // Process pending objects before serialization.
    processPendingObjects();

    // Function for checking if object is eligible for serialization.
    auto ShouldSerializeObject = [](const ObjectEntry& entry)
    {
        return entry.object != nullptr;
    };

    // Count object that are going to be written.
    int objectCount = (int)std::count_if(m_objects.begin(), m_objects.end(), ShouldSerializeObject);

    if(!serialize(buffer, objectCount))
        return false;

    // Serialize created objects.
    // All objects marked for destruction should be already flushed at this point.
    // All created objects should be already marked as created at this point.
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
        ASSERT(object != nullptr, "Runtime type allocation returned null!");

        if(!deserialize(buffer, object))
            return false;

        this->addObject(object);
    }

    return true;
}
