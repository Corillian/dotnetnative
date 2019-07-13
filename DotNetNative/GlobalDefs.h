#ifndef _DOTNETNATIVE_GLOBALDEFS_H_
#define _DOTNETNATIVE_GLOBALDEFS_H_

#include <cstdint>

namespace DotNetNative
{
    typedef int8_t asciichar;
    typedef uint8_t utf8char;
    typedef uint16_t utf16char;
    typedef uint32_t utf32char;

    inline size_t utf16len(const utf16char *str)
    {
        if(!str)
        {
            return 0;
        }

        int count = 0;

        while(*str)
        {
            ++count;
            ++str;
        }

        return count;
    }
}

#endif