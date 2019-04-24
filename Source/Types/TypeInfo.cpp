#include "Precompiled.hpp"
#include "Types/TypeInfo.hpp"
#include "Types/RuntimeTypes.hpp"
#include "System/Globals.hpp"

static const TypeInfo::IdentifierType InvalidIdentifier = stringHash("");

TypeInfo::TypeInfo(const char* typeName, AllocateFunction allocateFunction, TypeInfo* baseType) :
    m_typeIdentifier(InvalidIdentifier),
    m_typeName(typeName),
    m_allocateFunction(allocateFunction),
    m_baseType(baseType)
{
    ASSERT(typeName != nullptr, "Type name cannot be nullptr!");
    ASSERT(allocateFunction != nullptr, "Allocation function cannot be nullptr!");

    // Add derived class to base type.
    if(m_baseType != nullptr)
    {
        ASSERT(m_baseType->getIdentifier() != InvalidIdentifier);
        m_baseType->m_derivedTypes.emplace_back(this);
    }
}

int TypeInfo::getIdentifier() const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    return m_typeIdentifier;
}

const char* TypeInfo::getName() const
{
    return m_typeName;
}

TypeInfo::AllocateFunction TypeInfo::getAllocateFunction() const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    return m_allocateFunction;
}

const TypeInfo* TypeInfo::getBase() const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    return m_baseType;
}

const std::vector<TypeInfo*>& TypeInfo::getDerived() const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    return m_derivedTypes;
}

bool TypeInfo::isSame(IdentifierType typeIdentifier) const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    return m_typeIdentifier == typeIdentifier;
}

bool TypeInfo::isBase(IdentifierType typeIdentifier) const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    ASSERT(typeIdentifier != InvalidIdentifier);

    const TypeInfo* baseType = this->getBase();

    if(baseType != nullptr)
    {
        if(baseType->getIdentifier() == typeIdentifier)
        {
            return true;
        }
        else
        {
            return baseType->isBase(typeIdentifier);
        }
    }

    return false;
}

bool TypeInfo::isDerived(IdentifierType typeIdentifier) const
{
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    ASSERT(typeIdentifier != InvalidIdentifier);

    const std::vector<TypeInfo*>& derivedTypes = this->getDerived();

    for(auto& derivedType : derivedTypes)
    {
        ASSERT(derivedType != nullptr, "Derived type list contains a nullptr!");

        if(derivedType->getIdentifier() == typeIdentifier)
        {
            return true;
        }
        else
        {
            if(derivedType->isDerived(typeIdentifier))
                return true;
        }
    }

    return false;
}

TypeInfo* TypeInfo::getTypeInfo(IdentifierType typeIdentifier)
{
    ASSERT(g_runtimeTypes != nullptr);
    ASSERT(m_typeIdentifier != InvalidIdentifier);
    ASSERT(typeIdentifier != InvalidIdentifier);

    return g_runtimeTypes->getTypeInfo(typeIdentifier);
}
