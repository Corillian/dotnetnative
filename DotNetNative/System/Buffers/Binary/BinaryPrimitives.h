#ifndef _DOTNETNATIVE_SYSTEM_BUFFERS_BINARY_BINARYPRIMITIVES_H_
#define _DOTNETNATIVE_SYSTEM_BUFFERS_BINARY_BINARYPRIMITIVES_H_

#include <cstdint>
#include <cstdlib>

#ifdef _WIN32
#include <intrin.h>
#else
static_assert(false, "TODO: Include intrinsics for the target OS.");
#endif

namespace DotNetNative { namespace System { namespace Buffers { namespace Binary {
    
    class BinaryPrimitives
    {
    private:
        BinaryPrimitives() = delete;
        BinaryPrimitives(const BinaryPrimitives &copy) = delete;
        BinaryPrimitives(BinaryPrimitives &&mov) = delete;
        ~BinaryPrimitives() = delete;

    public:
        static constexpr int8_t ReverseEndianness(const int8_t value) noexcept { return value; }
        static constexpr uint8_t ReverseEndianness(const uint8_t value) noexcept { return value; }

        static constexpr int16_t ReverseEndianness(const int16_t value) noexcept { return static_cast<int16_t>(ReverseEndianness(static_cast<uint16_t>(value))); }
        static constexpr uint16_t ReverseEndianness(const uint16_t value) noexcept
        {
            // Don't need to AND with 0xFF00 or 0x00FF since the final
            // cast back to ushort will clear out all bits above [ 15 .. 00 ].
            // This is normally implemented via "movzx eax, ax" on the return.
            // Alternatively, the compiler could elide the movzx instruction
            // entirely if it knows the caller is only going to access "ax"
            // instead of "eax" / "rax" when the function returns.

            return static_cast<uint16_t>((value >> 8) + (value << 8));
        }

        inline static int32_t ReverseEndianness(const int32_t value) noexcept { return static_cast<int32_t>(ReverseEndianness(static_cast<uint32_t>(value))); }
        inline static uint32_t ReverseEndianness(const uint32_t value) noexcept
        {
            // This takes advantage of the fact that the JIT can detect
            // ROL32 / ROR32 patterns and output the correct intrinsic.
            //
            // Input: value = [ ww xx yy zz ]
            //
            // First line generates : [ ww xx yy zz ]
            //                      & [ 00 FF 00 FF ]
            //                      = [ 00 xx 00 zz ]
            //             ROR32(8) = [ zz 00 xx 00 ]
            //
            // Second line generates: [ ww xx yy zz ]
            //                      & [ FF 00 FF 00 ]
            //                      = [ ww 00 yy 00 ]
            //             ROL32(8) = [ 00 yy 00 ww ]
            //
            //                (sum) = [ zz yy xx ww ]
            //
            // Testing shows that throughput increases if the AND
            // is performed before the ROL / ROR.

            return _rotr(value & 0x00FF00FFu, 8) // xx zz
                + _rotl(value & 0xFF00FF00u, 8); // ww yy
        }

        inline static int64_t ReverseEndianness(const int64_t value) noexcept { return static_cast<int64_t>(ReverseEndianness(static_cast<uint64_t>(value))); }
        inline static uint64_t ReverseEndianness(const uint64_t value) noexcept
        {
            // Operations on 32-bit values have higher throughput than
            // operations on 64-bit values, so decompose.

            return (static_cast<uint64_t>(ReverseEndianness(static_cast<uint32_t>(value))) << 32)
                + ReverseEndianness(static_cast<uint32_t>(value >> 32));
        }
    };

}}}}

#endif
