#ifndef _DOTNETNATIVE_MEMORY_H_
#define _DOTNETNATIVE_MEMORY_H_

#include <functional>
#include <new>

namespace DotNetNative
{
    class Memory
    {
    public:
        typedef std::function<void*(size_t)> AllocFn;
        typedef std::function<void*(size_t, size_t)> CAllocFn;
        typedef std::function<void*(void*, size_t)> ReallocFn;
        typedef std::function<void(void*)> FreeFn;

        typedef std::function<void*(size_t, size_t)> AlignedAllocFn;
        typedef std::function<void*(size_t, size_t, size_t)> AlignedCAllocFn;
        typedef std::function<void*(void*, size_t, size_t)> AlignedReallocFn;
        typedef std::function<void(void*)> AlignedFreeFn;

        typedef std::function<void*(size_t, const char*, int)> AllocDebugFn;
        typedef std::function<void*(size_t, size_t, const char*, int)> CAllocDebugFn;
        typedef std::function<void*(void*, size_t, const char*, int)> ReallocDebugFn;
        typedef std::function<void(void*, const char*, int)> FreeDebugFn;

        typedef std::function<void*(size_t, size_t, const char*, int)> AlignedAllocDebugFn;
        typedef std::function<void*(size_t, size_t, size_t, const char*, int)> AlignedCAllocDebugFn;
        typedef std::function<void*(void*, size_t, size_t, const char*, int)> AlignedReallocDebugFn;
        typedef std::function<void(void*, const char*, int)> AlignedFreeDebugFn;

        struct AllocatorDescriptors
        {
            AllocFn               m_alloc;
            CAllocFn              m_calloc;
            ReallocFn             m_realloc;
            FreeFn                m_free;

            AlignedAllocFn        m_alignedAlloc;
            AlignedCAllocFn       m_alignedCAlloc;
            AlignedReallocFn      m_alignedRealloc;
            AlignedFreeFn         m_alignedFree;

            AllocDebugFn          m_debugAlloc;
            CAllocDebugFn         m_debugCAlloc;
            ReallocDebugFn        m_debugRealloc;
            FreeDebugFn           m_debugFree;

            AlignedAllocDebugFn   m_debugAlignedAlloc;
            AlignedCAllocDebugFn  m_debugAlignedCAlloc;
            AlignedReallocDebugFn m_debugAlignedRealloc;
            AlignedFreeDebugFn    m_debugAlignedFree;
        };

    private:
        static AllocatorDescriptors g_allocators;

    private:
        Memory() = delete;
        Memory(const Memory &copy) = delete;

    public:
        static void SetAllocators(AllocatorDescriptors &&allocators) noexcept;

        static void* Alloc(size_t size);
        static void* CAlloc(size_t count, size_t size);
        static void* Realloc(void *memory, size_t size);
        static void Free(void *memory);

        static void* AlignedAlloc(size_t size, size_t alignment);
        static void* AlignedCAlloc(size_t count, size_t size, size_t alignment);
        static void* AlignedRealloc(void *memory, size_t size, size_t alignment);
        static void AlignedFree(void *memory);

        static void* DebugAlloc(size_t size, const char *fileName, int lineNumber);
        static void* DebugCAlloc(size_t count, size_t size, const char *fileName, int lineNumber);
        static void* DebugRealloc(void *memory, size_t size, const char *fileName, int lineNumber);
        static void DebugFree(void *memory, const char *fileName, int lineNumber);

        static void* DebugAlignedAlloc(size_t size, size_t alignment, const char *fileName, int lineNumber);
        static void* DebugAlignedCAlloc(size_t count, size_t size, size_t alignment, const char *fileName, int lineNumber);
        static void* DebugAlignedRealloc(void *memory, size_t size, size_t alignment, const char *fileName, int lineNumber);
        static void DebugAlignedFree(void *memory, const char *fileName, int lineNumber);
    };
}

#ifndef _DEBUG

#define DNN_Alloc(size) DotNetNative::Memory::Alloc(size)
#define DNN_CAlloc(num, size) DotNetNative::Memory::CAlloc(num, size)
#define DNN_Realloc(memory, size) DotNetNative::Memory::Realloc(memory, size)
#define DNN_Free(memory) DotNetNative::Memory::Free(memory)

#define DNN_AlignedAlloc(size, alignment) DotNetNative::Memory::AlignedAlloc(size, alignment)
#define DNN_AlignedCAlloc(num, size) DotNetNative::Memory::AlignedCAlloc(num, size, alignment)
#define DNN_AlignedRealloc(memory, size) DotNetNative::Memory::AlignedRealloc(memory, size, alignment)
#define DNN_AlignedFree(memory) DotNetNative::Memory::AlignedFree(memory)

#else

#define DNN_Alloc(size) DotNetNative::Memory::DebugAlloc(size, __FILE__, __LINE__)
#define DNN_CAlloc(num, size) DotNetNative::Memory::DebugCAlloc(num, size, __FILE__, __LINE__)
#define DNN_Realloc(memory, size) DotNetNative::Memory::DebugRealloc(memory, size, __FILE__, __LINE__)
#define DNN_Free(memory) DotNetNative::Memory::DebugFree(memory, __FILE__, __LINE__)

#define DNN_AlignedAlloc(size, alignment) DotNetNative::Memory::DebugAlignedAlloc(size, alignment, __FILE__, __LINE__)
#define DNN_AlignedCAlloc(num, size) DotNetNative::Memory::DebugAlignedCAlloc(num, size, alignment, __FILE__, __LINE__)
#define DNN_AlignedRealloc(memory, size) DotNetNative::Memory::DebugAlignedRealloc(memory, size, alignment, __FILE__, __LINE__)
#define DNN_AlignedFree(memory) DotNetNative::Memory::DebugAlignedFree(memory, __FILE__, __LINE__)

#endif

#endif