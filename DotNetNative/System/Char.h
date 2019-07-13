#ifndef _DOTNETNATIVE_SYSTEM_CHAR_H_
#define _DOTNETNATIVE_SYSTEM_CHAR_H_

#include "../GlobalDefs.h"
#include "IEquitable.h"
#include "IComparable.h"
#include "UnicodeCategory.h"
#include "Object.h"

namespace DotNetNative
{
    namespace System
    {
        class Char
            : public Object
            , public IComparable<Char>
            , public IEquitable<Char>
        {
        public:
            static constexpr utf16char MaxValue = 0xFFFF;
            static constexpr utf16char MinValue = 0x00;
            static constexpr int UNICODE_PLANE00_END = 0x00ffff;

            // The starting codepoint for Unicode plane 1.  Plane 1 contains 0x010000 ~ 0x01ffff.
            static constexpr int UNICODE_PLANE01_START = 0x10000;

            // The end codepoint for Unicode plane 16.  This is the maximum code point value allowed for Unicode.
            // Plane 16 contains 0x100000 ~ 0x10ffff.
            static constexpr int UNICODE_PLANE16_END = 0x10ffff;

        private:
            const utf16char m_char;

        public:
            constexpr Char() : m_char(0) {}
            constexpr Char(const utf16char chr) : m_char(chr) {}
            constexpr Char(const char chr) : m_char(chr) {}
            virtual ~Char() {}

            virtual bool Equals(const Char &obj) const noexcept override;
            virtual String ToString() override;
            virtual int GetHashCode() const override;

            virtual int CompareTo(const Char &chr) const noexcept override { return m_char - chr.m_char; }
            constexpr int CompareTo(const utf16char chr) const noexcept { return m_char - chr; }
            constexpr operator utf16char() const noexcept { return m_char; }

            static utf16char Parse(const String &str);
            static bool TryParse(const String &str, utf16char &outResult) noexcept;
            static bool IsDigit(const utf16char c) noexcept;
            static bool IsLetter(utf16char c) noexcept;
            static bool IsWhiteSpace(const utf16char c) noexcept;
            static bool IsUpper(const utf16char c) noexcept;
            static bool IsLower(const utf16char c) noexcept;
            static bool IsPunctuation(const utf16char c) noexcept;
            static bool IsLetterOrDigit(const utf16char c) noexcept;
            static bool IsControl(const utf16char c) noexcept;
            static bool IsNumber(const utf16char c) noexcept;
            static bool IsSeparator(const utf16char c) noexcept;
            static bool IsSurrogate(const utf16char c) noexcept;
            static bool IsSymbol(const utf16char c) noexcept;
            static UnicodeCategory GetUnicodeCategory(const utf16char c) noexcept;
            static double GetNumericValue(const utf16char c) noexcept;
            static bool IsHighSurrogate(const utf16char c) noexcept;
            static bool IsLowSurrogate(const utf16char c) noexcept;
            static bool IsSurrogatePair(const utf16char highSurrogate, const utf16char lowSurrogate) noexcept;
            static int ConvertToUtf32(const utf16char highSurrogate, const utf16char lowSurrogate) noexcept;

            // ToUpper()
            // ToUpperInvariant()
            // ToLower()
            // ToLowerInvariant()
        };
    }
}

#endif