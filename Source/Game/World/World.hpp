#pragma once

#include "Precompiled.hpp"
#include "Common/HandleMap.hpp"
#include "Object.hpp"

// World class.
class World : public Serializable
{
public:
    // Object handle map.
    using ObjectList = HandleMap<ObjectEntry>;

    // Objects command.
    enum class ObjectCommandType
    {
        Invalid,
        Create,
        Destroy,
    };

    struct ObjectCommand
    {
        ObjectCommand() :
            handle(), type(ObjectCommandType::Invalid)
        {
        }

        ObjectHandle handle;
        ObjectCommandType type;
    };

    using CommandList = std::queue<ObjectCommand>;

    // Object name registry.
    struct ObjectNameHandle
    {
        ObjectNameHandle(std::string name) :
            name(name), handle()
        {
        }

        std::string name;
        ObjectHandle handle;
    };

    struct ObjectNameCompare
    {
        bool operator()(const ObjectNameHandle& first, const ObjectNameHandle& second) const
        {
            return first.name < second.name;
        }
    };

    using NameRegistry = std::set<ObjectNameHandle, ObjectNameCompare>;

    // Object group registry.
    using GroupEntrySet = std::set<ObjectHandle>;
    using GroupEntrySetPtr = std::unique_ptr<GroupEntrySet>;

    struct ObjectGroupHandles
    {
        ObjectGroupHandles(std::string group) :
            group(group), entryHandles(nullptr)
        {
        }

        ObjectGroupHandles(std::string group, GroupEntrySetPtr&& entrySet) :
            group(group), entryHandles(std::move(entrySet))
        {
        }

        std::string group;
        GroupEntrySetPtr entryHandles;
    };

    struct ObjectGroupCompare
    {
        bool operator()(const ObjectGroupHandles& first, const ObjectGroupHandles& second) const
        {
            return first.group < second.group;
        }
    };

    using GroupRegistry = std::set<ObjectGroupHandles, ObjectGroupCompare>;

public:
    World();
    ~World();

    // Initializes the world.
    bool initialize();

    // Update all objects in the world.
    void update(float timeDelta);

    // Ticks all objects in the world.
    void tick(float timeDelta);

    // Draws all objects in the world.
    void draw(float timeAlpha);

    // Adds object to world.
    ObjectHandle addObject(ObjectEntry::ObjectPtr&& object, std::string name = "", std::string group = "");

    // Destroys object in world.
    void destroyObject(ObjectHandle handle);

    // Processes pending objects.
    void flushObjects();

    // Sets object's unique name.
    bool setObjectName(ObjectHandle handle, std::string name, bool force = false);

    // Sets object's group name.
    void setObjectGroup(ObjectHandle handle, std::string group);

    // Gets object in world by handle.
    Object* getObjectByHandle(ObjectHandle handle);

    // Gets object in world by name.
    Object* getObjectByName(std::string name);

    // Gets objects in world by group.
    std::vector<Object*> getObjectsByGroup(std::string group);

    // Get total number of created objects.
    // Adding new object will not immediately increase this counter.
    uint32_t getObjectCount() const;

private:
    // Gets object entry by handle.
    ObjectEntry* getEntryByHandle(ObjectHandle handle);

    // Serialization methods.
    virtual bool onSerialize(MemoryStream& buffer) const override;
    virtual bool onDeserialize(MemoryStream& buffer) override;

public:
    // Public list of callbacks that other system can hook into.
    std::function<void(Object&)> replicationObjectCreated;
    std::function<void(Object&)> replicationObjectDestroyed;
    std::function<void(Object&)> replicationObjectDeserialized;

private:
    // List of commands.
    CommandList m_commands;

    // List of objects.
    ObjectList m_objects;
    uint32_t m_objectCount;

    // Registry of objects.
    NameRegistry m_names;
    GroupRegistry m_groups;
};
