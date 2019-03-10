#include "Precompiled.hpp"
#include "Types/TypeInfo.hpp"
#include "Types/RuntimeTypes.hpp"
#include "System/Globals.hpp"

TypeInfo::TypeInfo(const char* typeName, TypeInfo* baseType) :
    m_typeIdentifier(0),
    m_typeName(typeName),
    m_baseType(baseType)
{
    if(m_baseType != nullptr)
    {
        assert(m_baseType->getIdentifier() != 0 && "Base type has not been initialized yet!");
        m_baseType->m_derivedTypes.emplace_back(this);
    }
}

int TypeInfo::getIdentifier() const
{
    assert(m_typeIdentifier != 0 && "Type has not been initialized yet!");
    return m_typeIdentifier;
}

const char* TypeInfo::getName() const
{
    assert(m_typeIdentifier != 0 && "Type has not been initialized yet!");
    return m_typeName;
}

const TypeInfo* TypeInfo::getBase() const
{
    assert(m_typeIdentifier != 0 && "Type has not been initialized yet!");
    return m_baseType;
}

const std::vector<TypeInfo*>& TypeInfo::getDerived() const
{
    assert(m_typeIdentifier != 0 && "Type has not been initialized yet!");
    return m_derivedTypes;
}
