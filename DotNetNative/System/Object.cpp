#include "Object.h"
#include "String.h"

namespace DotNetNative
{
    namespace System
    {
        bool Object::Equals(const Object &obj) const noexcept
        {
            return this == &obj;
        }

        String Object::ToString()
        {
            return String("System.Object");
        }

        int Object::GetHashCode() const
        {
            ptrdiff_t ptrVal = reinterpret_cast<ptrdiff_t>(this);

            if(sizeof(void*) >= 8)
            {
                return static_cast<int>((ptrVal & 0xffffffff) ^ ((ptrVal >> 32) & 0xffffffff));
            }

            return static_cast<int>(ptrVal);
        }
    }
}