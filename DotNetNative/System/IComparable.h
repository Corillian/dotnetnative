#ifndef _DOTNETNATIVE_SYSTEM_ICOMPARABLE_H_
#define _DOTNETNATIVE_SYSTEM_ICOMPARABLE_H_

namespace DotNetNative
{
    namespace System
    {
        //
        // Summary:
        //     Defines a generalized comparison method that a value type or class implements
        //     to create a type-specific comparison method for ordering or sorting its instances.
        //
        // Type parameters:
        //   T:
        //     The type of object to compare. This type parameter is contravariant. That is,
        //     you can use either the type you specified or any type that is less derived. For
        //     more information about covariance and contravariance, see Covariance and Contravariance
        //     in Generics.
        template <typename T>
        class IComparable
        {
        protected:
            IComparable() = default;
            IComparable(const IComparable<T> &copy) = default;
            IComparable(IComparable<T> &&mov) noexcept = default;

        public:
            virtual ~IComparable() {}

            //
            // Summary:
            //     Compares the current instance with another object of the same type and returns
            //     an integer that indicates whether the current instance precedes, follows, or
            //     occurs in the same position in the sort order as the other object.
            //
            // Parameters:
            //   other:
            //     An object to compare with this instance.
            //
            // Returns:
            //     A value that indicates the relative order of the objects being compared. The
            //     return value has these meanings: Value Meaning Less than zero This instance precedes
            //     other in the sort order. Zero This instance occurs in the same position in the
            //     sort order as other. Greater than zero This instance follows other in the sort
            //     order.
            virtual int CompareTo(const T &other) const noexcept = 0;
        };
    }
}

#endif