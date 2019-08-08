#include "Precompiled.hpp"
#include "World.hpp"

#define ENABLE_WORLD_LOG_TRACE false

#if ENABLE_WORLD_LOG_TRACE
    #define LOG_WORLD_TRACE(format, ...) LOG_TRACE(format, ## __VA_ARGS__)
#else
    #define LOG_WORLD_TRACE(format, ...)
#endif

ConsoleVariable<bool> cv_showWorldInfo("showWorldInfo", false);

World::World() :
    m_objectCount(0)
{
}

World::~World()
{
    LOG_WORLD_TRACE("Destructing world...");
}

bool World::initialize()
{
    LOG_WORLD_TRACE("Initializing world...");

    // Success!
    return true;
}

void World::flushObjects()
{
    LOG_WORLD_TRACE("Flushing world objects...");

    // Iterate over list of commands.
    // Additional commands can be added during processing of objects.
    // For this reason we make copies of the list and process them until it is empty after all iterations.
    int currentIteration = 0;

    while(!m_commands.empty())
    {
        // Check iteration count.
        ++currentIteration;

        ASSERT(currentIteration <= 100, "Infinite loop detected! Maximum iteration of object processing loop has been reached!")

        // Copy and empty command list.
        CommandList commands;
        commands.swap(m_commands);

        // Iterate over list of copied commands.
        while(!commands.empty())
        {
            // Pop command from queue.
            ObjectCommand& command = commands.front();
            commands.pop();

            // Resolve object handle.
            ObjectEntry* objectEntry = getEntryByHandle(command.handle);
            if(objectEntry == nullptr)
                continue;

            // Check object state.
            ASSERT(objectEntry->object != nullptr, "Object entry pointed by command has null object!");
            ASSERT(objectEntry->object->getHandle() == command.handle, "Object handle mismatch!");

            // Process commands sequentially.
            switch(command.type)
            {
            case ObjectCommandType::Invalid:
                ASSERT(false, "Invalid command type!");
                break;

            case ObjectCommandType::Create:
                {
                    // Increment number of objects.
                    ++m_objectCount;

                    // Call on create method.
                    objectEntry->object->onCreate();

                    // Mark object as created.
                    objectEntry->created = true;

                    // Notify replication system about created object.
                    if(replicationObjectCreated != nullptr)
                    {
                        replicationObjectCreated(*objectEntry->object);
                    }

                    LOG_WORLD_TRACE("Processed create object command. (%u/%u)",
                        command.handle.getIdentifier(), command.handle.getVersion());
                }
                break;

            case ObjectCommandType::Destroy:
                {
                    // Call on destroy method.
                    objectEntry->object->onDestroy();

                    // Notify replication system about destroyed object.
                    if(replicationObjectDestroyed != nullptr)
                    {
                        replicationObjectDestroyed(*objectEntry->object);
                    }

                    // Reset object name and group to remove them from registry.
                    this->setObjectName(command.handle, "");
                    this->setObjectGroup(command.handle, "");

                    // Remove object from handle map.
                    bool result = m_objects.removeHandle(command.handle);
                    ASSERT(result, "Failed to remove object handle that should exist!");

                    // Decrement object count.
                    --m_objectCount;

                    LOG_WORLD_TRACE("Processed destroy object command. (%u/%u)",
                        command.handle.getIdentifier(), command.handle.getVersion());
                }
                break;
            }
        }
    }
}

void World::update(float timeDelta)
{
    LOG_WORLD_TRACE("Updating world....");

    // Update all objects.
    for(auto handleEntry : m_objects)
    {
        ASSERT(handleEntry.value != nullptr);
        ObjectEntry& objectEntry = *handleEntry.value;

        if(objectEntry.created)
        {
            // Call on update method.
            ASSERT(objectEntry.object != nullptr, "Created object entry does not have an object set!");
            objectEntry.object->onUpdate(timeDelta);
        }
    }
}

