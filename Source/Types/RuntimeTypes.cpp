#include "Precompiled.hpp"
#include "Types/RuntimeTypes.hpp"

RuntimeTypes::RuntimeTypes()
{
}

RuntimeTypes::~RuntimeTypes()
{
}

RuntimeTypes& RuntimeTypes::getSingletion()
{
    static RuntimeTypes singleton;
    return singleton;
}

void RuntimeTypes::registerType(TypeInfo& typeInfo)
{
    // Calculate type info hash from its name.
    TypeInfo::IdentifierType hash = stringHash(typeInfo.getName());

    // Make sure that type info with same hash is not registered twice.
    VERIFY(m_typeMap.find(hash) == m_typeMap.end(), "Type info with this hash is already registered! Possible hash collision?");

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
    for(auto& pair : m_typeMap)
    {
        // Check if element is base type.
        TypeInfo* typeInfo = pair.second;
        ASSERT(typeInfo != nullptr, "Type info map contains null element!");

        if(typeInfo->getBase() != nullptr)
            continue;

        // Print type info.
        LOG_INFO("%i : %s", typeInfo->getIdentifier(), typeInfo->getName());

        // Iterate over children.
        recursivePrintDerived(typeInfo->getDerived(), 1);
    }
}

void RuntimeTypes::recursivePrintDerived(const TypeInfo::TypeList& derivedList, int depth)
{
    // Print base types and their children.
    for(auto& derivedType : derivedList)
    {
        // Print type info.
        LOG_INDENT(depth);
        LOG_INFO("%i : %s", derivedType->getIdentifier(), derivedType->getName());

        // Iterate over children.
        recursivePrintDerived(derivedType->getDerived(), depth + 1);
    }
}
