#include "Precompiled.hpp"
#include "Types/RuntimeTypes.hpp"

extern bool registerTypes();

RuntimeTypes::RuntimeTypes()
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
    this->printTypes();

    return true;
}

void RuntimeTypes::registerType(TypeInfo& typeInfo)
{
    // Calculate type info hash from its name.
    TypeInfo::IdentifierType hash = stringHash(typeInfo.getName());

    // Make sure that type info with same hash is not registered twice.
    VERIFY(m_typeMap.find(hash) == m_typeMap.end(), "Type info is already registered or there is hash collision!");

    // Add type info to list of types.
    auto result = m_typeMap.emplace(hash, &typeInfo);
    typeInfo.m_typeIdentifier = hash;

    ASSERT(result.second, "Failed to register new type info!");
}

TypeInfo* RuntimeTypes::getTypeInfo(TypeInfo::IdentifierType type)
{
    // Find type info by its identifier hash.
    auto it = m_typeMap.find(type);

    if(it != m_typeMap.end())
    {
        return it->second;
    }
    else
    {
        ASSERT(false, "Failed to get type info by identifier hash!");
        return nullptr;
    }
}

void RuntimeTypes::printTypes()
{
    // Print all base types and their derived children
    LOG_INFO("Listing registered runtime types:");
    LOG_INDENT(1);

    for(auto& pair : m_typeMap)
    {
        // Check if element is base type.
        TypeInfo* typeInfo = pair.second;
        ASSERT(typeInfo != nullptr, "Type info map contains null element!");

        if(typeInfo->getBase() != nullptr)
            continue;

        // Print type info.
        LOG_INFO("%0*u : %s", 10, typeInfo->getIdentifier(), typeInfo->getName());

        // Iterate over children.
        recursivePrintDerived(typeInfo->getDerived(), 2);
    }
}

void RuntimeTypes::recursivePrintDerived(const TypeInfo::TypeList& derivedList, int depth)
{
    // Print base types and their children.
    for(auto& derivedType : derivedList)
    {
        // Print type info.
        LOG_INDENT(depth);
        LOG_INFO("%0*u : %s", 10, derivedType->getIdentifier(), derivedType->getName());

        // Iterate over children.
        recursivePrintDerived(derivedType->getDerived(), depth + 1);
    }
}
