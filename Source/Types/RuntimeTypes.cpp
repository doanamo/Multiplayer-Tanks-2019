#include "Precompiled.hpp"
#include "Types/RuntimeTypes.hpp"

extern bool registerTypes();

RuntimeTypes::RuntimeTypes() :
    m_typeCounter(0)
{
}

RuntimeTypes::~RuntimeTypes()
{
}

bool RuntimeTypes::initialize()
{
    // Register declared types.
    if(!registerTypes())
        return false;

    // List all registered types.
    LOG_INFO("Listing registered runtime types:");
    recursivePrint(m_typeList, 1, true);

    return true;
}

void RuntimeTypes::registerType(TypeInfo& typeInfo)
{
    // Make sure that type info is not being registered twice.
    assert(typeInfo.m_typeIdentifier == 0 && "Type info is already registered!");

    // Assign a new unique type info identifier.
    typeInfo.m_typeIdentifier = ++m_typeCounter;

    // Add type info to list of types.
    m_typeList.push_back(&typeInfo);
}

TypeInfo* RuntimeTypes::getTypeInfo(TypeInfo::IdentifierType type)
{
    // Make sure that type identifier is in valid range.
    assert(type > 0 && type <= m_typeCounter && "Invalid type identifier index!");

    // Return requested type identifier.
    return m_typeList[type - 1];
}

void RuntimeTypes::recursivePrint(const TypeInfo::TypeList& typeList, int depth, bool baseOnly)
{
    // Print base types and their children.
    for(auto& typeInfo : typeList)
    {
        if(typeInfo->getBase() == nullptr || !baseOnly)
        {
            // Print type info.
            LOG_INDENT(depth);
            LOG_INFO("%i : %s", typeInfo->getIdentifier(), typeInfo->getName());

            // Iterate over children.
            recursivePrint(typeInfo->getDerived(), depth + 1, false);
        }
    }
}
