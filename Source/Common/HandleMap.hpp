#pragma once

#include "Precompiled.hpp"

// Forward declarations.
template<typename Type>
class HandleMap;

// Handle class.
template<typename Type>
class Handle : public Serializable
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

protected:
    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override final
    {
        if(!serialize(buffer, m_identifier))
            return false;

        if(!serialize(buffer, m_version))
            return false;

        return true;
    }

    bool onDeserialize(MemoryStream& buffer) override final
    {
        if(!deserialize(buffer, &m_identifier))
            return false;
        
        if(!deserialize(buffer, &m_version))
            return false;

        return true;
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
    using HandleValueType = typename HandleType::ValueType;
    
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
    using FreeList = std::deque<typename HandleType::ValueType>;

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
    HandleEntryRef createHandle(HandleType requestedHandle = HandleType())
    {
        // Free list entry index that we want to use.
        // Initially pointing at invalid end element and needs to be found or created.
        auto freeEntryIndex = m_freeList.end();

        // Check if requested handle identifier is in free list queue.
        if(requestedHandle.isValid())
        {
            // Find handle index in free list queue that interests us.
            freeEntryIndex = std::find_if(m_freeList.begin(), m_freeList.end(), [requestedHandle](const HandleValueType & identifier)
            {
                return identifier == requestedHandle.getIdentifier();
            });

            // Check if handle with this identifier is already in use.
            if(freeEntryIndex == m_freeList.end())
            {
                HandleValueType currentMaxIdentifier = (HandleValueType)m_handles.size();
                if(requestedHandle.getIdentifier() <= currentMaxIdentifier)
                {
                    // We are unable to create requested handle because it's already in use.
                    return HandleEntryRef();
                }
            }
        }

        // Create new handles if free list is empty or until we get the requested handle.
        while(m_freeList.empty() || requestedHandle.isValid())
        {
            // Check if we already have our requested handle.
            if(freeEntryIndex != m_freeList.end())
                break;

            // Create handle entry with new index.
            HandleValueType newHandleIdentifier = (HandleValueType)(m_handles.size() + 1);
            m_handles.emplace_back(HandleType(newHandleIdentifier));

            // Add new object entry to free list.
            std::size_t currentObjectEntryCount = m_handles.size();
            m_freeList.push_back((HandleValueType)(currentObjectEntryCount - 1));

            // Check if this is our requested identifier.
            if(requestedHandle.isValid())
            {
                if(requestedHandle.getIdentifier() == newHandleIdentifier)
                {
                    // We got our requested handle. Pop it next.
                    // We can step back using end iterator as we know free list is not empty at this point.
                    freeEntryIndex = m_freeList.end() - 1;
                    break;
                }
            }
            else
            {
                // If we want any handle, return one from the beginning of queue.
                // Removing free entires from front of queue will help keep a healthy pool of handles that will not overflow quick.
                freeEntryIndex = m_freeList.begin();
            }
        }

        // Check if we have our handle candidate.
        if(freeEntryIndex == m_freeList.end())
        {
            // We may still not have a valid free entry selected. Choose the free entry in front.
            freeEntryIndex = m_freeList.begin();
        }

        // Get free handle entry from free list queue.
        HandleEntry& handleEntry = m_handles[*freeEntryIndex];

        // Set requested handle version.
        // There is no harm in bumping handle version.
        if(requestedHandle.isValid())
        {
            ASSERT(handleEntry.handle.m_version <= requestedHandle.m_version, "Requesting handle that will result in handle reuse!");
            handleEntry.handle.m_version = requestedHandle.m_version;
        }

        // Mark handle entry as valid.
        handleEntry.valid = true;

        // Erase index from free list queue.
        m_freeList.erase(freeEntryIndex);

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
        m_freeList.push_back(handleEntry->handle.getIdentifier() - 1);

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
        if(handle.getIdentifier() <= 0 && handle.getIdentifier() > (HandleValueType)m_handles.size())
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
