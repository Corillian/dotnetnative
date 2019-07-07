#include "../MemoryUtil.h"
#include "Char.h"
#include "Exception.h"
#include "String.h"
#include "CharUnicodeInfo.h"

namespace DotNetNative
{
    namespace System
    {
        static constexpr int CategoryForLatin1Size = 256;

        static uint8_t CategoryForLatin1[CategoryForLatin1Size] =
        {
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0000 - 0007
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0008 - 000F
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0010 - 0017
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0018 - 001F
            (uint8_t)UnicodeCategory::SpaceSeparator, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::CurrencySymbol, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation,    // 0020 - 0027
            (uint8_t)UnicodeCategory::OpenPunctuation, (uint8_t)UnicodeCategory::ClosePunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::DashPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation,    // 0028 - 002F
            (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber,    // 0030 - 0037
            (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::DecimalDigitNumber, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::OtherPunctuation,    // 0038 - 003F
            (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter,    // 0040 - 0047
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter,    // 0048 - 004F
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter,    // 0050 - 0057
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::OpenPunctuation, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::ClosePunctuation, (uint8_t)UnicodeCategory::ModifierSymbol, (uint8_t)UnicodeCategory::ConnectorPunctuation,    // 0058 - 005F
            (uint8_t)UnicodeCategory::ModifierSymbol, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 0060 - 0067
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 0068 - 006F
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 0070 - 0077
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::OpenPunctuation, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::ClosePunctuation, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::Control,    // 0078 - 007F
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0080 - 0087
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0088 - 008F
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0090 - 0097
            (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control, (uint8_t)UnicodeCategory::Control,    // 0098 - 009F
            (uint8_t)UnicodeCategory::SpaceSeparator, (uint8_t)UnicodeCategory::OtherPunctuation, (uint8_t)UnicodeCategory::CurrencySymbol, (uint8_t)UnicodeCategory::CurrencySymbol, (uint8_t)UnicodeCategory::CurrencySymbol, (uint8_t)UnicodeCategory::CurrencySymbol, (uint8_t)UnicodeCategory::OtherSymbol, (uint8_t)UnicodeCategory::OtherSymbol,    // 00A0 - 00A7
            (uint8_t)UnicodeCategory::ModifierSymbol, (uint8_t)UnicodeCategory::OtherSymbol, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::InitialQuotePunctuation, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::DashPunctuation, (uint8_t)UnicodeCategory::OtherSymbol, (uint8_t)UnicodeCategory::ModifierSymbol,    // 00A8 - 00AF
            (uint8_t)UnicodeCategory::OtherSymbol, (uint8_t)UnicodeCategory::MathSymbol, (uint8_t)UnicodeCategory::OtherNumber, (uint8_t)UnicodeCategory::OtherNumber, (uint8_t)UnicodeCategory::ModifierSymbol, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::OtherSymbol, (uint8_t)UnicodeCategory::OtherPunctuation,    // 00B0 - 00B7
            (uint8_t)UnicodeCategory::ModifierSymbol, (uint8_t)UnicodeCategory::OtherNumber, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::FinalQuotePunctuation, (uint8_t)UnicodeCategory::OtherNumber, (uint8_t)UnicodeCategory::OtherNumber, (uint8_t)UnicodeCategory::OtherNumber, (uint8_t)UnicodeCategory::OtherPunctuation,    // 00B8 - 00BF
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter,    // 00C0 - 00C7
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter,    // 00C8 - 00CF
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::MathSymbol,    // 00D0 - 00D7
            (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::UppercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 00D8 - 00DF
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 00E0 - 00E7
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 00E8 - 00EF
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::MathSymbol,    // 00F0 - 00F7
            (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter, (uint8_t)UnicodeCategory::LowercaseLetter,    // 00F8 - 00FF
        };

        // Return true for all characters below or equal U+00ff, which is ASCII + Latin-1 Supplement.
        constexpr static bool IsLatin1(const utf16char chr) noexcept { return static_cast<uint32_t>(chr) <= 0x00ff; }

        // Return true for all characters below or equal U+007f, which is ASCII.
        constexpr static bool IsAscii(const utf16char chr) noexcept { return static_cast<uint32_t>(chr) <= 0x007f; }


        constexpr static bool IsInRange(const utf16char c, const utf16char min, const utf16char max) noexcept { return (static_cast<uint32_t>(c) - static_cast<uint32_t>(min)) <= (static_cast<uint32_t>(max) - static_cast<uint32_t>(min)); }

        constexpr static bool IsInRange(const UnicodeCategory c, const UnicodeCategory min, const UnicodeCategory max) noexcept { return (static_cast<uint32_t>(c) - static_cast<uint32_t>(min)) <= (static_cast<uint32_t>(max) - static_cast<uint32_t>(min)); }

        constexpr static bool CheckLetter(const UnicodeCategory uc)
        {
            return IsInRange(uc, UnicodeCategory::UppercaseLetter, UnicodeCategory::OtherLetter);
        }

        // Return the Unicode category for Unicode character <= 0x00ff.      
        static UnicodeCategory GetLatin1UnicodeCategory(const utf16char ch)
        {
            if(ch >= CategoryForLatin1Size)
            {
                throw ArgumentOutOfRangeException("Invalid index into Latin1 unicode category.");
            }

            return static_cast<UnicodeCategory>(CategoryForLatin1[ch]);
        }

        static constexpr bool IsWhiteSpaceLatin1(const utf16char c)
        {
            // There are characters which belong to UnicodeCategory.Control but are considered as white spaces.
            // We use code point comparisons for these characters here as a temporary fix.

            // U+0009 = <control> HORIZONTAL TAB
            // U+000a = <control> LINE FEED
            // U+000b = <control> VERTICAL TAB
            // U+000c = <contorl> FORM FEED
            // U+000d = <control> CARRIAGE RETURN
            // U+0085 = <control> NEXT LINE
            // U+00a0 = NO-BREAK SPACE
            return
                c == ' ' ||
                static_cast<uint32_t>(c - 0x0009) <= (0x000d - 0x0009) || // (c >= '\x0009' && c <= '\x000d')
                c == 0x00a0 ||
                c == 0x0085;
        }

        static constexpr bool CheckSeparator(const UnicodeCategory uc)
        {
            return IsInRange(uc, UnicodeCategory::SpaceSeparator, UnicodeCategory::ParagraphSeparator);
        }

        static constexpr bool CheckPunctuation(const UnicodeCategory uc)
        {
            return IsInRange(uc, UnicodeCategory::ConnectorPunctuation, UnicodeCategory::OtherPunctuation);
        }

        static constexpr bool CheckLetterOrDigit(const UnicodeCategory uc)
        {
            return CheckLetter(uc) || uc == UnicodeCategory::DecimalDigitNumber;
        }

        static constexpr bool CheckNumber(const UnicodeCategory uc)
        {
            return IsInRange(uc, UnicodeCategory::DecimalDigitNumber, UnicodeCategory::OtherNumber);
        }

        static constexpr bool IsSeparatorLatin1(const utf16char c)
        {
            // U+00a0 = NO-BREAK SPACE
            // There is no LineSeparator or ParagraphSeparator in Latin 1 range.
            return c == 0x0020 || c == 0x00a0;
        }

        static constexpr bool CheckSymbol(const UnicodeCategory uc)
        {
            return IsInRange(uc, UnicodeCategory::MathSymbol, UnicodeCategory::OtherSymbol);
        }

        static void ConvertToUtf32_ThrowInvalidArgs(const uint32_t highSurrogateOffset)
        {
            // If the high surrogate is not within its expected range, throw an exception
            // whose message fingers it as invalid. If it's within the expected range,
            // change the message to read that the low surrogate was the problem.

            if(highSurrogateOffset > CharUnicodeInfo::HIGH_SURROGATE_RANGE)
            {
                throw new ArgumentOutOfRangeException();
            }
            else
            {
                throw new ArgumentOutOfRangeException();
            }
        }

        bool Char::Equals(const Char &obj) const noexcept
        {
            return this == &obj || m_char == obj.m_char;
        }

        String Char::ToString()
        {
            return String(&m_char, 1);
        }

        int Char::GetHashCode() const
        {
            return static_cast<int>(m_char) | (static_cast<int>(m_char) << 16);
        }

        utf16char Char::Parse(const String &str)
        {
            if(str.Length() != 1)
            {
                throw new FormatException("The string must only contain a single character.");
            }

            return str[0];
        }

        bool Char::TryParse(const String &str, utf16char &outResult) noexcept
        {
            if(str.Length() != 1)
            {
                outResult = 0;

                return false;
            }

            outResult = str[0];

            return true;
        }

        bool Char::IsDigit(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return IsInRange(c, '0', '9');
            }

            return CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::DecimalDigitNumber;
        }

        bool Char::IsLetter(utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                if(IsAscii(c))
                {
                    c |= (char)0x20;

                    return IsInRange(c, 'a', 'z');
                }

                return (CheckLetter(GetLatin1UnicodeCategory(c)));
            }

            return CheckLetter(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        bool Char::IsWhiteSpace(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return IsWhiteSpaceLatin1(c);
            }

            return CheckSeparator(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        bool Char::IsUpper(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                if(IsAscii(c))
                {
                    return IsInRange(c, 'A', 'Z');
                }

                return GetLatin1UnicodeCategory(c) == UnicodeCategory::UppercaseLetter;
            }

            return CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::UppercaseLetter;
        }

        bool Char::IsLower(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                if(IsAscii(c))
                {
                    return IsInRange(c, 'a', 'z');
                }

                return GetLatin1UnicodeCategory(c) == UnicodeCategory::LowercaseLetter;
            }

            return CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::LowercaseLetter;
        }

        bool Char::IsPunctuation(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return CheckPunctuation(GetLatin1UnicodeCategory(c));
            }

            return CheckPunctuation(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        bool Char::IsLetterOrDigit(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return CheckLetterOrDigit(GetLatin1UnicodeCategory(c));
            }

            return CheckLetterOrDigit(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        bool Char::IsControl(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return GetLatin1UnicodeCategory(c) == UnicodeCategory::Control;
            }

            return CharUnicodeInfo::GetUnicodeCategory(c) == UnicodeCategory::Control;
        }

        bool Char::IsNumber(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                if(IsAscii(c))
                {
                    return IsInRange(c, '0', '9');
                }

                return CheckNumber(GetLatin1UnicodeCategory(c));
            }

            return CheckNumber(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        bool Char::IsSeparator(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return IsSeparatorLatin1(c);
            }

            return CheckSeparator(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        bool Char::IsSurrogate(const utf16char c) noexcept
        {
            return IsInRange(c, CharUnicodeInfo::HIGH_SURROGATE_START, CharUnicodeInfo::LOW_SURROGATE_END);
        }

        bool Char::IsSymbol(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return CheckSymbol(GetLatin1UnicodeCategory(c));
            }

            return CheckSymbol(CharUnicodeInfo::GetUnicodeCategory(c));
        }

        UnicodeCategory Char::GetUnicodeCategory(const utf16char c) noexcept
        {
            if(IsLatin1(c))
            {
                return (GetLatin1UnicodeCategory(c));
            }

            return CharUnicodeInfo::GetUnicodeCategory(static_cast<int>(c));
        }

        double Char::GetNumericValue(const utf16char c) noexcept
        {
            return CharUnicodeInfo::GetNumericValue(c);
        }

        bool Char::IsHighSurrogate(const utf16char c) noexcept
        {
            return IsInRange(c, CharUnicodeInfo::HIGH_SURROGATE_START, CharUnicodeInfo::HIGH_SURROGATE_END);
        }

        bool Char::IsLowSurrogate(const utf16char c) noexcept
        {
            return IsInRange(c, CharUnicodeInfo::LOW_SURROGATE_START, CharUnicodeInfo::LOW_SURROGATE_END);
        }

        bool Char::IsSurrogatePair(const utf16char highSurrogate, const utf16char lowSurrogate) noexcept
        {
            // Since both the high and low surrogate ranges are exactly 0x400 elements
            // wide, and since this is a power of two, we can perform a single comparison
            // by baselining each value to the start of its respective range and taking
            // the logical OR of them.

            const uint32_t highSurrogateOffset = static_cast<uint32_t>(highSurrogate) - CharUnicodeInfo::HIGH_SURROGATE_START;
            const uint32_t lowSurrogateOffset = static_cast<uint32_t>(lowSurrogate) - CharUnicodeInfo::LOW_SURROGATE_START;

            return (highSurrogateOffset | lowSurrogateOffset) <= CharUnicodeInfo::HIGH_SURROGATE_RANGE;
        }

        int Char::ConvertToUtf32(const utf16char highSurrogate, const utf16char lowSurrogate) noexcept
        {
            // First, extend both to 32 bits, then calculate the offset of
            // each candidate surrogate char from the start of its range.

            const uint32_t highSurrogateOffset = static_cast<uint32_t>(highSurrogate) - CharUnicodeInfo::HIGH_SURROGATE_START;
            const uint32_t lowSurrogateOffset = static_cast<uint32_t>(lowSurrogate) - CharUnicodeInfo::LOW_SURROGATE_START;

            // This is a single comparison which allows us to check both for validity at once since
            // both the high surrogate range and the low surrogate range are the same length.
            // If the comparison fails, we call to a helper method to throw the correct exception message.

            if((highSurrogateOffset | lowSurrogateOffset) > CharUnicodeInfo::HIGH_SURROGATE_RANGE)
            {
                ConvertToUtf32_ThrowInvalidArgs(highSurrogateOffset);
            }

            // The 0x40u << 10 below is to account for uuuuu = wwww + 1 in the surrogate encoding.
            return (static_cast<int>(highSurrogateOffset) << 10) + (lowSurrogate - CharUnicodeInfo::LOW_SURROGATE_START) + (0x40 << 10);
        }
    }
}