void World::tick(float timeDelta)
{
    LOG_WORLD_TRACE("Ticking world...");

    // Tick all objects.
    for(auto handleEntry : m_objects)
    {
        ASSERT(handleEntry.value != nullptr);
        ObjectEntry& objectEntry = *handleEntry.value;

        if(objectEntry.created)
        {
            // Call on tick method.
            ASSERT(objectEntry.object != nullptr, "Created object entry does not have an object set!");
            objectEntry.object->onTick(timeDelta);
        }
    }

    // Process objects waiting for creation and destruction.
    // Do this in tick so object commands get processed once per frame.
    flushObjects();
}

void World::draw(float timeAlpha)
{
    LOG_WORLD_TRACE("Drawing world...");

    // Draw all objects.
    for(auto handleEntry : m_objects)
    {
        ASSERT(handleEntry.value != nullptr);
        ObjectEntry& objectEntry = *handleEntry.value;

        if(objectEntry.created)
        {
            // Call on update method.
            ASSERT(objectEntry.object != nullptr, "Created object entry does not have an object set!");
            objectEntry.object->onDraw(timeAlpha);
        }
    }

    // Display world info window.
    if(cv_showWorldInfo.value)
    {
        // Function lambda for printing objects.
        auto PrintObjectEntry = [](const ObjectEntry& objectEntry)
        {
            ASSERT(objectEntry.object != nullptr);

            if(!objectEntry.object->getName().empty())
            {
                ImGui::BulletText("%u/%u : %s (%s)",
                    objectEntry.object->getHandle().getIdentifier(),
                    objectEntry.object->getHandle().getVersion(),
                    objectEntry.object->getTypeInfo().getName(),
                    objectEntry.object->getName().c_str());
            }
            else
            {
                ImGui::BulletText("%u/%u : %s",
                    objectEntry.object->getHandle().getIdentifier(),
                    objectEntry.object->getHandle().getVersion(),
                    objectEntry.object->getTypeInfo().getName());
            }
        };

        // Draw ImGui window.
        if(ImGui::Begin("World Info", &cv_showWorldInfo.value))
        {
            ImGui::Text("List of object entries (%u total):", m_objectCount);

            int groupedObjectCount = 0;

            for(const ObjectGroupHandles& groupEntry : m_groups)
            {
                ASSERT(groupEntry.entryHandles.get() != nullptr, "Group object set is null!");

                if(ImGui::TreeNodeEx(groupEntry.group.c_str(), ImGuiTreeNodeFlags_DefaultOpen, "%s (%i)", groupEntry.group.c_str(), groupEntry.entryHandles->size()))
                {
                    const auto& objectEntries = groupEntry.entryHandles.get();

                    for(const auto& objectHandle : *objectEntries)
                    {
                        ObjectList::HandleEntryRef handleEntry = m_objects.fetchHandle(objectHandle);

                        ASSERT(handleEntry.value != nullptr);
                        ObjectEntry& objectEntry = *handleEntry.value;

                        PrintObjectEntry(objectEntry);

                        groupedObjectCount++;
                    }

                    ImGui::TreePop();
                }
            }

            if(ImGui::TreeNodeEx("Ungrouped", ImGuiTreeNodeFlags_DefaultOpen, "Ungrouped (%i)", m_objects.getValidHandleCount() - groupedObjectCount))
            {
                for(auto handleEntry : m_objects)
                {
                    ASSERT(handleEntry.value != nullptr);
                    ObjectEntry& objectEntry = *handleEntry.value;

                    if(objectEntry.object != nullptr)
                    {
                        if(objectEntry.object->getGroup().empty())
                        {
                            PrintObjectEntry(objectEntry);
                        }
                    }
                }

                ImGui::TreePop();
            }

            if(ImGui::TreeNodeEx("Unused", ImGuiTreeNodeFlags_DefaultOpen, "Unused (%i)", m_objects.getUnusedHandleCount()))
            {
                // #todo: Print unused handles.
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}

ObjectHandle World::addObject(ObjectEntry::ObjectPtr&& object, std::string name, std::string group)
{
    // Makes no sense to call this function with null.
    ASSERT(object != nullptr, "Cannot add nullptr object!");

    // Create new handle.
    ObjectList::HandleEntryRef handleEntry = m_objects.createHandle();
    ASSERT(handleEntry.value != nullptr, "Failed to create handle entry!");

    // Retrieve object entry.
    ObjectEntry& objectEntry = *handleEntry.value;
    ASSERT(!objectEntry.object, "Invalid state of new object entry!");
    ASSERT(!objectEntry.created, "Invalid state of new object entry!");

    // Assign new object to entry.
    objectEntry.object = std::move(object);

    // Assign handle and world to new object.
    ASSERT(objectEntry.object->m_world == nullptr, "Object is already assigned to a world!");

    objectEntry.object->m_handle = handleEntry.handle;
    objectEntry.object->m_world = this;

    // Register object name and group.
    this->setObjectName(handleEntry.handle, name);
    this->setObjectGroup(handleEntry.handle, group);

    // Add command to create object.
    ObjectCommand command;
    command.type = ObjectCommandType::Create;
    command.handle = handleEntry.handle;
    m_commands.push(command);

    // Return object handle.
    LOG_WORLD_TRACE("Added world object. (%u/%u)",
        handleEntry.handle.getIdentifier(), handleEntry.handle.getVersion());

    return handleEntry.handle;
}

void World::destroyObject(ObjectHandle handle)
{
    // Add command to destroy object.
    ObjectCommand command;
    command.type = ObjectCommandType::Destroy;
    command.handle = handle;
    m_commands.push(command);

    LOG_WORLD_TRACE("Destroyed world object. (%u/%u)",
        handle.getIdentifier(), handle.getVersion());
}

bool World::setObjectName(ObjectHandle handle, std::string name, bool force)
{
    // Get object entry if handle is valid.
    ObjectEntry* objectEntry = getEntryByHandle(handle);

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
        auto it = m_names.find(ObjectNameHandle(objectEntry->object->m_name));
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
        auto it = m_names.find(ObjectNameHandle(name));

        if(it != m_names.end())
        {
            // Reset existing object name.
            ObjectList::HandleEntryRef handleEntry = m_objects.fetchHandle(it->handle);

            ASSERT(handleEntry.value != nullptr);
            ObjectEntry& objectEntry = *handleEntry.value;

            ASSERT(objectEntry.object != nullptr);
            objectEntry.object->m_name = std::string();

            // Erase entry from name registry.
            m_names.erase(it);
        }
    }

    // Register new object name.
    ObjectNameHandle objectNameHandle(name);
    objectNameHandle.handle = handle;
    
    auto result = m_names.emplace(objectNameHandle);
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

void World::setObjectGroup(ObjectHandle handle, std::string group)
{
    // Get object entry if handle is valid.
    ObjectEntry* objectEntry = getEntryByHandle(handle);

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
        auto groupIter = m_groups.find(ObjectGroupHandles(objectEntry->object->m_group));
        ASSERT(groupIter != m_groups.end(), "Object with non empty group is not registered!");
        
        // Find object and remove it.
        GroupEntrySet* groupEntries = groupIter->entryHandles.get();
        ASSERT(groupEntries != nullptr, "Groupt entry set is null!");

        auto objectIter = groupEntries->find(handle);
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
    auto groupResult = m_groups.insert(ObjectGroupHandles(group, std::make_unique<GroupEntrySet>()));

    // Add object to group's entry set.
    GroupEntrySet* groupEntries = groupResult.first->entryHandles.get();
    ASSERT(groupEntries != nullptr, "Group entry set is null!");

    auto entryResult = groupEntries->insert(handle);
    ASSERT(entryResult.second, "Insertion to group entry must always succeed!");

    // Update object group.
    objectEntry->object->m_group = group;
}

ObjectEntry* World::getEntryByHandle(ObjectHandle handle)
{
    // Return object entry if it exists.
    ObjectList::HandleEntryRef handleEntry = m_objects.fetchHandle(handle);
    return handleEntry.value;
}

Object* World::getObjectByHandle(ObjectHandle handle)
{
    // Return object pointer if handle is valid.
    ObjectEntry* objectEntry = getEntryByHandle(handle);

    if(objectEntry != nullptr)
    {
        return objectEntry->object.get();
    }

    return nullptr;
}

Object* World::getObjectByName(std::string name)
{
    // Find object using name registry.
    auto it = m_names.find(ObjectNameHandle(name));
    
    if(it != m_names.end())
    {
        ObjectList::HandleEntryRef handleEntry = m_objects.fetchHandle(it->handle);

        ASSERT(handleEntry.value != nullptr);
        ObjectEntry& objectEntry = *handleEntry.value;

        ASSERT(objectEntry.object != nullptr);
        return objectEntry.object.get();
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
    auto it = m_groups.find(ObjectGroupHandles(group));

    if(it != m_groups.end())
    {
        for(auto& objectEntryHandle : *(it->entryHandles))
        {
            ObjectList::HandleEntryRef handleEntry = m_objects.fetchHandle(objectEntryHandle);

            ASSERT(handleEntry.value != nullptr);
            ObjectEntry & objectEntry = *handleEntry.value;

            ASSERT(objectEntry.object != nullptr);
            results.push_back(objectEntry.object.get());
        }
    }

    return results;
}

uint32_t World::getObjectCount() const
{
    return m_objectCount;
}

bool World::onSerialize(MemoryStream& buffer) const
{
    // Check if objects are in valid state for serialization.
    // Call flushObjects() before serialization to avoid failures.
    ASSERT(m_commands.empty(), "Cannot serialize while command list is not empty!");

    // Write valid object count.
    if(!serialize(buffer, m_objects.getValidHandleCount()))
        return false;

    // Serialize created objects.
    // All objects marked for destruction should be already flushed at this point.
    // All created objects should be already marked as created at this point.
    for(const auto handleEntry : m_objects)
    {
        ASSERT(handleEntry.value != nullptr);
        const ObjectEntry& objectEntry = *handleEntry.value;

        ASSERT(objectEntry.object != nullptr);
        ASSERT(objectEntry.created == true);

        if(!serialize(buffer, getTypeIdentifier(*objectEntry.object)))
            return false;

        if(!serialize(buffer, objectEntry.object->getName()))
            return false;

        if(!serialize(buffer, objectEntry.object->getGroup()))
            return false;

        if(!serialize(buffer, *objectEntry.object))
            return false;
    }

    // Success!
    LOG_WORLD_TRACE("Serialized world to memory.");
    return true;
}

bool World::onDeserialize(MemoryStream& buffer)
{
    // Deserialize and add new objects,
    uint32_t objectCount;
    if(!deserialize(buffer, &objectCount))
        return false;

    for(uint32_t i = 0; i < objectCount; i++)
    {
        // Create and deserialize object.
        TypeInfo::IdentifierType objectType;
        if(!deserialize(buffer, &objectType))
            return false;

        std::string name;
        if(!deserialize(buffer, &name))
            return false;

        std::string group;
        if(!deserialize(buffer, &group))
            return false;

        std::unique_ptr<Object> object(Object::create(objectType));
        ASSERT(object != nullptr, "Runtime type allocation returned null!");

        if(!deserialize(buffer, object.get()))
            return false;

        // Add object to world.
        auto objectHandle = this->addObject(std::move(object), name, group);

        // Retrieve added object back.
        Object* addedObject = this->getObjectByHandle(objectHandle);
        ASSERT(addedObject != nullptr, "Failed to retrieve just added object!");

        // Notify replication system about deserialized object.
        // Replication system will have to register it.
        replicationObjectDeserialized(*addedObject);
    }

    // Success!
    LOG_WORLD_TRACE("Deserialized world from memory.");
    return true;
}
