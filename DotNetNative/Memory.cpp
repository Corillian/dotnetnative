#include "Memory.h"

#include <cstdlib>

namespace DotNetNative
{
    Memory::AllocatorDescriptors Memory::g_allocators;

    void Memory::SetAllocators(Memory::AllocatorDescriptors &&allocators) noexcept
    {
        g_allocators = std::move(allocators);
    }

    void* Memory::Alloc(size_t size)
    {
        if(g_allocators.m_alloc)
        {
            return g_allocators.m_alloc(size);
        }

        return std::malloc(size);
    }

    void* Memory::CAlloc(size_t count, size_t size)
    {
        if(g_allocators.m_calloc)
        {
            return g_allocators.m_calloc(count, size);
        }

        return std::calloc(count, size);
    }

    void* Memory::Realloc(void *memory, size_t size)
    {
        if(g_allocators.m_realloc)
        {
            return g_allocators.m_realloc(memory, size);
        }

        return std::realloc(memory, size);
    }

    void Memory::Free(void *memory)
    {
        if(g_allocators.m_free)
        {
            return g_allocators.m_free(memory);
        }

        return std::free(memory);
    }

    void* Memory::AlignedAlloc(size_t size, size_t alignment)
    {
        if(g_allocators.m_alignedAlloc)
        {
            return g_allocators.m_alignedAlloc(size, alignment);
        }

        return ::_aligned_malloc(size, alignment);
    }

    void* Memory::AlignedCAlloc(size_t count, size_t size, size_t alignment)
    {
        if(g_allocators.m_alignedCAlloc)
        {
            return g_allocators.m_alignedCAlloc(count, size, alignment);
        }

        return ::_aligned_recalloc(nullptr, count, size, alignment);
    }

    void* Memory::AlignedRealloc(void *memory, size_t size, size_t alignment)
    {
        if(g_allocators.m_alignedRealloc)
        {
            return g_allocators.m_alignedRealloc(memory, size, alignment);
        }

        return ::_aligned_realloc(memory, size, alignment);
    }

    void Memory::AlignedFree(void *memory)
    {
        if(g_allocators.m_alignedFree)
        {
            return g_allocators.m_alignedFree(memory);
        }

        return ::_aligned_free(memory);
    }

    void* Memory::DebugAlloc(size_t size, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugAlloc)
        {
            return g_allocators.m_debugAlloc(size, fileName, lineNumber);
        }

        return ::_malloc_dbg(size, _NORMAL_BLOCK, fileName, lineNumber);
    }

    void* Memory::DebugCAlloc(size_t count, size_t size, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugCAlloc)
        {
            return g_allocators.m_debugCAlloc(count, size, fileName, lineNumber);
        }

        return ::_calloc_dbg(count, size, _NORMAL_BLOCK, fileName, lineNumber);
    }

    void* Memory::DebugRealloc(void *memory, size_t size, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugRealloc)
        {
            return g_allocators.m_debugRealloc(memory, size, fileName, lineNumber);
        }

        return ::_realloc_dbg(memory, size, _NORMAL_BLOCK, fileName, lineNumber);
    }

    void Memory::DebugFree(void *memory, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugFree)
        {
            return g_allocators.m_debugFree(memory, fileName, lineNumber);
        }

        return ::_free_dbg(memory, _NORMAL_BLOCK);
    }

    void* Memory::DebugAlignedAlloc(size_t size, size_t alignment, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugAlignedAlloc)
        {
            return g_allocators.m_debugAlignedAlloc(size, alignment, fileName, lineNumber);
        }

        return ::_aligned_malloc_dbg(size, alignment, fileName, lineNumber);
    }

    void* Memory::DebugAlignedCAlloc(size_t count, size_t size, size_t alignment, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugAlignedCAlloc)
        {
            return g_allocators.m_debugAlignedCAlloc(count, size, alignment, fileName, lineNumber);
        }

        return ::_aligned_recalloc_dbg(nullptr, count, size, alignment, fileName, lineNumber);
    }

    void* Memory::DebugAlignedRealloc(void *memory, size_t size, size_t alignment, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugAlignedRealloc)
        {
            return g_allocators.m_debugAlignedRealloc(memory, size, alignment, fileName, lineNumber);
        }

        return ::_aligned_realloc_dbg(memory, size, alignment, fileName, lineNumber);
    }

    void Memory::DebugAlignedFree(void *memory, const char *fileName, int lineNumber)
    {
        if(g_allocators.m_debugAlignedFree)
        {
            return g_allocators.m_debugAlignedFree(memory, fileName, lineNumber);
        }

        return ::_aligned_free_dbg(memory);
    }
}