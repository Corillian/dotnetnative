#include "Byte.h"
#include "String.h"
#include "Exception.h"

namespace DotNetNative
{
    namespace System
    {
        bool Byte::Equals(const Byte &obj) const noexcept
        {
            return m_byte == obj.m_byte;
        }

        String Byte::ToString()
        {
            throw NotImplementedException();
        }

        int Byte::GetHashCode() const
        {
            return m_byte;
        }
    }
}