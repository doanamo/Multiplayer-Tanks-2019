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
    std::cout << "Listing registered runtime types:" << std::endl;
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

void RuntimeTypes::recursivePrint(const std::vector<TypeInfo*>& typeList, int depth, bool baseOnly)
{
    // Print base types and their children.
    for(auto& typeInfo : typeList)
    {
        if(typeInfo->getBase() == nullptr || !baseOnly)
        {
            // Print indent.
            for(int i = 0; i < depth; ++i)
            {
                std::cout << "  ";
            }

            // Print type info.
            std::cout << typeInfo->getIdentifier() << " : " << typeInfo->getName() << std::endl;

            // Iterate over children.
            recursivePrint(typeInfo->getDerived(), depth + 1, false);
        }
    }
}
