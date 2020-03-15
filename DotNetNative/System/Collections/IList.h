#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_ILIST_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_ILIST_H_

#include "IReadOnlyList.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename T>
            class IList
                : public virtual IReadOnlyList<T>
            {
            protected:
                IList() = default;
                IList(const IList<T> &copy) = default;
                IList(IList<T> &&mov) noexcept = default;

            public:
                virtual ~IList() {}

                virtual T& operator[](const int64_t index) & = 0;

                // Returns the index of a particular item, if it is in the list.
                // Returns -1 if the item isn't in the list.
                virtual int IndexOf(const T &item) const = 0;

                // Inserts value into the list at position index.
                // index must be non-negative and less than or equal to the 
                // number of elements in the list.  If index equals the number
                // of items in the list, then value is appended to the end.
                virtual void Insert(int index, const T &item) = 0;
                virtual void Insert(int index, T &&item) = 0;

                // Removes the item at position index.
                virtual void RemoveAt(int index) = 0;

                virtual bool IsFixedSize() const noexcept = 0;

                virtual void Clear() = 0;
            };
        }
    }
}

#endif