#include "Precompiled.hpp"
#include "Types/TypeInfo.hpp"
#include "Types/RuntimeTypes.hpp"
#include "System/Globals.hpp"

TypeInfo::TypeInfo(const char* typeName, AllocateFunction allocateFunction, TypeInfo* baseType) :
    m_typeIdentifier(0),
    m_typeName(typeName),
    m_allocateFunction(allocateFunction),
    m_baseType(baseType)
{
    assert(allocateFunction != nullptr && "Allocation function cannot be nullptr!");

    if(m_baseType != nullptr)
    {
        assert(m_baseType->getIdentifier() != 0 && "Base type has not been registered yet!");
        m_baseType->m_derivedTypes.emplace_back(this);
    }
}

int TypeInfo::getIdentifier() const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    return m_typeIdentifier;
}

const char* TypeInfo::getName() const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    return m_typeName;
}

TypeInfo::AllocateFunction TypeInfo::getAllocateFunction() const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    return m_allocateFunction;
}

const TypeInfo* TypeInfo::getBase() const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    return m_baseType;
}

const std::vector<TypeInfo*>& TypeInfo::getDerived() const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    return m_derivedTypes;
}

bool TypeInfo::isSame(IdentifierType typeIdentifier) const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    return m_typeIdentifier == typeIdentifier;
}

bool TypeInfo::isBase(IdentifierType typeIdentifier) const
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    assert(typeIdentifier != 0 && "Type has not been registered yet!");

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
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    assert(typeIdentifier != 0 && "Type has not been registered yet!");

    const std::vector<TypeInfo*>& derivedTypes = this->getDerived();

    for(auto& derivedType : derivedTypes)
    {
        assert(derivedType != nullptr && "Derived type list contains a nullptr!");

        if(derivedType->getIdentifier() == typeIdentifier)
        {
            return true;
        }
        else
        {
            return derivedType->isDerived(typeIdentifier);
        }
    }

    return false;
}

TypeInfo* TypeInfo::getTypeInfo(IdentifierType typeIdentifier)
{
    assert(m_typeIdentifier != 0 && "Type has not been registered yet!");
    assert(typeIdentifier != 0 && "Type has not been registered yet!");

    return g_runtimeTypes->getTypeInfo(typeIdentifier);
}
