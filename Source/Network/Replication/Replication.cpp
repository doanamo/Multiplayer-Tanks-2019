#include "Precompiled.hpp"
#include "Replication.hpp"

/*
    Replication Design Draft
    - Extract versioned handles functionality from World and make it as a custom container that can be used here as well
    - Objects should inherit from Replicated interface and have following methods isReplicated() and onReplication()
    - Inside onReplication() object should write type of replication and then own data directly into memory stream, just like serialization
    - Replication system that calls onReplication() on each object will first write networkObjectID
    - There should be a way for objects/systems to do both onReliableReplication() and onUnreliableReplication(), whetever they choose
    - Register world for replication and subscribe to important events such as object creation and destruction
    - Replicate creation and destruction of objects as ordered reliable packets
    - Create a replicated object map, we need our own network IDs as every client will have different object IDs
    - Register systems that need onReliableReplication() and onUnreliableReplication() methods called, using common interface
*/

Replication::Replication() :
    m_initialized(false)
{

}

Replication::~Replication()
{

}

bool Replication::initialize(GameInstance* gameInstance)
{
    // 

    // Success!
    m_initialized = true;
    return true;
}
