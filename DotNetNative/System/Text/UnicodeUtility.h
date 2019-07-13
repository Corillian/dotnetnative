#ifndef _DOTNETNATIVE_SYSTEM_TEXT_UNICODEUTILITY_H_
#define _DOTNETNATIVE_SYSTEM_TEXT_UNICODEUTILITY_H_

#include "../../GlobalDefs.h"

#include <cassert>

namespace DotNetNative { namespace System { namespace Text {

    class UnicodeUtility
    {
    public:
        /// <summary>
        /// The Unicode replacement character U+FFFD.
        /// </summary>
        static constexpr uint32_t ReplacementChar = 0xFFFD;

    private:
        UnicodeUtility() = delete;
        UnicodeUtility(const UnicodeUtility &copy) = delete;
        UnicodeUtility(UnicodeUtility &&mov) = delete;
        ~UnicodeUtility() = delete;

    public:
        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is a valid Unicode scalar
        /// value, i.e., is in [ U+0000..U+D7FF ], inclusive; or [ U+E000..U+10FFFF ], inclusive.
        /// </summary>
        static constexpr bool IsValidUnicodeScalar(uint32_t value)
        {
            // This is an optimized check that on x86 is just three instructions: lea, xor, cmp.
            // 
            // After the subtraction operation, the input value is modified as such:
            // [ 00000000..0010FFFF ] -> [ FFEF0000..FFFFFFFF ]
            //
            // We now want to _exclude_ the range [ FFEFD800..FFEFDFFF ] (surrogates) from being valid.
            // After the xor, this particular exclusion range becomes [ FFEF0000..FFEF07FF ].
            //
            // So now the range [ FFEF0800..FFFFFFFF ] contains all valid code points,
            // excluding surrogates. This allows us to perform a single comparison.

            return ((value - 0x110000u) ^ 0xD800u) >= 0xFFEF0800u;
        }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is an ASCII
        /// character ([ U+0000..U+007F ]).
        /// </summary>
        /// <remarks>
        /// Per http://www.unicode.org/glossary/#ASCII, ASCII is only U+0000..U+007F.
        /// </remarks>
        static constexpr bool IsAsciiCodePoint(uint32_t value) noexcept { return value <= 0x7Fu; }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is in the
        /// Basic Multilingual Plane (BMP).
        /// </summary>
        static constexpr bool IsBmpCodePoint(uint32_t value) noexcept { return value <= 0xFFFFu; }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is a UTF-16 high surrogate code point,
        /// i.e., is in [ U+D800..U+DBFF ], inclusive.
        /// </summary>
        static constexpr bool IsHighSurrogateCodePoint(uint32_t value) noexcept { return IsInRangeInclusive(value, 0xD800U, 0xDBFFU); }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is between
        /// <paramref name="lowerBound"/> and <paramref name="upperBound"/>, inclusive.
        /// </summary>
        static constexpr bool IsInRangeInclusive(uint32_t value, uint32_t lowerBound, uint32_t upperBound) noexcept { return (value - lowerBound) <= (upperBound - lowerBound); }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is a UTF-16 low surrogate code point,
        /// i.e., is in [ U+DC00..U+DFFF ], inclusive.
        /// </summary>
        static constexpr bool IsLowSurrogateCodePoint(uint32_t value) noexcept { return IsInRangeInclusive(value, 0xDC00U, 0xDFFFU); }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="value"/> is a UTF-16 surrogate code point,
        /// i.e., is in [ U+D800..U+DFFF ], inclusive.
        /// </summary>
        static constexpr bool IsSurrogateCodePoint(uint32_t value) noexcept { return IsInRangeInclusive(value, 0xD800U, 0xDFFFU); }

        /// <summary>
        /// Returns <see langword="true"/> iff <paramref name="codePoint"/> is a valid Unicode code
        /// point, i.e., is in [ U+0000..U+10FFFF ], inclusive.
        /// </summary>
        static constexpr bool IsValidCodePoint(uint32_t codePoint) noexcept { return codePoint <= 0x10FFFFU; }

