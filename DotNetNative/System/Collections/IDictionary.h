#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_IDICTIONARY_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_IDICTIONARY_H_

#include "IReadOnlyDictionary.h"
#include "ICollection.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename TKey, typename TValue>
            class IDictionary
                : public virtual IReadOnlyDictionary<TKey, TValue>
                , public virtual ICollection<KeyValuePair<TKey, TValue>>
            {
            protected:
                IDictionary() = default;
                IDictionary(const IDictionary<TKey, TValue> &copy) = default;
                IDictionary(IDictionary<TKey, TValue> &&mov) noexcept = default;

            public:
                virtual ~IDictionary() {}

                virtual TValue& operator[](const TKey &key) = 0;

                virtual void Add(const TKey &key, const TValue &value) = 0;
                virtual void Add(TKey &&key, TValue &&value) = 0;
                virtual bool Remove(const TKey &key) = 0;
            };
        }
    }
}

#endif