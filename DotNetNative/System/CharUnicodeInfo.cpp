#include "CharUnicodeInfo.h"
#include "String.h"
#include "Exception.h"
#include "BitConverter.h"
#include "Buffers/Binary/BinaryPrimitives.h"

#include <cassert>
#include <iterator>

namespace DotNetNative
{
    namespace System
    {
        extern const uint8_t CategoryLevel1Index[2176];
        extern const uint8_t CategoryLevel2Index[5952];
        extern const uint8_t CategoryLevel3Index[10800];
        extern const uint8_t CategoriesValue[172];
        extern const uint8_t NumericLevel1Index[761];
        extern const uint8_t NumericLevel2Index[1024];
        extern const uint8_t NumericLevel3Index[1824];
        extern const uint8_t NumericValues[1320];

        /// <summary>
        /// Convert the BMP character or surrogate pointed by index to a UTF32 value.
        /// This is similar to char.ConvertToUTF32, but the difference is that
        /// it does not throw exceptions when invalid surrogate characters are passed in.
        ///
        /// WARNING: since it doesn't throw an exception it CAN return a value
        /// in the surrogate range D800-DFFF, which are not legal unicode values.
        /// </summary>
        static int InternalConvertToUtf32(const String &str, const int index)
        {
            assert(index >= 0 && index < str.Length());

            if(index < str.Length() - 1)
            {
                int temp1 = static_cast<int>(str[index]) - CharUnicodeInfo::HIGH_SURROGATE_START;

                if(static_cast<uint32_t>(temp1) <= CharUnicodeInfo::HIGH_SURROGATE_RANGE)
                {
                    int temp2 = static_cast<int>(str[index + 1]) - CharUnicodeInfo::LOW_SURROGATE_START;

                    if(static_cast<uint32_t>(temp2) <= CharUnicodeInfo::HIGH_SURROGATE_RANGE)
                    {
                        // Convert the surrogate to UTF32 and get the result.
                        return ((temp1 * 0x400) + temp2 + CharUnicodeInfo::UNICODE_PLANE01_START);
                    }
                }
            }

            return static_cast<int>(str[index]);
        }

        /// <summary>
        /// Returns the Unicode Category property for the character c.
        /// </summary>
        static UnicodeCategory InternalGetUnicodeCategory(const String &value, const int index)
        {
            assert(index < value.Length());

            return CharUnicodeInfo::GetUnicodeCategory(InternalConvertToUtf32(value, index));
        }

        /// <summary>
        /// Returns the Unicode Category property for the character c.
        /// Note that this API will return values for D800-DF00 surrogate halves.
        /// </summary>
        static uint8_t InternalGetCategoryValue(const int ch, const int offset)
        {
            assert(ch >= 0 && ch <= 0x10ffff);

            // Get the level 2 item from the highest 11 bits of ch.
            int index = CategoryLevel1Index[ch >> 9];

            // Get the level 2 WORD offset from the next 5 bits of ch.  This provides the base offset of the level 3 table.
            // Note that & has the lower precedence than addition, so don't forget the parathesis.
            index = *reinterpret_cast<const uint16_t*>(&CategoryLevel2Index[(index << 6) + ((ch >> 3) & 0b111110)]);

            if(!BitConverter::IsLittleEndian)
            {
                index = DotNetNative::System::Buffers::Binary::BinaryPrimitives::ReverseEndianness(static_cast<uint16_t>(index));
            }

            // Get the result from the 0 -3 bit of ch.
            index = CategoryLevel3Index[(index << 4) + (ch & 0x000f)];

            return CategoriesValue[index * 2 + offset];
        }

        /// <summary>
        /// This is called by the public char and string, index versions
        /// Note that for ch in the range D800-DFFF we just treat it as any
        /// other non-numeric character
        /// </summary>
        static double InternalGetNumericValue(const int ch)
        {
            assert(ch >= 0 && ch <= 0x10ffff, "ch is not in valid Unicode range.");

            // Get the level 2 item from the highest 12 bit (8 - 19) of ch.
            int index = ch >> 8;

            if(static_cast<uint32_t>(index) < static_cast<uint32_t>(std::size(NumericLevel1Index)))
            {
                index = NumericLevel1Index[index];

                // Get the level 2 offset from the 4 - 7 bit of ch.  This provides the base offset of the level 3 table.
                // Note that & has the lower precedence than addition, so don't forget the parathesis.
                index = NumericLevel2Index[(index << 4) + ((ch >> 4) & 0x000f)];
                index = NumericLevel3Index[(index << 4) + (ch & 0x000f)];

                const uint8_t *value = &NumericValues[index * 8];

                if(BitConverter::IsLittleEndian)
                {
                    return *reinterpret_cast<const double*>(value);
                }

                return BitConverter::Int64BitsToDouble(DotNetNative::System::Buffers::Binary::BinaryPrimitives::ReverseEndianness(*reinterpret_cast<const int64_t*>(value)));
            }

            return -1;
        }

        UnicodeCategory CharUnicodeInfo::GetUnicodeCategory(const utf16char ch)
        {
            return GetUnicodeCategory(static_cast<int>(ch));
        }

        UnicodeCategory CharUnicodeInfo::GetUnicodeCategory(const String &str, const int index)
        {
            if(static_cast<uint32_t>(index) >= static_cast<uint32_t>(str.Length()))
            {
                throw new ArgumentOutOfRangeException();
            }

            return InternalGetUnicodeCategory(str, index);
        }

        UnicodeCategory CharUnicodeInfo::GetUnicodeCategory(const int codePoint)
        {
            return static_cast<UnicodeCategory>(InternalGetCategoryValue(codePoint, UNICODE_CATEGORY_OFFSET));
        }

        double CharUnicodeInfo::GetNumericValue(const utf16char ch)
        {
            return InternalGetNumericValue(ch);
        }

        double CharUnicodeInfo::GetNumericValue(const String &str, const int index)
        {
            if(index < 0 || index >= str.Length())
            {
                throw new ArgumentOutOfRangeException();
            }

            return InternalGetNumericValue(InternalConvertToUtf32(str, index));
        }
    }
}