#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_KEYVALUEPAIR_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_KEYVALUEPAIR_H_

#include "../Object.h"
#include "../String.h"
#include "../StringBuilder.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename TKey, typename TValue>
            class KeyValuePair
                : public Object
            {
            private:
                TKey   m_key;
                TValue m_value;

            public:
                inline KeyValuePair(const TKey &key, const TValue &value) : m_key(key), m_value(value) {}
                KeyValuePair(TKey &&key, TValue &&value) : m_key(std::move(key)), m_value(std::move(value)) {}
                KeyValuePair(const KeyValuePair<TKey, TValue> &pair) = default;
                KeyValuePair(KeyValuePair<TKey, TValue> &&pair) noexcept = default;
                virtual ~IReadOnlyCollection() {}

                KeyValuePair<TKey, TValue>& operator=(const KeyValuePair<TKey, TValue> &pair) = default;
                KeyValuePair<TKey, TValue>& operator=(KeyValuePair<TKey, TValue> &&pair) noexcept = default;

                virtual String ToString() override;

                inline const TKey& Key() const noexcept { return m_key; }
                inline TValue& Value() { return m_value; } 
            };

            //////////////////////////////////////////////// KeyValuePair ////////////////////////////////////////////////

            template <typename TKey, typename TValue>
            String KeyValuePair<TKey, TValue>::ToString()
            {
                StringBuilder bldr;

                bldr.Append('[');

                bldr.Append(Object::ToString<TKey>(m_key));

                bldr.Append(", ");

                bldr.Append(Object::ToString<TValue>(m_value));

                bldr.Append(']');

                return bldr.ToString();
            }
        }
    }
}

#endif