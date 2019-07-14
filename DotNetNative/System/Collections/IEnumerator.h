#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_IENUMERATOR_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_IENUMERATOR_H_

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            //
            // Summary:
            //     Supports a simple iteration over a generic collection.
            //
            // Type parameters:
            //   T:
            //     The type of objects to enumerate.This type parameter is covariant. That is, you
            //     can use either the type you specified or any type that is more derived. For more
            //     information about covariance and contravariance, see Covariance and Contravariance
            //     in Generics.
            template <typename T>
            class IEnumerator
            {
            protected:
                IEnumerator() = default;
                IEnumerator(const IEnumerator<T> &copy) = default;
                IEnumerator(IEnumerator<T> &&mov) noexcept = default;

            public:
                virtual ~IEnumerator() {}

                //
                // Summary:
                //     Gets the element in the collection at the current position of the enumerator.
                //
                // Returns:
                //     The element in the collection at the current position of the enumerator.
                virtual const T& Current() const & = 0;

                //
                // Summary:
                //     Gets the element in the collection at the current position of the enumerator.
                //
                // Returns:
                //     The element in the collection at the current position of the enumerator.
                virtual T& Current() & = 0;

                //
                // Summary:
                //     Advances the enumerator to the next element of the collection.
                //
                // Returns:
                //     true if the enumerator was successfully advanced to the next element; false if
                //     the enumerator has passed the end of the collection.
                //
                // Exceptions:
                //   T:System.InvalidOperationException:
                //     The collection was modified after the enumerator was created.
                virtual bool MoveNext() = 0;

                //
                // Summary:
                //     Sets the enumerator to its initial position, which is before the first element
                //     in the collection.
                //
                // Exceptions:
                //   T:System.InvalidOperationException:
                //     The collection was modified after the enumerator was created.
                virtual void Reset() = 0;
            };
        }
    }
}

#endif