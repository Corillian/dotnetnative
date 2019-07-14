#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_IREADONLYLIST_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_IREADONLYLIST_H_

#include "IReadOnlyCollection.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename T>
            class IReadOnlyList
                : public virtual IReadOnlyCollection<T>
            {
            protected:
                IReadOnlyList() = default;
                IReadOnlyList(const IReadOnlyList<T> &copy) = default;
                IReadOnlyList(IReadOnlyList<T> &&mov) noexcept = default;

            public:
                virtual ~IReadOnlyList() {}

                virtual const T& operator[](const int64_t index) const & = 0;
            };
        }
    }
}

#endif