        /// <summary>
        /// Given a Unicode scalar value, gets the number of UTF-8 code units required to represent this value.
        /// </summary>
        inline static int GetUtf8SequenceLength(uint32_t value)
        {
            assert(IsValidUnicodeScalar(value));

            // The logic below can handle all valid scalar values branchlessly.
            // It gives generally good performance across all inputs, and on x86
            // it's only six instructions: lea, sar, xor, add, shr, lea.

            // 'a' will be -1 if input is < 0x800; else 'a' will be 0
            // => 'a' will be -1 if input is 1 or 2 UTF-8 code units; else 'a' will be 0

            int a = ((int)value - 0x0800) >> 31;

            // The number of UTF-8 code units for a given scalar is as follows:
            // - U+0000..U+007F => 1 code unit
            // - U+0080..U+07FF => 2 code units
            // - U+0800..U+FFFF => 3 code units
            // - U+10000+       => 4 code units
            //
            // If we XOR the incoming scalar with 0xF800, the chart mutates:
            // - U+0000..U+F7FF => 3 code units
            // - U+F800..U+F87F => 1 code unit
            // - U+F880..U+FFFF => 2 code units
            // - U+10000+       => 4 code units
            //
            // Since the 1- and 3-code unit cases are now clustered, they can
            // both be checked together very cheaply.

            value ^= 0xF800u;
            value -= 0xF880u;   // if scalar is 1 or 3 code units, high byte = 0xFF; else high byte = 0x00
            value += (4 << 24); // if scalar is 1 or 3 code units, high byte = 0x03; else high byte = 0x04
            value >>= 24;       // shift high byte down

            // Final return value:
            // - U+0000..U+007F => 3 + (-1) * 2 = 1
            // - U+0080..U+07FF => 4 + (-1) * 2 = 2
            // - U+0800..U+FFFF => 3 + ( 0) * 2 = 3
            // - U+10000+       => 4 + ( 0) * 2 = 4
            return static_cast<int>(value) + (a * 2);
        }

        /// <summary>
        /// Returns the Unicode plane (0 through 16, inclusive) which contains this code point.
        /// </summary>
        inline static int GetPlane(uint32_t codePoint)
        {
            assert(IsValidCodePoint(codePoint));

            return static_cast<int>(codePoint >> 16);
        }

        /// <summary>
        /// Returns a Unicode scalar value from two code points representing a UTF-16 surrogate pair.
        /// </summary>
        inline static uint32_t GetScalarFromUtf16SurrogatePair(uint32_t highSurrogateCodePoint, uint32_t lowSurrogateCodePoint)
        {
            assert(IsHighSurrogateCodePoint(highSurrogateCodePoint));
            assert(IsLowSurrogateCodePoint(lowSurrogateCodePoint));

            // This calculation comes from the Unicode specification, Table 3-5.
            // Need to remove the D800 marker from the high surrogate and the DC00 marker from the low surrogate,
            // then fix up the "wwww = uuuuu - 1" section of the bit distribution. The code is written as below
            // to become just two instructions: shl, lea.

            return (highSurrogateCodePoint << 10) + lowSurrogateCodePoint - ((0xD800U << 10) + 0xDC00U - (1 << 16));
        }

        /// <summary>
        /// Given a Unicode scalar value, gets the number of UTF-16 code units required to represent this value.
        /// </summary>
        inline static int GetUtf16SequenceLength(uint32_t value)
        {
            assert(IsValidUnicodeScalar(value));

            value -= 0x10000;   // if value < 0x10000, high byte = 0xFF; else high byte = 0x00
            value += (2 << 24); // if value < 0x10000, high byte = 0x01; else high byte = 0x02
            value >>= 24;       // shift high byte down

            return static_cast<int>(value);  // and return it
        }

        /// <summary>
        /// Decomposes an astral Unicode scalar into UTF-16 high and low surrogate code units.
        /// </summary>
        static constexpr void GetUtf16SurrogatesFromSupplementaryPlaneScalar(uint32_t value, utf16char &highSurrogateCodePoint, utf16char &lowSurrogateCodePoint)
        {
            assert(IsValidUnicodeScalar(value) && !IsBmpCodePoint(value));

            // This calculation comes from the Unicode specification, Table 3-5.

            highSurrogateCodePoint = static_cast<utf16char>((value + ((0xD800u - 0x40u) << 10)) >> 10);
            lowSurrogateCodePoint = static_cast<utf16char>((value & 0x3FFu) + 0xDC00u);
        }
    };
}}}

#endif