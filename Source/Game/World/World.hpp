#pragma once

#include "Precompiled.hpp"
#include "Handle.hpp"
#include "Object.hpp"

class World : public Serializable
{
public:
    // Object entry.
    struct ObjectEntry
    {
        ObjectEntry(int index = 0) :
            handle(index),
            object(nullptr),
            created(false),
            destroy(false)
        {
        }

        Handle handle;
        Object* object;
        bool created;
        bool destroy;
    };

    using ObjectList = std::vector<ObjectEntry>;
    using FreeList = std::queue<std::size_t>;

    // Object name registry.
    struct ObjectNameIndex
    {
        ObjectNameIndex(std::string name) :
            name(name), entryIndex(0)
        {
        }

        std::string name;
        std::size_t entryIndex;
    };

    struct ObjectNameCompare
    {
        bool operator()(const ObjectNameIndex& first, const ObjectNameIndex& second) const
        {
            return first.name < second.name;
        }
    };

    using NameRegistry = std::set<ObjectNameIndex, ObjectNameCompare>;

    // Object group registry.
    using GroupEntrySet = std::set<std::size_t>;
    using GroupEntrySetPtr = std::unique_ptr<GroupEntrySet>;

    struct ObjectGroupIndices
    {
        ObjectGroupIndices(std::string group) :
            group(group), entryIndices(nullptr)
        {
        }

        ObjectGroupIndices(std::string group, GroupEntrySetPtr&& entrySet) :
            group(group), entryIndices(std::move(entrySet))
        {
        }

        std::string group;
        GroupEntrySetPtr entryIndices;
    };

    struct ObjectGroupCompare
    {
        bool operator()(const ObjectGroupIndices& first, const ObjectGroupIndices& second) const
        {
            return first.group < second.group;
        }
    };

    using GroupRegistry = std::set<ObjectGroupIndices, ObjectGroupCompare>;

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
    Handle addObject(Object* object, std::string name = "", std::string group = "");

    // Destroys object in world.
    void destroyObject(Handle handle);

    // Processes pending objects.
    void flushObjects();

    // Sets object's unique name.
    bool setObjectName(Handle handle, std::string name, bool force = false);

    // Sets object's group name.
    void setObjectGroup(Handle handle, std::string group);

    // Gets object in world by handle.
    Object* getObjectByHandle(Handle handle);

    // Gets object in world by name.
    Object* getObjectByName(std::string name);

    // Gets objects in world by group.
    std::vector<Object*> getObjectsByGroup(std::string group);

private:
    // Gets object entry by handle.
    ObjectEntry* getEntryByHandle(Handle handle);

    // Serialization methods.
    virtual bool onSerialize(MemoryStream& buffer) const override;
    virtual bool onDeserialize(MemoryStream& buffer) override;

private:
    // List of objects currently in the world.
    ObjectList m_objects;
    FreeList m_freeList;

    // Registry of objects.
    NameRegistry m_names;
    GroupRegistry m_groups;
};
