#ifndef _DOTNETNATIVE_SYSTEM_IEQUITABLE_H_
#define _DOTNETNATIVE_SYSTEM_IEQUITABLE_H_

namespace DotNetNative
{
    namespace System
    {
        //
        // Summary:
        //     Defines a generalized method that a value type or class implements to create
        //     a type-specific method for determining equality of instances.
        //
        // Type parameters:
        //   T:
        //     The type of objects to compare.
        template <typename T>
        class IEquitable
        {
        protected:
            IEquitable() = default;
            IEquitable(const IEquitable<T> &copy) = default;
            IEquitable(IEquitable<T> &&mov) noexcept = default;

        public:
            virtual ~IEquitable() {}

            //
            // Summary:
            //     Indicates whether the current object is equal to another object of the same type.
            //
            // Parameters:
            //   other:
            //     An object to compare with this object.
            //
            // Returns:
            //     true if the current object is equal to the other parameter; otherwise, false.
            virtual bool Equals(const T &other) const noexcept = 0;
        };
    }
}

#endif