#ifndef _DOTNETNATIVE_SYSTEM_ARRAY_H_
#define _DOTNETNATIVE_SYSTEM_ARRAY_H_

#include "../MemoryUtil.h"
#include "ObjectImpl.h"
#include "Exception.h"
#include "Collections/IReadOnlyList.h"
#include "Collections/GenericEnumerator.h"

namespace DotNetNative
{
    namespace System
    {
        template <typename T>
        class Array
            : public Object
            , public Collections::IReadOnlyList<T>
        {
        private:
            T       *m_array;
            int64_t  m_length;

        public:
            Array() : m_array(nullptr), m_length(0) {}
            Array(const int64_t length);
            Array(const T *arr, const int64_t length);
            Array(const Array<T> &copy);
            Array(Array<T> &&mov) noexcept;
            virtual ~Array();

            Array<T>& operator=(const Array<T> &copy);
            Array<T>& operator=(Array<T> &&mov) noexcept;

            virtual const T& operator[](const int64_t index) const & override;
            T& operator[](const int64_t index) &;

            virtual int64_t Count() const override { return m_length; }
            inline int64_t Length() const noexcept { return m_length; }
            inline operator const T*() const noexcept { return m_array.get(); }
            inline operator T*() noexcept { return m_array.get(); }

            //
            // Summary:
            //     Returns an enumerator that iterates through the collection.
            //
            // Returns:
            //     An enumerator that can be used to iterate through the collection.
            virtual unique_ptr<Collections::IEnumerator<T>> GetEnumerator() override;

            // Returns the index of a particular item, if it is in the list.
            // Returns -1 if the item isn't in the list.
            inline static int IndexOf(const Array<T> &ar, const T &item) { return IndexOf(ar, item, 0, ar.Length()); }
            inline static int IndexOf(const Array<T> &ar, const T &item, const int64_t startIndex) { return IndexOf(ar, item, startIndex, ar.Length() - startIndex); }
            static int64_t IndexOf(const Array<T> &ar, const T &item, const int64_t startIndex, const int64_t count);

            inline static int LastIndexOf(const Array<T> &ar, const T &item) { return LastIndexOf(ar, item, 0, ar.Length()); }
            inline static int LastIndexOf(const Array<T> &ar, const T &item, const int64_t startIndex) { return LastIndexOf(ar, item, startIndex, ar.Length() - startIndex); }
            static int64_t LastIndexOf(const Array<T> &ar, const T &item, const int64_t startIndex, const int64_t count);

            static void Copy(const T *source, T *destination, const size_t count);
            static void Copy(const Array<T> &source, const size_t sourceIndex, Array<T> &destination, const size_t destinationIndex, const size_t count);
            static void Move(const T *source, T *destination, const size_t count);
            static void Move(const Array<T> &source, const size_t sourceIndex, Array<T> &destination, const size_t destinationIndex, const size_t count);
            static void Clear(Array<T> &arr, const size_t index, const size_t count);
            static void Reverse(Array<T> &arr, const size_t index, const size_t length);
        };

        //////////////////////////////////////////////////////// Array ////////////////////////////////////////////////////////

        template <typename T>
        Array<T>::Array(const int64_t length)
            : m_array(nullptr)
            , m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length");
            }

