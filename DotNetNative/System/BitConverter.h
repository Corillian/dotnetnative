#ifndef _DOTNETNATIVE_SYSTEM_BITCONVERTER_H_
#define _DOTNETNATIVE_SYSTEM_BITCONVERTER_H_

namespace DotNetNative
{
    namespace System
    {
        // The BitConverter class contains methods for
        // converting an array of bytes to one of the base data 
        // types, as well as for converting a base data type to an
        // array of bytes.
        class BitConverter
        {
        public:
            static constexpr bool IsLittleEndian = true;

        private:
            BitConverter() = delete;
            BitConverter(const BitConverter &copy) = delete;
            BitConverter(BitConverter &&mov) = delete;
            ~BitConverter() = delete;

        public:
            inline static double Int64BitsToDouble(const int64_t value) noexcept { return *reinterpret_cast<const double*>(&value); }
        };
    }
}

#endif