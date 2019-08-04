#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_ICOLLECTION_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_ICOLLECTION_H_

#include "IReadOnlyCollection.h"

namespace DotNetNative
{
    namespace System
    {
        template<typename T>
        class Array;

        namespace Collections
        {
            // Base interface for all collections, defining enumerators, size, and 
            // synchronization methods.
            template <typename T>
            class ICollection
                : public virtual IReadOnlyCollection<T>
            {
            protected:
                ICollection() = default;
                ICollection(const ICollection<T> &copy) = default;
                ICollection(ICollection<T> &&mov) noexcept = default;

            public:
                virtual ~ICollection() {}

                virtual bool IsReadOnly() const = 0;

                virtual void Add(const T &item) = 0;
                virtual void Add(T &&item) = 0;

                virtual void Clear() = 0;

                virtual bool Contains(const T &item) const noexcept = 0;

                // CopyTo copies a collection into an Array, starting at a particular
                // index into the array.
                // 
                virtual void CopyTo(Array<T> &arr, const int arrayIndex) const = 0;

                virtual bool Remove(const T &item) = 0;
            };
        }
    }
}

#endif