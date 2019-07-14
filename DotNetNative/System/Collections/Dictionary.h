#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_DICTIONARY_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_DICTIONARY_H_

#include "IDictionary.h"
#include "../Array.h"
#include "IEqualityComparer.h"
#include "../../MemoryUtil.h"
#include "../Exception.h"
#include "HashHelpers.h"

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
                private struct Entry
                {
                    // 0-based index of next entry in chain: -1 means end of chain
                    // also encodes whether this entry _itself_ is part of the free list by changing sign and subtracting 3,
                    // so -2 means end of free list, -3 means index 0 but on free list, -4 means index 1 but on free list, etc.
                    public int      m_next;
                    public uint32_t m_hashCode;
                    public TKey     m_key;           // Key of entry
                    public TValue   m_value;         // Value of entry
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

            private:
                static constexpr int64_t StartOfFreeList = -3;

                unique_ptr<Array<Entry>>            m_entries;
                unique_ptr<Array<int>>              m_buckets;
                shared_ptr<IEqualityComparer<TKey>> m_comparer;
                int64_t                             m_count;
                int64_t                             m_freeList;
                int64_t                             m_freeCount;
                int                                 m_version;

            private:
                int Initialize(const int capacity);
                int FindEntry(const TKey &key) const noexcept;

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

                virtual void Add(const TKey &key, const TValue &value) override;
                virtual void Add(TKey &&key, TValue &&value) override;
                virtual bool Remove(const TKey &key) override;
                virtual bool ContainsKey(const TKey &key) const override;
                virtual bool TryGetValue(const TKey &key, TValue &outValue) const override;

                virtual unique_ptr<IEnumerable<TKey>> Keys() const override;
                virtual unique_ptr<IEnumerable<TValue>> Values() const override;

                virtual int64_t Count() const override;

                virtual bool IsReadOnly() const override;

                virtual void Add(const T &item) override;
                virtual void Add(T &&item) override;

                virtual void Clear() override;

                virtual bool Contains(const T &item) const noexcept override;

                // CopyTo copies a collection into an Array, starting at a particular
                // index into the array.
                // 
                virtual void CopyTo(Array<T> &arr, const int arrayIndex) override;

                virtual bool Remove(const T &item) override;

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
            int Dictionary<TKey, TValue>::Initialize(const int capacity)
            {
                const int size = HashHelpers::GetPrime(capacity);

                m_entries = unique_ptr<Array<Entry>>(DNN_New Array<Entry>(size));
                m_buckets = DNN_make_unique(Array<int>, size);
                m_freeList = -1;

                return size;
            }

            template <typename TKey, typename TValue>
            int Dictionary<TKey, TValue>::FindEntry(const TKey &key) const noexcept
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
                            || ((*entries)[i].m_hashCode == hashCode && comparer->Equals((*entries)[i].m_key, key)))
                        {
                            break;
                        }

                        i = (*entries)[i].m_next;

                        if(collisionCount >= entries->Length())
                        {
                            // The chain of entries forms a loop; which means a concurrent update has happened.
                            // Break out of the loop and throw, rather than looping forever.
                            throw InvalidOperationException("Operation not supported.")
                        }

                        collisionCount++;

                    } while(true);
                }

                return i;
            }
        }
    }
}

#endif