#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_IEQUALITYCOMPARER_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_IEQUALITYCOMPARER_H_

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            // The generic IEqualityComparer interface implements methods to if check two objects are equal
            // and generate Hashcode for an object.
            // It is use in Dictionary class.  
            template <typename T>
            class IEqualityComparer
            {
            protected:
                IEqualityComparer() = default;
                IEqualityComparer(const IEqualityComparer<T> &copy) = default;
                IEqualityComparer(IEqualityComparer<T> &&mov) noexcept = default;

            public:
                virtual ~IEqualityComparer() {}

                virtual bool Equals(const T &x, const T &y) const noexcept = 0;
                virtual int GetHashCode(const T &obj) const = 0;
            };
        }
    }
}

#endif