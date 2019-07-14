#include "Int32.h"
#include "Exception.h"

namespace DotNetNative
{
    namespace System
    {
        bool Int32::Equals(const Int32 &obj) const noexcept
        {
            return m_int == obj.m_int;
        }

        String Int32::ToString()
        {
            throw NotImplementedException();
        }

        int Int32::GetHashCode() const
        {
            return m_int;
        }

        int Int32::CompareTo(const Int32 &value) const noexcept
        {
            // NOTE: Cannot use return (_value - value) as this causes a wrap
            // around in cases where _value - value > MaxValue.
            if(m_int < value.m_int)
            {
                return -1;
            }

            if(m_int > value.m_int)
            {
                return 1;
            }

            return 0;
        }
    }
}