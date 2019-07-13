#ifndef _DOTNETNATIVE_SYSTEM_ARRAY_H_
#define _DOTNETNATIVE_SYSTEM_ARRAY_H_

#include "../MemoryUtil.h"
#include "Object.h"
#include "Exception.h"
#include "IReadOnlyList.h"
#include "GenericEnumerator.h"

namespace DotNetNative
{
    namespace System
    {
        template <typename T>
        class Array
            : public Object
            , public IReadOnlyList<T>
        {
        private:
            unique_ptr<T[]> m_array;
            int64_t         m_length;

        public:
            Array() : m_length(0) {}
            Array(const int64_t length);
            Array(const T *arr, const int64_t length);
            Array(const Array<T> &copy);
            Array(Array<T> &&mov) noexcept;
            virtual ~Array() {}

            Array<T>& operator=(const Array<T> &copy);
            Array<T>& operator=(Array<T> &&mov) noexcept;

            virtual const T& operator[](const int64_t index) const & override;
            T& operator[](const int64_t index) &;

            virtual int64_t Count() const override { return m_length; }
            inline int64_t Length() const noexcept { return m_length; }
            inline T* operator T*() const noexcept { return m_array.get(); }

            //
            // Summary:
            //     Returns an enumerator that iterates through the collection.
            //
            // Returns:
            //     An enumerator that can be used to iterate through the collection.
            virtual unique_ptr<IEnumerator<T>> GetEnumerator() override;

            static void Copy(T *destination, const T *source, const size_t count);
        };

        //////////////////////////////////////////////////////// Array ////////////////////////////////////////////////////////

        template <typename T>
        Array<T>::Array(const int64_t length)
            : m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException();
            }

            if(length > 0)
            {
                m_array = make_unique<T[]>(length);
            }
        }

        template <typename T>
        Array<T>::Array(const T *arr, const int64_t length)
            : m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException();
            }

            if(!arr)
            {
                throw ArgumentNullException();
            }

            if(m_length > 0)
            {
                m_array = make_unique<T[]>(m_length);

                Copy(m_array.get(), arr, m_length);
            }
        }

        template <typename T>
        Array<T>::Array(const Array<T> &copy)
            : m_length(copy.m_length)
        {
            if(length > 0)
            {
                m_array = make_unique<T[]>(m_length);

                Copy(m_array.get(), copy.m_array.get(), length);
            }
        }

        template <typename T>
        Array<T>::Array(Array<T> &&mov) noexcept
            : m_array(std::move(mov.m_array))
            , m_length(mov.m_length)
        {
            mov.m_length = 0;
        }

        template <typename T>
        Array<T>& Array<T>::operator=(const Array<T> &copy)
        {
            if(this != &copy)
            {
                m_array.reset();
                m_length = copy.m_length;

                if(m_length > 0)
                {
                    m_array = make_unique<T[]>(m_length);

                    Copy(m_array.get(), copy.m_array.get(), length);
                }
            }

            return *this;
        }

        template <typename T>
        Array<T>& Array<T>::operator=(Array<T> &&mov) noexcept
        {
            if(this != &mov)
            {
                m_array = std::move(mov.m_array);
                m_length = mov.m_length;

                mov.m_length = 0;
            }

            return *this;
        }

        template <typename T>
        const T& Array<T>::operator[](const int64_t index) const &
        {
            if(index < 0 || index >= m_length)
            {
                throw IndexOutOfRangeException();
            }

            return m_array.get()[index];
        }

        template <typename T>
        T& Array<T>::operator[](const int64_t index) &
        {
            if(index < 0 || index >= m_length)
            {
                throw IndexOutOfRangeException();
            }

            return m_array.get()[index];
        }

        template <typename T>
        void Array<T>::Copy(T *destination, const T *source, const size_t count)
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
                for(int64_t i = 0; i < length; ++i)
                {
                    destination[i] = source[i];
                }
            }
        }

        template <typename T>
        unique_ptr<IEnumerator<T>> Array<T>::GetEnumerator()
        {
            static int cookie = 0;

            return unique_ptr<IEnumerator<T>>(DNN_New GenericEnumerator<T>(m_string, m_length, &cookie));
        }
    }
}

#endif