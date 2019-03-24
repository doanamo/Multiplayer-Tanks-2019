#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"
#include "Game/Object.hpp"

class World : public Serializable
{
public:
    // Object entry structure.
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

    // Type declarations.
    using ObjectList = std::vector<ObjectEntry>;
    using FreeList = std::queue<std::size_t>;

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

    // Object management methods.
    Handle addObject(Object* object);

    // Destroys object in world.
    void destroyObject(Handle handle);

    // Gets object in world.
    Object* getObject(Handle handle);

private:
    // Processes pending objects.
    void processPendingObjects();

    // Serialization methods.
    virtual bool onSerialize(MemoryBuffer& buffer) override;
    virtual bool onDeserialize(MemoryBuffer& buffer) override;

private:
    // List of objects currently in the world.
    ObjectList m_objects;
    FreeList m_freeList;

};