            if(length > 0)
            {
                if(std::is_trivially_constructible<T>::value)
                {
                    m_array = reinterpret_cast<T*>(DNN_CAlloc(length, sizeof(T)));
                }
                else
                {
                    m_array = reinterpret_cast<T*>(DNN_Alloc(sizeof(T) * length));

                    for(int64_t i = 0; i < length; ++i)
                    {
                        new (m_array + i) T();
                    }
                }
            }
        }

        template <typename T>
        Array<T>::Array(const T *arr, const int64_t length)
            : m_array(nullptr)
            , m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length");
            }

            if(!arr)
            {
                throw ArgumentNullException("arr");
            }

            if(m_length > 0)
            {
                if(std::is_trivially_constructible<T>::value)
                {
                    m_array = reinterpret_cast<T*>(DNN_CAlloc(length, sizeof(T)));
                }
                else
                {
                    m_array = reinterpret_cast<T*>(DNN_Alloc(sizeof(T) * length));

                    for(int64_t i = 0; i < length; ++i)
                    {
                        new (m_array + i) T();
                    }
                }

                Copy(arr, m_array.get(), m_length);
            }
        }

        template <typename T>
        Array<T>::Array(const Array<T> &copy)
            : m_length(copy.m_length)
        {
            if(length > 0)
            {
                if(std::is_trivially_constructible<T>::value)
                {
                    m_array = reinterpret_cast<T*>(DNN_CAlloc(length, sizeof(T)));
                }
                else
                {
                    m_array = reinterpret_cast<T*>(DNN_Alloc(sizeof(T) * length));

                    for(int64_t i = 0; i < length; ++i)
                    {
                        new (m_array + i) T();
                    }
                }

                Copy(copy.m_array.get(), m_array.get(), length);
            }
        }

        template <typename T>
        Array<T>::Array(Array<T> &&mov) noexcept
            : m_array(mov.m_array)
            , m_length(mov.m_length)
        {
            mov.m_array = nullptr;
            mov.m_length = 0;
        }

        template <typename T>
        Array<T>::~Array()
        {
            if(m_array)
            {
                if(!std::is_trivially_destructible<T>::value)
                {
                    for(int64_t i = 0; i < m_length; ++i)
                    {
                        (m_array + i)->~T();
                    }
                }

                DNN_Free(m_array);
                m_array = nullptr;
            }

            m_length = 0;
        }

        template <typename T>
        Array<T>& Array<T>::operator=(const Array<T> &copy)
        {
            if(this != &copy)
            {
                if(copy.m_length == m_length)
                {
                    Copy(copy.m_array, m_array, m_length);
                }
                else
                {
                    if(m_array)
                    {
                        if(!std::is_trivially_destructible<T>::value)
                        {
                            for(int64_t i = 0; i < m_length; ++i)
                            {
                                (m_array + i)->~T();
                            }
                        }

                        DNN_Free(m_array);
                        m_array = nullptr;
                    }

                    m_length = copy.m_length;

                    if(m_length > 0)
                    {
                        if(std::is_trivially_constructible<T>::value)
                        {
                            m_array = reinterpret_cast<T*>(DNN_CAlloc(length, sizeof(T)));
                        }
                        else
                        {
                            m_array = reinterpret_cast<T*>(DNN_Alloc(sizeof(T) * length));

                            for(int64_t i = 0; i < length; ++i)
                            {
                                new (m_array + i) T();
                            }
                        }

                        Copy(copy.m_array, m_array, m_length);
                    }
                }
            }

            return *this;
        }

        template <typename T>
        Array<T>& Array<T>::operator=(Array<T> &&mov) noexcept
        {
            if(this != &mov)
            {
                m_array = mov.m_array;
                m_length = mov.m_length;

                mov.m_array = nullptr;
                mov.m_length = 0;
            }

            return *this;
        }

        template <typename T>
        const T& Array<T>::operator[](const int64_t index) const &
        {
            if(index < 0 || index >= m_length)
            {
                throw IndexOutOfRangeException("index");
            }

            return m_array[index];
        }

        template <typename T>
        T& Array<T>::operator[](const int64_t index) &
        {
            if(index < 0 || index >= m_length)
            {
                throw IndexOutOfRangeException("index");
            }

            return m_array[index];
        }

        template <typename T>
        void Array<T>::Copy(const T *source, T *destination, const size_t count)
        {
            if(count < 0)
            {
                throw ArgumentOutOfRangeException("count");
            }

            if(!destination)
            {
                throw ArgumentNullException("destination");
            }

            if(!source)
            {
                throw ArgumentNullException("source");
            }

            if(std::is_trivially_copyable<T>::value)
            {
                const size_t size = sizeof(T) * count;

                memcpy_s(destination, size, source, size);
            }
            else
            {
                for(size_t i = 0; i < count; ++i)
                {
                    destination[i] = source[i];
                }
            }
        }

        template <typename T>
        void Array<T>::Copy(const Array<T> &source, const size_t sourceIndex, Array<T> &destination, const size_t destinationIndex, const size_t count)
        {
            if(sourceIndex + count > static_cast<size_t>(source.Length()))
            {
                throw ArgumentOutOfRangeException("sourceIndex + count > source.Length()");
            }

            if(destinationIndex + count > static_cast<size_t>(destination.Length()))
            {
                throw ArgumentOutOfRangeException("destinationIndex + count > destination.Length()");
            }

            if(count == 0)
            {
                return;
            }

            if(std::is_trivially_copyable<T>::value)
            {
                const size_t size = sizeof(T) * count;

                memcpy_s(destination.m_array + destinationIndex, size, source.m_array + sourceIndex, size);
            }
            else
            {
                T *destPtr = destination.m_array + destinationIndex;
                const T *srcPtr = source.m_array + sourceIndex;

                for(size_t i = 0; i < count; ++i)
                {
                    destPtr[i] = srcPtr[i];
                }
            }
        }

        template <typename T>
        void Array<T>::Move(const T *source, T *destination, const size_t count)
        {
            if(count < 0)
            {
                throw ArgumentOutOfRangeException("count");
            }

            if(!destination)
            {
                throw ArgumentNullException("destination");
            }

            if(!source)
            {
                throw ArgumentNullException("source");
            }

            if(std::is_trivially_move_assignable<T>::value)
            {
                const size_t size = sizeof(T) * count;

                memcpy_s(destination, size, source, size);
            }
            else
            {
                for(size_t i = 0; i < count; ++i)
                {
                    destination[i] = std::move(source[i]);
                }
            }
        }

        template <typename T>
        void Array<T>::Move(const Array<T> &source, const size_t sourceIndex, Array<T> &destination, const size_t destinationIndex, const size_t count)
        {
            if(sourceIndex + count > static_cast<size_t>(source.Length()))
            {
                throw ArgumentOutOfRangeException("sourceIndex + count > source.Length()");
            }

            if(destinationIndex + count > static_cast<size_t>(destination.Length()))
            {
                throw ArgumentOutOfRangeException("destinationIndex + count > destination.Length()");
            }

            if(count == 0)
            {
                return;
            }

            if(std::is_trivially_move_assignable<T>::value)
            {
                const size_t size = sizeof(T) * count;

                memcpy_s(destination.m_array + destinationIndex, size, source.m_array + sourceIndex, size);
            }
            else
            {
                T *destPtr = destination.m_array + destinationIndex;
                const T *srcPtr = source.m_array + sourceIndex;

                for(size_t i = 0; i < count; ++i)
                {
                    destPtr[i] = std::move(srcPtr[i]);
                }
            }
        }

        template <typename T>
        unique_ptr<Collections::IEnumerator<T>> Array<T>::GetEnumerator()
        {
            static int cookie = 0;

            return unique_ptr<Collections::IEnumerator<T>>(DNN_New Collections::GenericEnumerator<T>(m_array, m_length, &cookie));
        }

        template <typename T>
        void Array<T>::Clear(Array<T> &arr, const size_t index, const size_t count)
        {
            if(index + count > static_cast<size_t>(arr.Length()))
            {
                throw ArgumentOutOfRangeException("index + count > arr.Length()");
            }

            if(!std::is_trivially_destructible<T>::value)
            {
                T *arrOffset = arr.m_array + index;

                for(size_t i = 0; i < count; ++i)
                {
                    T *elem = arrOffset + i;

                    elem->~T();
                    new (elem) T();
                }
            }
        }

        // Returns the index of a particular item, if it is in the list.
        // Returns -1 if the item isn't in the list.
        template <typename T>
        int64_t Array<T>::IndexOf(const Array<T> &ar, const T &item, const int64_t startIndex, const int64_t count)
        {
            if(startIndex < 0)
            {
                throw IndexOutOfRangeException("startIndex is out of range.");
            }

            if(startIndex + count > ar.Length())
            {
                throw IndexOutOfRangeException("startIndex + count is out of range.");
            }

            const T *ptr = ar;

            ptr += startIndex;

            for(int64_t i = 0; i < count; ++i)
            {
                if(Internal::ObjectHelper<T>::Equals(item, ptr[i]))
                {
                    return i;
                }
            }

            return -1;
        }

        template <typename T>
        int64_t Array<T>::LastIndexOf(const Array<T> &ar, const T &item, const int64_t startIndex, const int64_t count)
        {
            if(startIndex < 0)
            {
                throw IndexOutOfRangeException("startIndex is out of range.");
            }

            if(startIndex + count > ar.Length())
            {
                throw IndexOutOfRangeException("startIndex + count is out of range.");
            }

            const T *ptr = ar;

            ptr += startIndex;

            // To maintain some type of cache coherency without iterating over every element we walk backwards in blocks
            // of forward searched items
            constexpr int64_t blockSize = 128;

            int64_t baseIndex = count;
            int64_t foundIndex = -1;
            int64_t blockCount;

            do
            {
                baseIndex -= blockSize;

                if(baseIndex < 0)
                {
                    blockCount = -baseIndex;
                    baseIndex = 0;
                }
                else
                {
                    blockCount = blockSize;
                }

                for(int64_t i = 0; i < blockCount; ++i)
                {
                    const int64_t index = baseIndex + i;

                    if(Internal::ObjectHelper<T>::Equals(item, ptr[index]))
                    {
                        foundIndex = index + startIndex;
                    }
                }


            } while(foundIndex == -1 && baseIndex > 0);

            return foundIndex;
        }

        template <typename T>
        void Array<T>::Reverse(Array<T> &arr, const size_t index, const size_t length)
        {
            if(index + length > arr.Length())
            {
                throw ArgumentException("Invalid offset + length.");
            }

            if(length <= 1)
            {
                return;
            }

            size_t i = index;
            size_t j = index + length - 1;

            while(i < j)
            {
                T temp = std::move(arr.m_array[i]);

                arr.m_array[i] = std::move(arr.m_array[j]);
                arr.m_array[j] = std::move(temp);

                ++i;
                --j;
            }
        }
    }
}

#endif