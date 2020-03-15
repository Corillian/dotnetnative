#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_ICOMPARER_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_ICOMPARER_H_

namespace DotNetNative
{
    namespace System
    {

        namespace Collections
        {
            // Base interface for all collections, defining enumerators, size, and 
            // synchronization methods.
            template <typename T>
            class IComparer
            {
            protected:
                IComparer() = default;
                IComparer(const IComparer<T> &copy) = default;
                IComparer(IComparer<T> &&mov) noexcept = default;

            public:
                virtual ~IComparer() {}

                virtual int Compare(const T &x, const T &y) const = 0;
            };
        }
    }
}

#endif