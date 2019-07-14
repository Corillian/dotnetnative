#ifndef _DOTNETNATIVE_SYSTEM_INT32_H_
#define _DOTNETNATIVE_SYSTEM_INT32_H_

#include "IEquatable.h"
#include "IComparable.h"
#include "Object.h"
#include "Globalization/NumberStyles.h"
#include "String.h"

#include <cstdint>

namespace DotNetNative
{
    namespace System
    {
        // Due to the vtable pointer an Int32 is much larger than an integer
        class Int32
            : public Object
            , public IComparable<Int32>
            , public IEquatable<Int32>
        {
        public:
            static constexpr int32_t MaxValue = 0x7fffffff;
            static constexpr int32_t MinValue = 0x80000000;

        private:
            int32_t m_int;

        public:
            constexpr Int32() : m_int(0) {}
            constexpr Int32(const int32_t value) : m_int(value) {}
            virtual ~Int32() {}

            virtual bool Equals(const Int32 &obj) const noexcept override;
            virtual String ToString() override;
            virtual int GetHashCode() const override;

            virtual int CompareTo(const Int32 &value) const noexcept override;

            constexpr operator int32_t() const noexcept { return m_int; }

            //static int32_t Parse(const String &str);
            //static bool TryParse(const String &str, int32_t &outResult) noexcept;
        };
    }
}

#endif