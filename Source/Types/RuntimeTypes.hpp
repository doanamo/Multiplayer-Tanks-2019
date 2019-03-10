#pragma once

#include "Precompiled.hpp"

class RuntimeTypes
{
public:
    RuntimeTypes();
    ~RuntimeTypes();

    bool initialize();

    void registerType(TypeInfo& typeInfo);
    TypeInfo* getTypeInfo(TypeInfo::IdentifierType type);

private:
    // Prints type list and all its children recursively.
    void recursivePrint(const TypeInfo::TypeList& typeList, int depth, bool baseOnly);

private:
    //  Type counter for assigning IDs.
    TypeInfo::IdentifierType m_typeCounter;

    // List of runtime types.
    TypeInfo::TypeList m_typeList;
};
