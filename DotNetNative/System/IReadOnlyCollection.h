#ifndef _DOTNETNATIVE_SYSTEM_IREADONLYCOLLECTION_H_
#define _DOTNETNATIVE_SYSTEM_IREADONLYCOLLECTION_H_

#include "IEnumerable.h"

namespace DotNetNative
{
    namespace System
    {
        template <typename T>
        class IReadOnlyCollection
            : public IEnumerable<T>
        {
        protected:
            IReadOnlyCollection() = default;
            IReadOnlyCollectionCollection(const IReadOnlyCollection<T> &copy) = default;
            IReadOnlyCollection(IReadOnlyCollection<T> &&mov) noexcept = default;

        public:
            virtual ~IReadOnlyCollection() {}

            virtual int64_t Count() const = 0;
        };
    }
}

#endif