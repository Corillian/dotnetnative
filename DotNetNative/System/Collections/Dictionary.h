#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_DICTIONARY_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_DICTIONARY_H_

#include "IDictionary.h"
#include "../Array.h"
#include "IEqualityComparer.h"
#include "../../MemoryUtil.h"
#include "../Exception.h"
#include "HashHelpers.h"
#include "EqualityComparer.h"

#include <cassert>

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename TKey, typename TValue>
            class Dictionary
                : public virtual Object
                , public virtual IDictionary<TKey, TValue>
            {
            private:
                struct Entry
                {
                    // 0-based index of next entry in chain: -1 means end of chain
                    // also encodes whether this entry _itself_ is part of the free list by changing sign and subtracting 3,
                    // so -2 means end of free list, -3 means index 0 but on free list, -4 means index 1 but on free list, etc.
                    int                        m_next;
                    uint32_t                   m_hashCode;
                    KeyValuePair<TKey, TValue> m_pair;
                };

                /// <summary>
                /// Used internally to control behavior of insertion into a <see cref="Dictionary{TKey, TValue}"/>.
                /// </summary>
                enum class InsertionBehavior
                {
                    /// <summary>
                    /// The default insertion behavior.
                    /// </summary>
                    None = 0,

                    /// <summary>
                    /// Specifies that an existing entry with the same key should be overwritten if encountered.
                    /// </summary>
                    OverwriteExisting = 1,

                    /// <summary>
                    /// Specifies that if an existing entry with the same key is encountered, an exception should be thrown.
                    /// </summary>
                    ThrowOnExisting = 2
                };

                class KeyValuePairEnumerator
                    : public virtual Object
                    , public IEnumerator<KeyValuePair<TKey, TValue>>
                {
                private:
                    const Dictionary<TKey, TValue>  *m_dictionary;
                    Dictionary<TKey, TValue>::Entry *m_currentValue;
                    int                              m_index;
                    int                              m_cookie;

                public:
                    KeyValuePairEnumerator(const Dictionary<TKey, TValue> *dictionary);

                    virtual const KeyValuePair<TKey, TValue>& Current() const & override;
                    virtual KeyValuePair<TKey, TValue>& Current() & override;
                    virtual bool MoveNext() override;
                    virtual void Reset() override;

                    virtual String ToString() override { return String("System.Collections.Dictionary`2.KeyValuePairEnumerator"); }
                };

                class KeyEnumerator
                    : public virtual Object
                    , public IEnumerator<TKey>
                {
                private:
                    KeyValuePairEnumerator m_enumerator;

                public:
                    KeyEnumerator(const Dictionary<TKey, TValue> *dictionary) : m_enumerator(dictionary) {}

                    virtual const TKey& Current() const & override { return m_enumerator.Current().Key(); }
                    virtual TKey& Current() & override { throw NotSupportedException(); }
                    virtual bool MoveNext() override { return m_enumerator.MoveNext(); }
                    virtual void Reset() override { m_enumerator.Reset(); }

                    virtual String ToString() override { return String("System.Collections.Dictionary`2.KeyEnumerator"); }
                };

                class ValueEnumerator
                    : public virtual Object
                    , public IEnumerator<TValue>
                {
                private:
                    KeyValuePairEnumerator m_enumerator;

                public:
                    ValueEnumerator(const Dictionary<TKey, TValue> *dictionary) : m_enumerator(dictionary) {}

                    virtual const TValue& Current() const & override { return m_enumerator.Current().Value(); }
                    virtual TValue& Current() & override { return m_enumerator.Current().Value(); }
                    virtual bool MoveNext() override { return m_enumerator.MoveNext(); }
                    virtual void Reset() override { m_enumerator.Reset(); }

                    virtual String ToString() override { return String("System.Collections.Dictionary`2.ValueEnumerator"); }
                };

                class KeyCollection
                    : public virtual Object
                    , public virtual IReadOnlyCollection<TKey>
                {
                private:
                    const Dictionary<TKey, TValue> *m_dictionary;

                public:
                    inline KeyCollection(const Dictionary<TKey, TValue> *dictionary) : m_dictionary(dictionary) {}
                    virtual ~KeyCollection() {}

                    virtual String ToString() override { return String("System.Collections.Dictionary`2.KeyCollection"); }
                    virtual int64_t Count() const override { return m_dictionary->Count(); }

                    virtual unique_ptr<IEnumerator<TKey>> GetEnumerator() override;
                };

                class ValueCollection
                    : public virtual Object
                    , public virtual IReadOnlyCollection<TValue>
                {
                private:
                    const Dictionary<TKey, TValue> *m_dictionary;

                public:
                    inline ValueCollection(const Dictionary<TKey, TValue> *dictionary) : m_dictionary(dictionary) {}
                    virtual ~ValueCollection() {}

                    virtual String ToString() override { return String("System.Collections.Dictionary`2.ValueCollection"); }
                    virtual int64_t Count() const override { return m_dictionary->Count(); }

                    virtual unique_ptr<IEnumerator<TValue>> GetEnumerator() override;
                };

            private:
                static constexpr int64_t StartOfFreeList = -3;

                unique_ptr<Array<Entry>>            m_entries;
                unique_ptr<Array<int>>              m_buckets;
                shared_ptr<IEqualityComparer<TKey>> m_comparer;
                int                                 m_count;
                int                                 m_freeList;
                int                                 m_freeCount;
                int                                 m_version;

            private:
                int Initialize(const int capacity);
                int FindEntry(const TKey &key) const;
                void Resize();
                void Resize(const int newSize, const bool forceNewHashCodes);
                bool TryInsert(const TKey &key, const TValue &value, const InsertionBehavior behavior);
                bool TryInsert(TKey &&key, TValue &&value, const InsertionBehavior behavior);

            public:
                Dictionary() : Dictionary(0, nullptr) {}
                Dictionary(const int capacity) : Dictionary(capacity, nullptr) {}
                Dictionary(const shared_ptr<IEqualityComparer<TKey>> &comparer) : Dictionary(0, comparer) {}
                Dictionary(const int capacity, const shared_ptr<IEqualityComparer<TKey>> &comparer);
                Dictionary(IEnumerable<KeyValuePair<TKey, TValue>> &collection, const shared_ptr<IEqualityComparer<TKey>> &comparer);
                Dictionary(IEnumerable<KeyValuePair<TKey, TValue>> &collection) : Dictionary(collection, nullptr) {}
                Dictionary(const Dictionary<TKey, TValue> &copy);
                Dictionary(const IDictionary<TKey, TValue> &copy) : Dictionary(copy, nullptr) {}
                Dictionary(const IDictionary<TKey, TValue> &copy, const shared_ptr<IEqualityComparer<TKey>> &comparer);
                Dictionary(Dictionary<TKey, TValue> &&mov) noexcept;
                virtual ~Dictionary() {}

                virtual TValue& operator[](const TKey &key) override;
                virtual const TValue& operator[](const TKey &key) const override;

                Dictionary<TKey, TValue>& operator=(const Dictionary<TKey, TValue> &copy);
                Dictionary<TKey, TValue>& operator=(Dictionary<TKey, TValue> &&mov) noexcept;

                bool TryAdd(const TKey &key, const TValue &value);
                bool TryAdd(TKey &&key, TValue &&value);
                virtual void Add(const TKey &key, const TValue &value) override;
                virtual void Add(TKey &&key, TValue &&value) override;
                virtual bool Remove(const TKey &key) override;
                bool Remove(const TKey &key, TValue &outValue);
                virtual bool ContainsKey(const TKey &key) const override;
                virtual bool TryGetValue(const TKey &key, const TValue **outValue) const override;

                virtual unique_ptr<IReadOnlyCollection<TKey>> Keys() const override;
                virtual unique_ptr<IReadOnlyCollection<TValue>> Values() const override;

                virtual int64_t Count() const override { return m_count - m_freeCount; }

                virtual bool IsReadOnly() const override { return false; }

                virtual void Add(const KeyValuePair<TKey, TValue> &item) override;
                virtual void Add(KeyValuePair<TKey, TValue> &&item) override;

                virtual void Clear() override;

                virtual bool Contains(const KeyValuePair<TKey, TValue> &item) const noexcept override;

                // CopyTo copies a collection into an Array, starting at a particular
                // index into the array.
                // 
                virtual void CopyTo(Array<KeyValuePair<TKey, TValue>> &arr, const int arrayIndex) const override;

                virtual bool Remove(const KeyValuePair<TKey, TValue> &item) override { return Remove(item.Key()); }

                virtual unique_ptr<IEnumerator<KeyValuePair<TKey, TValue>>> GetEnumerator() override;

                /// <summary>
                /// Sets the capacity of this dictionary to what it would be if it had been originally initialized with all its entries
                /// 
                /// This method can be used to minimize the memory overhead 
                /// once it is known that no new elements will be added. 
                /// 
                /// To allocate minimum size storage array, execute the following statements:
                /// 
                /// dictionary.Clear();
                /// dictionary.TrimExcess();
                /// </summary>
                inline void TrimExcess() { TrimExcess(static_cast<int>(this->Count())); }
                void TrimExcess(const int capacity);

                bool ContainsValue(const TValue &value) const;

                /// <summary>
                /// Ensures that the dictionary can hold up to 'capacity' entries without any further expansion of its backing storage.
                /// </summary>
                int EnsureCapacity(const int capacity);

                inline const shared_ptr<IEqualityComparer<TKey>>& Comparer() const noexcept { return m_comparer; }
            };

            ///////////////////////////////////////// Dictionary /////////////////////////////////////////

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>::Dictionary(const int capacity, const shared_ptr<IEqualityComparer<TKey>> &comparer)
                : m_count(0)
                , m_freeList(0)
                , m_freeCount(0)
                , m_version(0)
            {
                if(capacity < 0)
                {
                    throw ArgumentOutOfRangeException("capacity");
                }

                if(capacity > 0)
                {
                    Initialize(capacity);
                }

                if(!comparer)
                {
                    m_comparer = DNN_make_shared(GenericEqualityComparer<TKey>);
                }
            }

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>::Dictionary(const IDictionary<TKey, TValue> &copy, const shared_ptr<IEqualityComparer<TKey>> &comparer)
                : Dictionary(static_cast<int>(copy.Count()), comparer)
            {
                unique_ptr<IEnumerator<KeyValuePair<TKey, TValue>>> iter = copy.GetEnumerator();

                while(iter->MoveNext())
                {
                    Add(iter->Current());
                }
            }

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>::Dictionary(IEnumerable<KeyValuePair<TKey, TValue>> &collection, const shared_ptr<IEqualityComparer<TKey>> &comparer)
                : Dictionary(static_cast<int>(collection.Count()), comparer)
            {
                unique_ptr<IEnumerator<KeyValuePair<TKey, TValue>>> iter = collection.GetEnumerator();

                while(iter->MoveNext())
                {
                    Add(iter->Current());
                }
            }

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>::Dictionary(const Dictionary<TKey, TValue> &copy)
                : m_entries(copy.m_entries ? DNN_New Array<Entry>(*copy.m_entries) : nullptr)
                , m_buckets(copy.m_buckets ? DNN_make_unique(Array<int>, *mov.m_buckets) : nullptr)
                , m_comparer(copy.m_comparer)
                , m_count(copy.m_count)
                , m_freeList(copy.m_freeList)
                , m_freeCount(copy.m_freeCount)
                , m_version(0)
            {
            }

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>::Dictionary(Dictionary<TKey, TValue> &&mov) noexcept
                : m_entries(std::move(mov.m_entries))
                , m_buckets(std::move(mov.m_buckets))
                , m_comparer(std::move(mov.m_comparer))
                , m_count(mov.m_count)
                , m_freeList(mov.m_freeList)
                , m_freeCount(mov.m_freeCount)
                , m_version(0)
            {
                mov.m_count = 0;
                mov.m_freeList = -1;
                mov.m_freeCount = 0;
                ++mov.m_version;
            }

            template <typename TKey, typename TValue>
            int Dictionary<TKey, TValue>::Initialize(const int capacity)
            {
                const int size = HashHelpers::GetPrime(capacity);

                m_entries = unique_ptr<Array<Entry>>(DNN_New Array<Entry>(size));
                m_buckets = DNN_make_unique(Array<int>, size);
                m_freeList = -1;

                return size;
            }

            template <typename TKey, typename TValue>
            int Dictionary<TKey, TValue>::FindEntry(const TKey &key) const
            {
                int i = -1;
                Array<int> *buckets = m_buckets.get();
                Array<Entry> *entries = m_entries.get();
                int collisionCount = 0;

                if(buckets)
                {
                    assert(entries);

                    IEqualityComparer<TKey> *comparer = m_comparer.get();

                    assert(comparer);

                    const uint32_t hashCode = static_cast<uint32_t>(comparer->GetHashCode(key));

                    // Value in _buckets is 1-based
                    i = (*buckets)[hashCode % static_cast<uint32_t>(buckets->Length())] - 1;

                    do
                    {
                        // Should be a while loop https://github.com/dotnet/coreclr/issues/15476
                        // Test in if to drop range check for following array access
                        if(static_cast<uint32_t>(i) >= static_cast<uint32_t>(entries->Length())
                            || ((*entries)[i].m_hashCode == hashCode && comparer->Equals((*entries)[i].m_pair.Key(), key)))
                        {
                            break;
                        }

                        i = (*entries)[i].m_next;

                        if(collisionCount >= entries->Length())
                        {
                            // The chain of entries forms a loop; which means a concurrent update has happened.
                            // Break out of the loop and throw, rather than looping forever.
                            throw InvalidOperationException("Operation not supported.");
                        }

                        collisionCount++;

                    } while(true);
                }

                return i;
            }

            template <typename TKey, typename TValue>
            int Dictionary<TKey, TValue>::EnsureCapacity(const int capacity)
            {
                if(capacity < 0)
                {
                    throw ArgumentOutOfRangeException("capacity");
                }

                int currentCapacity = m_entries == nullptr ? 0 : m_entries->Length();

                if(currentCapacity >= capacity)
                {
                    return currentCapacity;
                }

                ++m_version;

                if(!m_buckets)
                {
                    return Initialize(capacity);
                }

                const int newSize = HashHelpers.GetPrime(capacity);

                Resize(newSize, false);

                return newSize;
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Resize()
            {
                Resize(HashHelpers::ExpandPrime(m_count), false);
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Resize(const int newSize, const bool forceNewHashCodes)
            {
                // Value types never rehash
                assert(!forceNewHashCodes);
                assert(m_entries != nullptr);
                assert(newSize >= m_entries->Length());

                unique_ptr<Array<int>> bucketsPtr = DNN_make_unique(Array<int>, newSize);
                unique_ptr<Array<Entry>> entriesPtr = DNN_make_unique(Array<Entry>, newSize);

                Array<Entry>::Copy(*m_entries, 0, *entriesPtr, 0, m_count);

                Array<Entry> &entries = *entriesPtr;
                Array<int> &buckets = *bucketsPtr;

                if(forceNewHashCodes) // TODO-NULLABLE: default(T) == null warning (https://github.com/dotnet/roslyn/issues/34757)
                {
                    for(int i = 0; i < m_count; ++i)
                    {
                        if(entries[i].m_next >= -1)
                        {
                            //assert(m_comparer == nullptr);
                            entries[i].m_hashCode = static_cast<uint32_t>(Object::GetHashCode<TKey>(entries[i].m_pair.Key()));
                        }
                    }
                }

                for(int i = 0; i < m_count; i++)
                {
                    if(entries[i].m_next >= -1)
                    {
                        uint32_t bucket = entries[i].m_hashCode % static_cast<uint32_t>(newSize);

                        // Value in _buckets is 1-based
                        entries[i].m_next = buckets[bucket] - 1;

                        // Value in _buckets is 1-based
                        buckets[bucket] = i + 1;
                    }
                }

                m_buckets = std::move(bucketsPtr);
                m_entries = std::move(entriesPtr);
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::TryInsert(const TKey &key, const TValue &value, const InsertionBehavior behavior)
            {
                if(!m_buckets)
                {
                    Initialize(0);
                }

                assert(m_buckets != nullptr);

                assert(m_entries != nullptr);

                Array<Entry> &entries = *m_entries;
                Array<int> &buckets = *m_buckets;

                const uint32_t hashCode = static_cast<uint32_t>(m_comparer->GetHashCode(key)); // TODO-NULLABLE: Remove ! when [DoesNotReturn] respected

                int collisionCount = 0;
                int *bucket = &buckets[hashCode % static_cast<uint32_t>(m_buckets->Length())];

                // Value in m_buckets is 1-based
                int i = *bucket - 1;

                do
                {
                    // Should be a while loop https://github.com/dotnet/coreclr/issues/15476
                    // Test uint in if rather than loop condition to drop range check for following array access
                    if(static_cast<uint32_t>(i) >= static_cast<uint32_t>(m_entries->Length()))
                    {
                        break;
                    }

                    if(entries[i].m_hashCode == hashCode && m_comparer->Equals(entries[i].m_pair.Key(), key))
                    {
                        if(behavior == InsertionBehavior::OverwriteExisting)
                        {
                            entries[i].m_pair.Value() = value;
                            ++m_version;
                            return true;
                        }

                        if(behavior == InsertionBehavior::ThrowOnExisting)
                        {
                            throw ArgumentException("Duplicate key.");
                        }

                        return false;
                    }

                    i = entries[i].m_next;

                    if(collisionCount >= m_entries->Length())
                    {
                        // The chain of entries forms a loop; which means a concurrent update has happened.
                        // Break out of the loop and throw, rather than looping forever.
                        throw InvalidOperationException("Concurrent operations not supported.");
                    }

                    ++collisionCount;

                } while(true);

                bool updateFreeList = false;
                int index;

                if(m_freeCount > 0)
                {
                    index = m_freeList;
                    updateFreeList = true;
                    m_freeCount--;
                }
                else
                {
                    if(m_count == m_entries->Length())
                    {
                        Resize();
                        bucket = &buckets[hashCode % static_cast<uint32_t>(m_buckets->Length())];
                    }

                    index = m_count;
                    m_count += 1;
                }

                Entry *entry = &entries[index];

                if(updateFreeList)
                {
                    assert((StartOfFreeList - entries[m_freeList].m_next) >= -1);

                    m_freeList = StartOfFreeList - entries[m_freeList].m_next;
                }

                entry->m_hashCode = hashCode;
                // Value in _buckets is 1-based
                entry->m_next = *bucket - 1;
                entry->m_pair = KeyValuePair<TKey, TValue>(key, value);
                // Value in _buckets is 1-based
                *bucket = index + 1;
                ++m_version;

                // Value types never rehash
                /*if(default(TKey)!== null && collisionCount > HashHelpers.HashCollisionThreshold && comparer is NonRandomizedStringEqualityComparer) // TODO-NULLABLE: default(T) == null warning (https://github.com/dotnet/roslyn/issues/34757)
                {
                    // If we hit the collision threshold we'll need to switch to the comparer which is using randomized string hashing
                    // i.e. EqualityComparer<string>.Default.
                    _comparer = null;
                    Resize(entries.Length, true);
                }*/

                return true;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::TryInsert(TKey &&key, TValue &&value, const InsertionBehavior behavior)
            {
                if(!m_buckets)
                {
                    Initialize(0);
                }

                assert(m_buckets != nullptr);

                assert(m_entries != nullptr);

                Array<Entry> &entries = *m_entries;
                Array<int> &buckets = *m_buckets;

                const uint32_t hashCode = static_cast<uint32_t>(m_comparer->GetHashCode(key)); // TODO-NULLABLE: Remove ! when [DoesNotReturn] respected

                int collisionCount = 0;
                int *bucket = &buckets[hashCode % static_cast<uint32_t>(m_buckets->Length())];

                // Value in m_buckets is 1-based
                int i = *bucket - 1;

                do
                {
                    // Should be a while loop https://github.com/dotnet/coreclr/issues/15476
                    // Test uint in if rather than loop condition to drop range check for following array access
                    if(static_cast<uint32_t>(i) >= static_cast<uint32_t>(m_entries->Length()))
                    {
                        break;
                    }

                    if(entries[i].m_hashCode == hashCode && m_comparer->Equals(entries[i].m_pair.Key(), key))
                    {
                        if(behavior == InsertionBehavior::OverwriteExisting)
                        {
                            entries[i].m_pair.Value() = std::move(value);
                            ++m_version;
                            return true;
                        }

                        if(behavior == InsertionBehavior::ThrowOnExisting)
                        {
                            throw ArgumentException("Duplicate key.");
                        }

                        return false;
                    }

                    i = entries[i].m_next;

                    if(collisionCount >= m_entries->Length())
                    {
                        // The chain of entries forms a loop; which means a concurrent update has happened.
                        // Break out of the loop and throw, rather than looping forever.
                        throw InvalidOperationException("Concurrent operations not supported.");
                    }

                    ++collisionCount;

                } while(true);

                bool updateFreeList = false;
                int index;

                if(m_freeCount > 0)
                {
                    index = m_freeList;
                    updateFreeList = true;
                    m_freeCount--;
                }
                else
                {
                    if(m_count == m_entries->Length())
                    {
                        Resize();
                        bucket = &buckets[hashCode % static_cast<uint32_t>(m_buckets->Length())];
                    }

                    index = m_count;
                    m_count += 1;
                }

                Entry *entry = &entries[index];

                if(updateFreeList)
                {
                    assert((StartOfFreeList - entries[m_freeList].m_next) >= -1);

                    m_freeList = StartOfFreeList - entries[m_freeList].m_next;
                }

                entry->m_hashCode = hashCode;
                // Value in _buckets is 1-based
                entry->m_next = *bucket - 1;
                entry->m_pair = KeyValuePair<TKey, TValue>(std::move(key), std::move(value));
                // Value in _buckets is 1-based
                *bucket = index + 1;
                ++m_version;

                // Value types never rehash
                /*if(default(TKey)!== null && collisionCount > HashHelpers.HashCollisionThreshold && comparer is NonRandomizedStringEqualityComparer) // TODO-NULLABLE: default(T) == null warning (https://github.com/dotnet/roslyn/issues/34757)
                {
                    // If we hit the collision threshold we'll need to switch to the comparer which is using randomized string hashing
                    // i.e. EqualityComparer<string>.Default.
                    _comparer = null;
                    Resize(entries.Length, true);
                }*/

                return true;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::Remove(const TKey &key)
            {
                int collisionCount = 0;

                if(m_buckets)
                {
                    assert(m_entries);

                    Array<int> &buckets = *m_buckets;
                    Array<Entry> &entries = *m_entries;
                    
                    const uint32_t hashCode = static_cast<uint32_t>(m_comparer->GetHashCode(key)); // TODO-NULLABLE: Remove ! when [DoesNotReturn] respected
                    const uint32_t bucket = hashCode % static_cast<uint32_t>(buckets.Length());
                    int last = -1;

                    // Value in buckets is 1-based
                    int i = buckets[bucket] - 1;

                    while(i >= 0)
                    {
                        Entry &entry = entries[i];

                        if(entry.m_hashCode == hashCode && m_comparer->Equals(entry.m_pair.Key(), key))
                        {
                            if(last < 0)
                            {
                                // Value in buckets is 1-based
                                buckets[bucket] = entry.m_next + 1;
                            }
                            else
                            {
                                entries[last].m_next = entry.m_next;
                            }

                            assert((StartOfFreeList - m_freeList) < 0);

                            entry.m_next = StartOfFreeList - m_freeList;

                            if(!std::is_trivially_destructible<TValue>::value)
                            {
                                entry.m_pair.~KeyValuePair<TKey, TValue>();
                                new (&entry.m_pair) KeyValuePair<TKey, TValue>();
                            }

                            m_freeList = i;
                            ++m_freeCount;

                            return true;
                        }

                        last = i;
                        i = entry.m_next;
                        
                        if(collisionCount >= entries.Length())
                        {
                            // The chain of entries forms a loop; which means a concurrent update has happened.
                            // Break out of the loop and throw, rather than looping forever.
                            throw InvalidOperationException("Concurrent operations not supported.");
                        }

                        ++collisionCount;
                    }
                }

                return false;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::Remove(const TKey &key, TValue &outValue)
            {
                int collisionCount = 0;

                if(m_buckets)
                {
                    assert(m_entries);

                    Array<int> &buckets = *m_buckets;
                    Array<Entry> &entries = *m_entries;

                    const uint32_t hashCode = static_cast<uint>(m_comparer->GetHashCode(key)); // TODO-NULLABLE: Remove ! when [DoesNotReturn] respected
                    const uint32_t bucket = hashCode % static_cast<uint32_t>(buckets.Length());
                    int last = -1;

                    // Value in buckets is 1-based
                    int i = buckets[bucket] - 1;

                    while(i >= 0)
                    {
                        Entry &entry = entries[i];

                        if(entry.m_hashCode == hashCode && m_comparer->Equals(entry.key, key))
                        {
                            if(last < 0)
                            {
                                // Value in buckets is 1-based
                                buckets[bucket] = entry.m_next + 1;
                            }
                            else
                            {
                                entries[last].m_next = entry.m_next;
                            }

                            outValue = std::move(entry.m_pair.Value());

                            assert((StartOfFreeList - m_freeList) < 0);

                            entry.m_next = StartOfFreeList - m_freeList;

                            m_freeList = i;
                            m_freeCount++;

                            return true;
                        }

                        last = i;
                        i = entry.m_next;

                        if(collisionCount >= entries.Length())
                        {
                            // The chain of entries forms a loop; which means a concurrent update has happened.
                            // Break out of the loop and throw, rather than looping forever.
                            throw InvalidOperationException("Concurrent operations not supported.");
                        }

                        ++collisionCount;
                    }
                }

                return false;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::TryGetValue(const TKey &key, const TValue **outValue) const
            {
                if(!outValue)
                {
                    throw ArgumentNullException("outValue");
                }

                const int i = FindEntry(key);

                if(i >= 0)
                {
                    *outValue = &(*m_entries)[i].m_pair.Value();
                    return true;
                }

                *outValue = nullptr;
                
                return false;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::TryAdd(const TKey &key, const TValue &value)
            {
                return TryInsert(key, value, InsertionBehavior::None);
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::TryAdd(TKey &&key, TValue &&value)
            {
                return TryInsert(std::move(key), std::move(value), InsertionBehavior::None);
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::TrimExcess(const int capacity)
            {
                if(capacity < this->Count())
                {
                    throw ArgumentOutOfRangeException("capacity");
                }
                    
                const int newSize = HashHelpers.GetPrime(capacity);
                const int currentCapacity = m_entries ? 0 : static_cast<int>(m_entries->Length());

                if(newSize >= currentCapacity)
                {
                    return;
                }

                const int oldCount = m_count;
                unique_ptr<Array<Entry>> oldEntries = std::move(m_entries);

                ++m_version;

                Initialize(newSize);

                int count = 0;

                for(int i = 0; i < oldCount; i++)
                {
                    const uint32_t hashCode = (*oldEntries)[i].m_hashCode; // At this point, we know we have entries.

                    if((*oldEntries)[i].m_next >= -1)
                    {
                        Entry &entry = (*m_entries)[count];
                        entry = std::move((*oldEntries)[i]);

                        const uint32_t bucket = hashCode % static_Cast<uint32_t>(newSize);

                        // Value in _buckets is 1-based
                        entry.m_next = (*m_buckets)[bucket] - 1; // If we get here, we have entries, therefore buckets is not null.

                        // Value in _buckets is 1-based
                        (*m_buckets)[bucket] = count + 1;
                        count++;
                    }
                }

                m_count = count;
                m_freeCount = 0;
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Add(const TKey &key, const TValue &value)
            {
                bool modified = TryInsert(key, value, InsertionBehavior::ThrowOnExisting);

                assert(modified);
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Add(TKey &&key, TValue &&value)
            {
                bool modified = TryInsert(std::move(key), std::move(value), InsertionBehavior::ThrowOnExisting);

                assert(modified);
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Add(const KeyValuePair<TKey, TValue> &item)
            {
                bool modified = TryInsert(item.Key(), item.Value(), InsertionBehavior::ThrowOnExisting);

                assert(modified);
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Add(KeyValuePair<TKey, TValue> &&item)
            {
                TKey key = item.Key();

                bool modified = TryInsert(std::move(key), std::move(item.Value()), InsertionBehavior::ThrowOnExisting);

                assert(modified);
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::Clear()
            {
                if(m_count > 0)
                {
                    assert(m_buckets);
                    assert(m_entries);

                    Array<int>::Clear(*m_buckets, 0, m_buckets->Length());
                    Array<Entry>::Clear(*m_entries, 0, m_count);

                    m_count = 0;
                    m_freeList = -1;
                    m_freeCount = 0;
                }
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::ContainsKey(const TKey &key) const
            {
                return FindEntry(key) >= 0;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::ContainsValue(const TValue &value) const
            {
                if(m_count == 0)
                {
                    return false;
                }

                assert(m_entries);

                const Array<Entry> &entries = *m_entries;
                GenericEqualityComparer<TValue> valueComparer;

                for(int i = 0; i < m_count; ++i)
                {
                    const Entry &entry = entries[i];

                    if(entry.m_next >= -1 && valueComparer.Equals(entry.m_pair.Value(), value))
                    {
                        return true;
                    }
                }

                return false;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::Contains(const KeyValuePair<TKey, TValue> &item) const noexcept
            {
                const int i = FindEntry(item.Key());

                if(i >= 0)
                {
                    GenericEqualityComparer<TValue> valueComparer;

                    return valueComparer.Equals(item.Value(), (*m_entries)[i].m_pair.Value());
                }

                return false;
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::CopyTo(Array<KeyValuePair<TKey, TValue>> &arr, const int arrayIndex) const
            {
                if(static_cast<uint32_t>(arrayIndex) > static_cast<uint32_t>(arr.Length()))
                {
                    throw ArgumentOutOfRangeException("arrayIndex");
                }

                if(arr.Length() - arrayIndex < Count())
                {
                    throw ArgumentException("The array is too small.");
                }

                assert(m_entries);

                const Array<Entry> &entries = *m_entries;

                for(int i = 0, destIndex = arrayIndex; i < m_count; ++i)
                {
                    const Entry &entry = entries[i];

                    if(entry.m_next >= -1)
                    {
                        arr[destIndex++] = entry.m_pair;
                    }
                }
            }

            template <typename TKey, typename TValue>
            TValue& Dictionary<TKey, TValue>::operator[](const TKey &key)
            {
                const int i = FindEntry(key);
                
                if(i >= 0)
                {
                    return (*m_entries)[i].m_pair.Value();
                }

                throw KeyNotFoundException();
            }

            template <typename TKey, typename TValue>
            const TValue& Dictionary<TKey, TValue>::operator[](const TKey &key) const
            {
                const int i = FindEntry(key);

                if(i >= 0)
                {
                    return (*m_entries)[i].m_pair.Value();
                }

                throw KeyNotFoundException();
            }

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>& Dictionary<TKey, TValue>::operator=(const Dictionary<TKey, TValue> &copy)
            {
                if(this != &mov)
                {
                    
                    if(mov.m_entries)
                    {
                        m_entries = unique_ptr<Array<Entry>>(DNN_New Array<Entry>(*mov.m_entries));
                    }
                    else
                    {
                        m_entries.reset();
                    }

                    if(mov.m_buckets)
                    {
                        m_buckets = DNN_make_unique(Array<int>, *mov.m_buckets);
                    }
                    else
                    {
                        m_buckets.reset();
                    }

                    m_comparer = mov.m_comparer;
                    m_count = mov.m_count;
                    m_freeList = mov.m_freeList;
                    m_freeCount = mov.m_freeCount;
                    ++m_version;
                }

                return *this;
            }

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>& Dictionary<TKey, TValue>::operator=(Dictionary<TKey, TValue> &&mov) noexcept
            {
                if(this != &mov)
                {
                    m_entries = std::move(mov.m_entries);
                    m_buckets = std::move(mov.m_buckets);
                    m_comparer = std::move(mov.m_comparer);
                    m_count = mov.m_count;
                    m_freeList = mov.m_freeList;
                    m_freeCount = mov.m_freeCount;
                    ++m_version;

                    mov.m_count = 0;
                    mov.m_freeList = -1;
                    mov.m_freeCount = 0;
                    ++mov.m_version;
                }

                return *this;
            }

            template <typename TKey, typename TValue>
            unique_ptr<IReadOnlyCollection<TKey>> Dictionary<TKey, TValue>::Keys() const
            {
                return unique_ptr<IReadOnlyCollection<TKey>>(DNN_New Dictionary<TKey, TValue>::KeyCollection(this));
            }

            template <typename TKey, typename TValue>
            unique_ptr<IReadOnlyCollection<TValue>> Dictionary<TKey, TValue>::Values() const
            {
                return unique_ptr<IReadOnlyCollection<TValue>>(DNN_New Dictionary<TKey, TValue>::ValueCollection(this));
            }

            template <typename TKey, typename TValue>
            unique_ptr<IEnumerator<KeyValuePair<TKey, TValue>>> Dictionary<TKey, TValue>::GetEnumerator()
            {
                return unique_ptr<IEnumerator<KeyValuePair<TKey, TValue>>>(DNN_New Dictionary<TKey, TValue>::KeyValuePairEnumerator(this));
            }

            ///////////////////////////////////////// Dictionary::KeyValuePairEnumerator /////////////////////////////////////////

            template <typename TKey, typename TValue>
            Dictionary<TKey, TValue>::KeyValuePairEnumerator::KeyValuePairEnumerator(const Dictionary<TKey, TValue> *dictionary)
                : m_dictionary(dictionary)
                , m_currentValue(nullptr)
                , m_index(0)
                , m_cookie(dictionary->m_version)
            {
            }

            template <typename TKey, typename TValue>
            const KeyValuePair<TKey, TValue>& Dictionary<TKey, TValue>::KeyValuePairEnumerator::Current() const &
            {
                if(m_index < 0 || !m_currentValue)
                {
                    throw new InvalidOperationException("Enumeration not started.");
                }

                if(m_cookie != m_dictionary->m_version)
                {
                    throw InvalidOperationException("The collection has been modified.");
                }

                return m_currentValue->m_pair;
            }

            template <typename TKey, typename TValue>
            KeyValuePair<TKey, TValue>& Dictionary<TKey, TValue>::KeyValuePairEnumerator::Current() &
            {
                if(m_index < 0 || !m_currentValue)
                {
                    throw new InvalidOperationException("Enumeration not started.");
                }

                if(m_cookie != m_dictionary->m_version)
                {
                    throw InvalidOperationException("The collection has been modified.");
                }

                return m_currentValue->m_pair;
            }

            template <typename TKey, typename TValue>
            bool Dictionary<TKey, TValue>::KeyValuePairEnumerator::MoveNext()
            {
                if(m_cookie != m_dictionary->m_version)
                {
                    throw InvalidOperationException("The dictionary has been modified.");
                }

                while(static_cast<uint32_t>(m_index) < static_cast<uint32_t>(m_dictionary->m_count))
                {
                    m_currentValue = &(*m_dictionary->m_entries)[m_index++];

                    if(m_currentValue->m_next >= -1)
                    {
                        return true;
                    }
                }

                m_index = m_dictionary->m_count + 1;
                m_currentValue = nullptr;

                return false;
            }

            template <typename TKey, typename TValue>
            void Dictionary<TKey, TValue>::KeyValuePairEnumerator::Reset()
            {
                if(m_cookie != m_dictionary->m_version)
                {
                    throw InvalidOperationException("The collection has been modified.");
                }

                m_index = 0;
                m_currentValue = nullptr;
            }

            ///////////////////////////////////////// Dictionary::KeyCollection /////////////////////////////////////////

            template <typename TKey, typename TValue>
            unique_ptr<IEnumerator<TKey>> Dictionary<TKey, TValue>::KeyCollection::GetEnumerator()
            {
                return unique_ptr<IEnumerator<TKey>>(DNN_New Dictionary<TKey, TValue>::KeyEnumerator(m_dictionary));
            }

            ///////////////////////////////////////// Dictionary::ValueCollection /////////////////////////////////////////

            template <typename TKey, typename TValue>
            unique_ptr<IEnumerator<TValue>> Dictionary<TKey, TValue>::ValueCollection::GetEnumerator()
            {
                return unique_ptr<IEnumerator<TValue>>(DNN_New Dictionary<TKey, TValue>::ValueEnumerator(m_dictionary));
            }
        }
    }
}

#endif