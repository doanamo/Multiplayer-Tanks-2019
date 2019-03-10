#pragma once

#include "Precompiled.hpp"

class RuntimeTypes
{
public:
    RuntimeTypes();
    ~RuntimeTypes();

    bool initialize();
    void registerType(TypeInfo& typeInfo);

private:
    // Prints type list and all its children recursively.
    void recursivePrint(const std::vector<TypeInfo*>& typeList, int depth, bool baseOnly);

private:
    //  Type counter for assigning IDs.
    TypeInfo::IdentifierType m_typeCounter;

    // List of runtime types.
    std::vector<TypeInfo*> m_typeList;
};
