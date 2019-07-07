#ifndef _DOTNETNATIVE_SYSTEM_CHARUNICODEINFO_H_
#define _DOTNETNATIVE_SYSTEM_CHARUNICODEINFO_H_

#include "UnicodeCategory.h"
#include "Char.h"

namespace DotNetNative
{
    namespace System
    {
        class String;

        class CharUnicodeInfo
        {
        public:
            static constexpr utf16char HIGH_SURROGATE_START = 0xd800;
            static constexpr utf16char HIGH_SURROGATE_END = 0xdbff;
            static constexpr utf16char LOW_SURROGATE_START = 0xdc00;
            static constexpr utf16char LOW_SURROGATE_END = 0xdfff;

            static constexpr int  HIGH_SURROGATE_RANGE = 0x3FF;

            static constexpr int UNICODE_CATEGORY_OFFSET = 0;
            static constexpr int BIDI_CATEGORY_OFFSET = 1;

            // The starting codepoint for Unicode plane 1.  Plane 1 contains 0x010000 ~ 0x01ffff.
            static constexpr int UNICODE_PLANE01_START = 0x10000;

        private:
            CharUnicodeInfo() = delete;
            CharUnicodeInfo(const CharUnicodeInfo &copy) = delete;
            CharUnicodeInfo(CharUnicodeInfo &&mov) = delete;
            ~CharUnicodeInfo() = delete;

        public:
            static UnicodeCategory GetUnicodeCategory(const utf16char ch);
            static UnicodeCategory GetUnicodeCategory(const String &str, const int index);
            static UnicodeCategory GetUnicodeCategory(const int codePoint);

            static double GetNumericValue(const utf16char ch);
            static double GetNumericValue(const String &str, const int index);
        };
    }
}

#endif