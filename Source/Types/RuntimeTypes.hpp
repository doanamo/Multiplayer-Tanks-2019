#pragma once

#include "Precompiled.hpp"

class RuntimeTypes
{
public:
    // Type declarations.
    using TypeMap = std::map<TypeInfo::IdentifierType, TypeInfo*>;

public:
    RuntimeTypes();
    ~RuntimeTypes();

    // Initializes runtime type system.
    bool initialize();

    // Registers type.
    void registerType(TypeInfo& typeInfo);

    // Returns type info.
    TypeInfo* getTypeInfo(TypeInfo::IdentifierType type);

    // Prints registered types.
    void printTypes();

private:
    // Prints type list and all its children recursively.
    void recursivePrintDerived(const TypeInfo::TypeList& typeList, int depth);

private:
    // Map of runtime types.
    TypeMap m_typeMap;
};
