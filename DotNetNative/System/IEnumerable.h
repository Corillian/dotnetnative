#ifndef _DOTNETNATIVE_SYSTEM_IENUMERABLE_H_
#define _DOTNETNATIVE_SYSTEM_IENUMERABLE_H_

#include "IEnumerator.h"
#include "../MemoryUtil.h"

namespace DotNetNative
{
    namespace System
    {
        //
        // Summary:
        //     Exposes the enumerator, which supports a simple iteration over a collection of
        //     a specified type.To browse the .NET Framework source code for this type, see
        //     the Reference Source.
        //
        // Type parameters:
        //   T:
        //     The type of objects to enumerate.This type parameter is covariant. That is, you
        //     can use either the type you specified or any type that is more derived. For more
        //     information about covariance and contravariance, see Covariance and Contravariance
        //     in Generics.
        template <typename T>
        class IEnumerable
        {
        protected:
            IEnumerable() = default;
            IEnumerable(const IEnumerable<T> &copy) = default;
            IEnumerable(IEnumerable<T> &&mov) noexcept = default;

        public:
            virtual ~IEnumerable() {}

            //
            // Summary:
            //     Returns an enumerator that iterates through the collection.
            //
            // Returns:
            //     An enumerator that can be used to iterate through the collection.
            virtual unique_ptr<IEnumerator<T>> GetEnumerator() = 0;
        };
    }
}

#endif