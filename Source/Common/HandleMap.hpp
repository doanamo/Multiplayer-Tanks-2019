#pragma once

#include "Precompiled.hpp"

// Forward declarations.
template<typename Type>
class HandleMap;

// Handle class.
template<typename Type>
class Handle
{
public:
    // Type declarations.
    using ValueType = uint32_t;

    // Friend declarations.
    friend HandleMap<Type>;

public:
    // Constructor.
    Handle() :
        m_identifier(0),
        m_version(0)
    {
    }

    // Comparison operators.
    bool operator==(const Handle<Type>& other) const
    {
        return m_identifier == other.m_identifier && m_version == other.m_version;
    }

    bool operator!=(const Handle<Type>& other) const
    {
        return m_identifier != other.m_identifier || m_version != other.m_version;
    }

    bool operator<(const Handle<Type>& other) const
    {
        return this->m_identifier < other.m_identifier;
    }

    // Accessor methods.
    ValueType getIdentifier() const
    {
        return m_identifier;
    }

    ValueType getVersion() const
    {
        return m_version;
    }

    // Checks if handle is valid.
    bool isValid() const
    {
        return m_identifier != 0;
    }

private:
    // Private constructor.
    Handle(ValueType identifier) :
        m_identifier(identifier),
        m_version(0)
    {
    }

    // Invalidates handle by increasing its version.
    void invalidate()
    {
        ASSERT(isValid(), "Invalidating invalid handle!");

        m_version++;
    }

private:
    // Identifier that can be reused.
    ValueType m_identifier;

    // Unique version for each identifier.
    ValueType m_version;
};

// Handle map class.
template<typename Type>
class HandleMap
{
public:
    // Type declarations.
    using HandleType = Handle<Type>;
    
    // Handle entry.
    struct HandleEntry
    {
        HandleEntry(const HandleType& handle) :
            handle(handle), valid(false), value()
        {
        }

        HandleType handle;
        bool valid;

        Type value;
    };

    // Handle entry reference.
    struct HandleEntryRef
    {
        HandleEntryRef() :
            handle(),
            valid(false),
            value(nullptr)
        {
        }

        HandleEntryRef(HandleEntry& reference) :
            handle(reference.handle),
            valid(reference.valid),
            value(&reference.value)
        {
        }

        const HandleType handle;
        const bool valid;
        Type* value;
    };

    // Constant handle entry reference.
    struct ConstHandleEntryRef
    {
        ConstHandleEntryRef() :
            handle(),
            valid(false),
            value(nullptr)
        {
        }

        ConstHandleEntryRef(const HandleEntry& reference) :
            handle(reference.handle),
            valid(reference.valid),
            value(&reference.value)
        {
        }

        const HandleType handle;
        const bool valid;
        const Type* value;
    };

    // Container types.
    using HandleList = std::vector<HandleEntry>;
    using FreeList = std::queue<typename HandleType::ValueType>;

    // Iterator class.
    // Forward iterator that skips over invalid handles.
    template<bool IsConst>
    class Iterator
    {
    public:
        // Type declarations.
        using IteratorType = typename std::conditional_t<IsConst, typename HandleList::const_iterator, typename HandleList::iterator>;
        using DereferenceReturnType = typename std::conditional_t<IsConst, ConstHandleEntryRef, HandleEntryRef>;

    public:
        // Constructor.
        Iterator(IteratorType it, IteratorType end) :
            m_it(it), m_end(end)
        {
            if(!isValid())
            {
                advanceUntilValid();
            }
        }

        Iterator(const Iterator& iterator) :
            m_it(iterator.m_it),
            m_end(iterator.m_end)
        {
        }

        // Iterator operators.
        Iterator& operator++()
        {
            ASSERT(m_it != m_end, "Out of bounds iteration!");
            this->advanceUntilValid();
            return *this;
        }

        Iterator operator++(int)
        {
            ASSERT(m_it != m_end, "Out of bounds iteration!");
            Iterator iterator(this);
            this->advanceUntilValid();
            return iterator;
        }

        DereferenceReturnType operator*() const
        {
            return DereferenceReturnType(*m_it);
        }

