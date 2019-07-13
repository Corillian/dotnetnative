#ifndef _DOTNETNATIVE_NEWOVERLOAD_H_
#define _DOTNETNATIVE_NEWOVERLOAD_H_

#include "Memory.h"
#include <memory>
#include <type_traits>
#include <vcruntime_new.h>

#ifndef _WIN32
#define _VCRT_ALLOCATOR
#define _Ret_notnull_
#define _Post_writable_byte_size_(x)
#define _Success_(x)
#endif

#ifndef DNN_NO_MEMORY_OVERLOAD

_Ret_notnull_ _Post_writable_byte_size_(size)
_VCRT_ALLOCATOR void* operator new(size_t size);

_Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(size)
_VCRT_ALLOCATOR void* operator new(size_t size, std::nothrow_t const&) noexcept;

_Ret_notnull_ _Post_writable_byte_size_(size)
_VCRT_ALLOCATOR void* operator new(size_t size, const char *fileName, int lineNumber);

void operator delete(void *memory);

void operator delete(void *memory, const char *fileName, int lineNumber);

#endif

#ifndef _DEBUG
#define DNN_New new
#else
#define DNN_New new(__FILE__, __LINE__)
#endif

namespace DotNetNative
{
    /////////////////////////////////////////////////////// Allocator ///////////////////////////////////////////////////////

    template <typename T>
    class Allocator
    {
        template <typename Other>
        friend class Allocator;

    private:
#ifdef _DEBUG
        const char *m_fileName;
        int         m_lineNumber;
#endif

    public:
        typedef T value_type;

#if _DEBUG
        Allocator(const char *fileName, const int lineNumber) noexcept
            : m_fileName(fileName)
            , m_lineNumber(lineNumber)
        {
        }

        Allocator(const Allocator<T> &alloc) noexcept
            : m_fileName(alloc.m_fileName)
            , m_lineNumber(alloc.m_lineNumber)
        {
        }

        Allocator(Allocator<T> &&alloc) noexcept
            : m_fileName(alloc.m_fileName)
            , m_lineNumber(alloc.m_lineNumber)
        {
        }

        template <typename Other>
        Allocator(const Allocator<Other> &alloc) noexcept
            : m_fileName(alloc.m_fileName)
            , m_lineNumber(alloc.m_lineNumber)
        {
        }

        Allocator<T>& operator=(const Allocator<T> &alloc) noexcept
        {
            if(this != &alloc)
            {
                m_fileName = alloc.m_fileName;
                m_lineNumber = alloc.m_fileName;
            }

            return *this;
        }

        Allocator<T>& operator=(Allocator<T> &&alloc) noexcept
        {
            if(this != &alloc)
            {
                m_fileName = alloc.m_fileName;
                m_lineNumber = alloc.m_fileName;
            }

            return *this;
        }
#else
        Allocator() noexcept {}

        template <typename Other>
        Allocator(const Allocator<Other> &alloc) noexcept {}
#endif

        // [[nodiscard]]
        T* allocate(size_t count)
        {
#if _DEBUG
            return static_cast<T*>(Memory::DebugAlloc(sizeof(T) * count, m_fileName, m_lineNumber));
#else
            return static_cast<T*>(Memory::Alloc(sizeof(T) * count));
#endif
        }

        void deallocate(T *memory, size_t count)
        {
            delete memory;
        }
    };

#ifdef _DEBUG
#define DNN_Allocator(type) DotNetNative::Allocator<type>(__FILE__, __LINE__)
#else
#define DNN_Allocator(type) DotNetNative::Allocator<type>()
#endif

    template <typename T, typename U>
    inline bool operator == (const Allocator<T>&, const Allocator<U>&)
    {
        return true;
    }

    template <typename T, typename U>
    inline bool operator != (const Allocator<T> &a, const Allocator<U> &b)
    {
        return !(a == b);
    }

