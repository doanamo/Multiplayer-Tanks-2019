#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"
#include "Game/Object.hpp"

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
            name(name),
            entryIndex(0)
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
    struct ObjectGroupIndices
    {
        ObjectGroupIndices(std::string group) :
            group(group)
        {
        }

        std::string group;
        std::set<std::size_t> entryIndices;
    };

    struct ObjectGroupCompare
    {
        bool operator()(const ObjectGroupIndices& first, const ObjectGroupIndices& second)
        {
            return first.group < second.group;
        }
    };

    using GroupRegistry = std::set<ObjectGroupIndices>;

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

    // Sets object's unique name.
    bool setObjectName(Handle handle, std::string name, bool force = false);

    // Sets object's group name.
    void setObjectGroup(Handle handle, std::string name);

    // Gets object in world by handle.
    Object* getObjectByHandle(Handle handle);

    // Gets object in world by name.
    Object* getObjectByName(std::string name);

    // Gets objects in world by group.
    std::vector<Object*> getObjectsByGroup(std::string group);

private:
    // Processes pending objects.
    void processPendingObjects();

    // Gets object entry by handle.
    ObjectEntry* GetEntryByHandle(Handle handle);

    // Serialization methods.
    virtual bool onSerialize(MemoryBuffer& buffer) override;
    virtual bool onDeserialize(MemoryBuffer& buffer) override;

private:
    // List of objects currently in the world.
    ObjectList m_objects;
    FreeList m_freeList;

    // Registry of objects.
    NameRegistry m_names;
    GroupRegistry m_groups;
};
