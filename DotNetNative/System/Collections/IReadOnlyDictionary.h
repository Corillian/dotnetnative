#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_IREADONLYDICTIONARY_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_IREADONLYDICTIONARY_H_

#include "IReadOnlyCollection.h"
#include "KeyValuePair.h"
#include "../../MemoryUtil.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename TKey, typename TValue>
            class IReadOnlyDictionary
                : public virtual IReadOnlyCollection<KeyValuePair<TKey, TValue>>
            {
            protected:
                IReadOnlyDictionary() = default;
                IReadOnlyDictionary(const IReadOnlyDictionary<TKey, TValue> &copy) = default;
                IReadOnlyDictionary(IReadOnlyDictionary<TKey, TValue> &&mov) noexcept = default;

            public:
                virtual ~IReadOnlyDictionary() {}

                virtual bool ContainsKey(const TKey &key) const = 0;
                virtual bool TryGetValue(const TKey &key, const TValue **outValue) const = 0;

                virtual const TValue& operator[](const TKey &key) const = 0;

                virtual unique_ptr<IReadOnlyCollection<TKey>> Keys() const = 0;
                virtual unique_ptr<IReadOnlyCollection<TValue>> Values() const = 0;
            };
        }
    }
}

#endif