    template <typename T>
    struct Deleter
    {
        void operator()(T *memory) const noexcept
        {
            static_assert(0 < sizeof(T), "can't delete an incomplete type");

            delete memory;
        }
    };

    template <typename T>
    struct Deleter<T[]>
    {
        template <class T2, std::enable_if_t<std::is_convertible_v<T2(*)[], T(*)[]>, int> = 0>
        void operator()(T2 *memory) const noexcept
        {
            static_assert(0 < sizeof(T2), "can't delete an incomplete type");

            delete[] memory;
        }
    };

    /////////////////////////////////////////////////////// Util Functions ///////////////////////////////////////////////////////

    template <typename T>
    using unique_ptr = std::unique_ptr<T, Deleter<T>>;

    template <typename T>
    using shared_ptr = std::shared_ptr<T>;

#ifdef _DEBUG

#define DNN_make_unique(type, ...) DotNetNative::make_unique<type>(__FILE__, __LINE__, __VA_ARGS__)
#define DNN_make_unique_array(type, size) DotNetNative::make_unique<type>(size, __FILE__, __LINE__)

    template <typename T, typename... TArgs, std::enable_if_t<!std::is_array_v<T>, int> = 0>
    // [[nodiscard]]
    unique_ptr<T> make_unique(const char *fileName, const int lineNumber, TArgs... args)
    {
        return unique_ptr<T>(new(fileName, lineNumber) T(std::forward<TArgs>(args)...), Deleter<T>());
    }

    template <typename T, std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, int> = 0>
    // [[nodiscard]]
    unique_ptr<T> make_unique(size_t size, const char *fileName, const int lineNumber)
    {
        using Elem = std::remove_extent_t<T>;

        return unique_ptr<T>(new(fileName, lineNumber) Elem[size](), Deleter<T>());
    }

#else

#define DNN_make_unique(type, ...) DotNetNative::make_unique<type>(__VA_ARGS__)
#define DNN_make_unique_array(type, size) DotNetNative::make_unique<type>(size)

    template <typename T, typename... TArgs, std::enable_if_t<!std::is_array_v<T>, int> = 0>
    // [[nodiscard]]
    unique_ptr<T> make_unique(TArgs... args)
    {
        return unique_ptr<T>(new T(std::forward<TArgs>(args)...), Deleter<T>());
    }

    template <typename T, std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, int> = 0>
    // [[nodiscard]]
    unique_ptr<T> make_unique(size_t size)
    {
        using Elem = std::remove_extent_t<T>;

        return unique_ptr<T>(new Elem[size](), Deleter<T>());
    }

#endif

#ifdef _DEBUG

#define DNN_make_shared(type, ...) DotNetNative::make_shared<type>(__FILE__, __LINE__, __VA_ARGS__)
#define DNN_make_shared_array(type, size) DotNetNative::make_shared<type>(size, __FILE__, __LINE__)

    template <typename T, typename... TArgs, std::enable_if_t<!std::is_array_v<T>, int> = 0>
    shared_ptr<T> make_shared(const char *fileName, const int lineNumber, TArgs... args)
    {
        return std::allocate_shared<T, Allocator<T>>(Allocator<T>(fileName, lineNumber), std::forward<TArgs>(args)...);
    }

    template <typename T, std::enable_if_t<std::is_array_v<T> && std::extent_v<T> == 0, int> = 0>
    shared_ptr<T> make_shared(size_t size, const char *fileName, const int lineNumber)
    {
        return std::allocate_shared<T, Allocator<T>>(Allocator<T>(fileName, lineNumber), size);
    }

#else

#define DNN_make_shared(type, ...) DotNetNative::make_shared<type>(__VA_ARGS__)
#define DNN_make_shared_array(type, size) DotNetNative::make_shared<type>(size)

    template <typename T, typename... TArgs>
    std::shared_ptr<T> make_shared(TArgs... args)
    {
        return std::allocate_shared<T, Allocator<T>>(Allocator<T>(), std::forward<TArgs>(args)...);
    }

#endif

}

#endif