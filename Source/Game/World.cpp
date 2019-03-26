#include "Precompiled.hpp"
#include "World.hpp"

ConsoleVariable<bool> cv_showWorldInfo("showWorldInfo", false);

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

                // Reset object name and group to remove them from registry.
                this->setObjectName(objectEntry.handle, "");
                this->setObjectGroup(objectEntry.handle, "");

                // Invalidate object handle.
                objectEntry.handle.version++;

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

    // Display world info window.
    if(cv_showWorldInfo.value)
    {
        // Function lambda for printing objects.
        auto PrintObjectEntry = [](const ObjectEntry* objectEntry)
        {
            ASSERT(objectEntry != nullptr);

            if(objectEntry->object != nullptr)
            {
                if(!objectEntry->object->getName().empty())
                {
                    ImGui::BulletText("%i/%i : %s (%s)", objectEntry->handle.identifier, objectEntry->handle.version, objectEntry->object->getType().getName(), objectEntry->object->getName().c_str());
                }
                else
                {
                    ImGui::BulletText("%i/%i : %s", objectEntry->handle.identifier, objectEntry->handle.version, objectEntry->object->getType().getName());
                }
            }
            else
            {
                ImGui::BulletText("%i/%i : -", objectEntry->handle.identifier, objectEntry->handle.version);
            }
        };

        // Draw ImGui window.
        if(ImGui::Begin("World Info", &cv_showWorldInfo.value))
        {
            ImGui::Text("List of object entries (%i total):", m_objects.size());

            int groupedEntityCount = 0;

            for(const ObjectGroupIndices& groupEntry : m_groups)
            {
                ASSERT(groupEntry.entryIndices.get() != nullptr, "Group object set is null!");

                if(ImGui::TreeNodeEx(groupEntry.group.c_str(), ImGuiTreeNodeFlags_DefaultOpen, "%s (%i)", groupEntry.group.c_str(), groupEntry.entryIndices->size()))
                {
                    const auto& objectEntries = groupEntry.entryIndices.get();

                    for(const auto& objectIndex : *objectEntries)
                    {
                        ObjectEntry& objectEntry = m_objects[objectIndex];

                        PrintObjectEntry(&objectEntry);

                        groupedEntityCount++;
                    }

                    ImGui::TreePop();
                }
            }

            if(ImGui::TreeNodeEx("Ungrouped", ImGuiTreeNodeFlags_DefaultOpen, "Ungrouped (%i)", m_objects.size() - m_freeList.size() - groupedEntityCount))
            {
                for(const ObjectEntry& objectEntry : m_objects)
                {
                    if(objectEntry.object != nullptr)
                    {
                        if(objectEntry.object->getGroup().empty())
                        {
                            PrintObjectEntry(&objectEntry);
                        }
                    }
                }

                ImGui::TreePop();
            }

            if(ImGui::TreeNodeEx("Unused", ImGuiTreeNodeFlags_DefaultOpen, "Unused (%i)", m_freeList.size()))
            {
                for(const ObjectEntry& objectEntry : m_objects)
                {
                    if(objectEntry.object == nullptr)
                    {
                        PrintObjectEntry(&objectEntry);
                    }
                }

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}

Handle World::addObject(Object* object, std::string name, std::string group)
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
    std::size_t entryIndex = m_freeList.front();
    ObjectEntry& objectEntry = m_objects[entryIndex];
    m_freeList.pop();

    ASSERT(!objectEntry.created, "Free object entry should not be marked as already created!");
    ASSERT(!objectEntry.destroy, "Free object entry should not be marked as pending destruction!");

    // Assign new object to entry.
    objectEntry.object = object;

    // Assign handle and world to new object.
    ASSERT(object->m_world == nullptr, "Object is already assigned to a world!");

    object->m_handle = objectEntry.handle;
    object->m_world = this;

    // Register object name and group.
    this->setObjectName(objectEntry.handle, name);
    this->setObjectGroup(objectEntry.handle, group);

    // Return object handle.
    return objectEntry.handle;
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

bool World::setObjectName(Handle handle, std::string name, bool force)
{
    // Get object entry if handle is valid.
    ObjectEntry* objectEntry = GetEntryByHandle(handle);

    if(objectEntry == nullptr)
    {
        LOG_WARNING("Could not set \"%s\" object name because handle is no longer valid!", name.c_str());
        return false;
    }

    // Erase current object name from name registry.
    ASSERT(objectEntry->object, "Object cannot be null if its handle is valid!");

    if(!objectEntry->object->m_name.empty())
    {
        // Find and remove object from name registry.
        auto it = m_names.find(ObjectNameIndex(objectEntry->object->m_name));
        ASSERT(it != m_names.end(), "Object with non empty name is not registered!");
        m_names.erase(it);

        // Clear current object name.
        objectEntry->object->m_name = std::string();
    }

    // Proceed with name registration if new name is not empty.
    if(name.empty())
        return true;

    // Erase existing name in registry if we want to force setting it to another object.
    if(force)
    {
        // Find object using requested name.
        auto it = m_names.find(ObjectNameIndex(name));

        if(it != m_names.end())
        {
            // Reset existing object name.
            ObjectEntry& registeredEntry = m_objects[it->entryIndex];
            ASSERT(registeredEntry.object != nullptr, "Registered object with name is null!");
            registeredEntry.object->m_name = std::string();

            // Erase entry from name registry.
            m_names.erase(it);
        }
    }

    // Register new object name.
    ObjectNameIndex objectNameIndex(name);
    objectNameIndex.entryIndex = objectEntry->handle.identifier - 1;
    
    auto result = m_names.insert(objectNameIndex);
    ASSERT(result.second || !force, "Object name insertion failed despite being forced!");

    if(result.second)
    {
        // Update object's name after it has been successfully registered.
        objectEntry->object->m_name = name;
        return true;
    }
    else
    {
        // Display warning if requested name is already in use.
        LOG_WARNING("Could not set \"%s\" object name because it is already used by existing object!", name.c_str());
        return false;
    }
}

void World::setObjectGroup(Handle handle, std::string group)
{
    // Get object entry if handle is valid.
    ObjectEntry* objectEntry = GetEntryByHandle(handle);

    if(objectEntry == nullptr)
    {
        LOG_WARNING("Could not set \"%s\" object group because handle is no longer valid!", group.c_str());
        return;
    }

    // Erase current object group from group registry.
    ASSERT(objectEntry->object, "Object cannot be null if its handle is valid!");

    if(!objectEntry->object->m_group.empty())
    {
        // Find and remove object from group registry.
        auto groupIter = m_groups.find(ObjectGroupIndices(objectEntry->object->m_group));
        ASSERT(groupIter != m_groups.end(), "Object with non empty group is not registered!");
        
        // Find object and remove it.
        GroupEntrySet* groupEntries = groupIter->entryIndices.get();
        ASSERT(groupEntries != nullptr, "Groupt entry set is null!");

        auto objectIter = groupEntries->find(objectEntry->handle.identifier - 1);
        ASSERT(objectIter != groupEntries->end(), "Object with non empty group is not registered!");
        groupEntries->erase(objectIter);

        // Remove empty group if needed.
        if(groupEntries->empty())
        {
            m_groups.erase(groupIter);
        }

        // Clear current object group.
        objectEntry->object->m_group = std::string();
    }

    // Proceed with group registration if new group is not empty.
    if(group.empty())
        return;

    // Register object group.
    auto groupResult = m_groups.insert(ObjectGroupIndices(group, std::make_unique<GroupEntrySet>()));

    // Add object to group's entry set.
    GroupEntrySet* groupEntries = groupResult.first->entryIndices.get();
    ASSERT(groupEntries != nullptr, "Group entry set is null!");

    auto entryResult = groupEntries->insert(objectEntry->handle.identifier - 1);
    ASSERT(entryResult.second, "Insertion to group entry must always succeed!");

    // Update object group.
    objectEntry->object->m_group = group;
}

World::ObjectEntry* World::GetEntryByHandle(Handle handle)
{
    // Make sure identifier is within objects array range and return null otherwise.
    if(handle.identifier <= 0 && handle.identifier > (int)m_objects.size())
        return nullptr;

    // Make sure versions are matching.
    ObjectEntry& objectEntry = m_objects[handle.identifier - 1];

    if(handle.version == objectEntry.handle.version)
    {
        return &objectEntry;
    }
    else
    {
        return nullptr;
    }
}

Object* World::getObjectByHandle(Handle handle)
{
    // Return object pointer if handle is valid.
    ObjectEntry* objectEntry = GetEntryByHandle(handle);

    if(objectEntry != nullptr)
    {
        return objectEntry->object;
    }

    return nullptr;
}

Object* World::getObjectByName(std::string name)
{
    // Find object using name registry.
    auto it = m_names.find(ObjectNameIndex(name));
    
    if(it != m_names.end())
    {
        ObjectEntry& registeredEntry = m_objects[it->entryIndex];
        ASSERT(registeredEntry.object != nullptr, "Registered object with name is null!");
        return registeredEntry.object;
    }
    else
    {
        return nullptr;
    }
}

std::vector<Object*> World::getObjectsByGroup(std::string group)
{
    std::vector<Object*> results;

    // Find objects using group registry.
    auto it = m_groups.find(ObjectGroupIndices(group));

    if(it != m_groups.end())
    {
        for(auto& objectEntryIndex : *(it->entryIndices))
        {
            ObjectEntry& objectEntry = m_objects[objectEntryIndex];
            ASSERT(objectEntry.object, "Found null object in group registry!");
            results.push_back(objectEntry.object);
        }
    }

    return results;
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

        if(!serialize(buffer, objectEntry.object->getName()))
            return false;

        if(!serialize(buffer, objectEntry.object->getGroup()))
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

        std::string name;
        if(!deserialize(buffer, &name))
            return false;

        std::string group;
        if(!deserialize(buffer, &group))
            return false;

        Object* object = Object::create(objectType);
        ASSERT(object != nullptr, "Runtime type allocation returned null!");

        if(!deserialize(buffer, object))
            return false;

        this->addObject(object, name, group);
    }

    return true;
}
