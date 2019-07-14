#ifndef _DOTNETNATIVE_GLOBALDEFS_H_
#define _DOTNETNATIVE_GLOBALDEFS_H_

#include <cstdint>
#include <type_traits>

namespace DotNetNative
{
    typedef char asciichar;
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

    inline size_t utf32len(const uint32_t *str)
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

    /*-**********************************************************************
     *  32-bit hash
     ************************************************************************/
    typedef unsigned int XXH32_hash_t;

    /*! XXH32() :
        Calculate the 32-bit hash of sequence "length" bytes stored at memory address "input".
        The memory between input & input+length must be valid (allocated and read-accessible).
        "seed" can be used to alter the result predictably.
        Speed on Core 2 Duo @ 3 GHz (single thread, SMHasher benchmark) : 5.4 GB/s */
    XXH32_hash_t XXH32(const void* input, size_t length, unsigned int seed);

    /*-**********************************************************************
     *  64-bit hash
     ************************************************************************/
    typedef unsigned long long XXH64_hash_t;

    /*! XXH64() :
        Calculate the 64-bit hash of sequence of length "len" stored at memory address "input".
        "seed" can be used to alter the result predictably.
        This function runs faster on 64-bit systems, but slower on 32-bit systems (see benchmark).
    */
    XXH64_hash_t XXH64(const void* input, size_t length, unsigned long long seed);

    template <typename T, typename = typename std::enable_if<std::is_trivial<T>::value>::type>
    XXH32_hash_t HashTrivialObject32(const T &obj)
    {
        return XXH32(&obj, sizeof(T), 0);
    }

    template <typename T, typename = typename std::enable_if<std::is_trivial<T>::value>::type>
    XXH64_hash_t HashTrivialObject64(const T & obj)
    {
        return XXH64(&obj, sizeof(T), 0);
    }
}

#endif