#pragma once

#include "Precompiled.hpp"

// Forward declarations.
class RuntimeTypes;

// Type info class.
class TypeInfo
{
    // Friend declarations.
    friend RuntimeTypes;

    // Identifier type.
    using IdentifierType = int;

public:
    // Type info constructor.
    TypeInfo(const char* typeName, TypeInfo* baseType = nullptr);

    // Gets unique type identifier.
    int getIdentifier() const;
    
    // Gets readable type name.
    const char* getName() const;

    // Gets reference to base type.
    const TypeInfo* getBase() const;

    // Gets list of derived types.
    const std::vector<TypeInfo*>& getDerived() const;

protected:
    // Type identifier.
    IdentifierType m_typeIdentifier;

    // Readable type name.
    const char* m_typeName;

    // Reference to base type.
    TypeInfo* m_baseType;

    // List of derived types.
    std::vector<TypeInfo*> m_derivedTypes;
};

// Utility macros.
#define TYPE_DECLARE_EXPAND(x) x
#define TYPE_DECLARE_STRINGIFY(type) #type
#define TYPE_DECLARE_DEDUCE(arg1, arg2, arg3, ...) arg3

#define TYPE_DECLARE_BASE(type) \
    public: \
        static TypeInfo& Type() \
        { \
            static TypeInfo typeInfo(TYPE_DECLARE_STRINGIFY(type)); \
            return typeInfo; \
        } \
        \
        virtual const TypeInfo& getType() const \
        { \
            return type::Type(); \
        }

#define TYPE_DECLARE_DERIVED(type, base) \
    public: \
        static TypeInfo& Type() \
        { \
            static TypeInfo typeInfo(TYPE_DECLARE_STRINGIFY(type), &base::Type()); \
            return typeInfo; \
        } \
        \
        virtual const TypeInfo& getType() const \
        { \
            return type::Type(); \
        }

#define TYPE_DECLARE_CHOOSER(...) TYPE_DECLARE_EXPAND(TYPE_DECLARE_DEDUCE(__VA_ARGS__, TYPE_DECLARE_DERIVED, TYPE_DECLARE_BASE))
#define TYPE_DECLARE(...) TYPE_DECLARE_EXPAND(TYPE_DECLARE_CHOOSER(__VA_ARGS__)(__VA_ARGS__))
