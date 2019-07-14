#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_DICTIONARY_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_DICTIONARY_H_

#include "IDictionary.h"
#include "../Array.h"
#include "IEqualityComparer.h"
#include "../../MemoryUtil.h"

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

            private:
                static constexpr int64_t StartOfFreeList = -3;

                unique_ptr<Array<Entry>>            m_entries;
                unique_ptr<Array<int>>              m_buckets;
                shared_ptr<IEqualityComparer<TKey>> m_comparer;
                int64_t                             m_count;
                int64_t                             m_freeList;
                int64_t                             m_freeCount;
                int                                 m_version;

            public:
                Dictionary();
                Dictionary(const int64_t capacity);
                Dictionary(const shared_ptr<IEqualityComparer<TKey>> &comparer);
                Dictionary(const int64_t capacity, const shared_ptr<IEqualityComparer<TKey>> &comparer);
                Dictionary(IEnumerable<KeyValuePair<TKey, TValue>> &collection, const shared_ptr<IEqualityComparer<TKey>> &comparer);
                Dictionary(IEnumerable<KeyValuePair<TKey, TValue>> &collection);
                Dictionary(const Dictionary<TKey, TValue> &copy);
                Dictionary(const Dictionary<TKey, TValue> &copy, const shared_ptr<IEqualityComparer<TKey>> &comparer);
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
            };

            ///////////////////////////////////////// Dictionary /////////////////////////////////////////


        }
    }
}

#endif