        DereferenceReturnType operator->() const
        {
            return DereferenceReturnType(*m_it);
        }

        bool operator==(const Iterator& it) const
        {
            return this->m_it == it.m_it;
        }

        bool operator!=(const Iterator& it) const
        {
            return this->m_it != it.m_it;
        }

    protected:
        // Moves iterator forward to next valid element.
        void advanceUntilValid()
        {
            while(m_it != m_end)
            {
                ++m_it;

                if(isValid())
                    break;
            }
        }

        // Checks if current iterator position is valid.
        bool isValid()
        {
            return m_it != m_end && m_it->valid;
        }

    private:
        // Iterator state.
        IteratorType m_it;
        IteratorType m_end;
    };

public:
    // Constructor.
    HandleMap()
    {
    }

    // Creates new handle.
    HandleEntryRef createHandle()
    {
        // Create new handle entry if free list is empty.
        if(m_freeList.empty())
        {
            // Create handle entry with new index.
            HandleType::ValueType newHandleIdentifier = (HandleType::ValueType)(m_handles.size() + 1);
            m_handles.emplace_back(HandleType(newHandleIdentifier));

            // Add new object entry to free list.
            std::size_t currentObjectEntryCount = m_handles.size();
            m_freeList.push((HandleType::ValueType)(currentObjectEntryCount - 1));
        }

        // Get free handle entry from free list queue.
        std::size_t entryIndex = m_freeList.front();
        HandleEntry& handleEntry = m_handles[entryIndex];
        m_freeList.pop();

        // Mark handle entry as valid.
        handleEntry.valid = true;

        // Return handle entry reference.
        return HandleEntryRef(handleEntry);
    }

    // Fetches handle entry.
    HandleEntryRef fetchHandle(HandleType handle)
    {
        HandleEntry* handleEntry = fetchHandleEntry(handle);

        if(handleEntry != nullptr)
        {
            return HandleEntryRef(*handleEntry);

        }
        else
        {
            return HandleEntryRef();
        }
    }

    // Removes existing handle.
    bool removeHandle(HandleType handle)
    {
        // Fetch handle entry.
        HandleEntry* handleEntry = fetchHandleEntry(handle);
        if(handleEntry == nullptr)
            return false;

        // Invalidate object handle.
        handleEntry->handle.invalidate();
        handleEntry->valid = false;

        // Reset handle value.
        handleEntry->value = Type();
        
        // Add entry index to free list.
        ASSERT(handleEntry->handle.getIdentifier() != 0);
        m_freeList.push(handleEntry->handle.getIdentifier() - 1);

        // Successfully removed handle.
        return true;
    }

    // Gets size of handle map.
    uint32_t getValidHandleCount() const
    {
        return (uint32_t)(m_handles.size() - m_freeList.size());
    }

    uint32_t getUnusedHandleCount() const
    {
        return (uint32_t)m_freeList.size();
    }

    // Range iterators.
    Iterator<false> begin()
    {
        return Iterator<false>(m_handles.begin(), m_handles.end());
    }

    Iterator<false> end()
    {
        return Iterator<false>(m_handles.end(), m_handles.end());
    }

    Iterator<true> begin() const
    {
        return Iterator<true>(m_handles.begin(), m_handles.end());
    }

    Iterator<true> end() const
    {
        return Iterator<true>(m_handles.end(), m_handles.end());
    }

private:
    // Fetch handle internal.
    HandleEntry* fetchHandleEntry(HandleType handle)
    {
        // Make sure identifier is within handle array's range and return null otherwise.
        if(handle.getIdentifier() <= 0 && handle.getIdentifier() > (HandleType::ValueType)m_handles.size())
            return nullptr;

        // Make sure handle versions are matching.
        HandleEntry& handleEntry = m_handles[handle.getIdentifier() - 1];

        if(handle.getVersion() == handleEntry.handle.getVersion())
        {
            return &handleEntry;
        }
        else
        {
            return nullptr;
        }
    }

private:
    // List of handles and type storage.
    HandleList m_handles;

    // List of free handles that can be reused.
    FreeList m_freeList;
};
