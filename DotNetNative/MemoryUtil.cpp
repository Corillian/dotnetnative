#include "MemoryUtil.h"

#ifndef DNN_NO_MEMORY_OVERLOAD

_Ret_notnull_ _Post_writable_byte_size_(size)
_VCRT_ALLOCATOR void* operator new(size_t size)
{
#ifdef _DEBUG
    void *memory = DotNetNative::Memory::DebugAlloc(size, __FILE__, __LINE__);
#else
    void *memory = DotNetNative::Memory::Alloc(size);
#endif

    if(!memory)
    {
        throw std::bad_alloc();
    }

    return memory;
}

_Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(size)
_VCRT_ALLOCATOR void* operator new(size_t size, std::nothrow_t const&) noexcept
{
#ifdef _DEBUG
    return DotNetNative::Memory::DebugAlloc(size, __FILE__, __LINE__);
#else
    return DotNetNative::Memory::Alloc(size);
#endif
}

_Ret_notnull_ _Post_writable_byte_size_(size)
_VCRT_ALLOCATOR void* operator new(size_t size, const char *fileName, int lineNumber)
{
#ifdef _DEBUG
    void *memory = DotNetNative::Memory::DebugAlloc(size, fileName, lineNumber);
#else
    void *memory = DotNetNative::Memory::Alloc(size);
#endif

    if(!memory)
    {
        throw std::bad_alloc();
    }

    return memory;
}

void operator delete(void *memory)
{
#ifdef _DEBUG
    DotNetNative::Memory::DebugFree(memory, __FILE__, __LINE__);
#else
    DotNetNative::Memory::Free(memory);
#endif
}

void operator delete(void *memory, const char *fileName, int lineNumber)
{
#ifdef _DEBUG
    DotNetNative::Memory::DebugFree(memory, fileName, lineNumber);
#else
    DotNetNative::Memory::Free(memory);
#endif
}

#endif