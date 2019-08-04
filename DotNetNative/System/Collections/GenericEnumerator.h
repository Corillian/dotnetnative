#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_GENERICENUMERATOR_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_GENERICENUMERATOR_H_

#include "IEnumerator.h"
#include "../Exception.h"
#include "../Object.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename T>
            class GenericEnumerator
                : public Object
                , public IEnumerator<T>
            {
            private:
                T              *m_elements;
                int            *m_collectionModifiedCookie;
                int64_t         m_length;
                int64_t         m_index;
                int             m_cookie;


            public:
                GenericEnumerator(T *elements, const int64_t length, int *collectionModifiedCookie);
                GenericEnumerator(const GenericEnumerator<T> &copy);
                GenericEnumerator(GenericEnumerator<T> &&mov) noexcept;
                virtual ~GenericEnumerator() {}

                GenericEnumerator<T>& operator=(const GenericEnumerator<T> &copy);
                GenericEnumerator<T>& operator=(GenericEnumerator<T> &&mov) noexcept;

                //
                // Summary:
                //     Gets the element in the collection at the current position of the enumerator.
                //
                // Returns:
                //     The element in the collection at the current position of the enumerator.
                virtual const T& Current() const & override;

                //
                // Summary:
                //     Gets the element in the collection at the current position of the enumerator.
                //
                // Returns:
                //     The element in the collection at the current position of the enumerator.
                virtual T& Current() & override;

                //
                // Summary:
                //     Advances the enumerator to the next element of the collection.
                //
                // Returns:
                //     true if the enumerator was successfully advanced to the next element; false if
                //     the enumerator has passed the end of the collection.
                //
                // Exceptions:
                //   T:System.InvalidOperationException:
                //     The collection was modified after the enumerator was created.
                virtual bool MoveNext() override;

                //
                // Summary:
                //     Sets the enumerator to its initial position, which is before the first element
                //     in the collection.
                //
                // Exceptions:
                //   T:System.InvalidOperationException:
                //     The collection was modified after the enumerator was created.
                virtual void Reset() override;
            };

            ////////////////////////////////////////////////////////////// GenericEnumerator //////////////////////////////////////////////////////////////

            template <typename T>
            GenericEnumerator<T>::GenericEnumerator(T *elements, const int64_t length, int *collectionModifiedCookie)
                : m_elements(elements)
                , m_collectionModifiedCookie(collectionModifiedCookie)
                , m_length(length)
                , m_index(-1)
                , m_cookie(*collectionModifiedCookie)
            {
                if(!elements)
                {
                    throw ArgumentNullException("elements");
                }
            }

            template <typename T>
            GenericEnumerator<T>::GenericEnumerator(const GenericEnumerator<T> &copy)
                : m_elements(copy.m_elements)
                , m_collectionModifiedCookie(copy.m_collectionModifiedCookie)
                , m_length(copy.m_length)
                , m_index(copy.m_index)
                , m_cookie(copy.m_cookie)
            {
            }

            template <typename T>
            GenericEnumerator<T>::GenericEnumerator(GenericEnumerator<T> &&mov) noexcept
                : m_elements(mov.m_elements)
                , m_collectionModifiedCookie(mov.m_collectionModifiedCookie)
                , m_length(mov.m_length)
                , m_index(mov.m_index)
                , m_cookie(mov.m_cookie)
            {
            }

            template <typename T>
            GenericEnumerator<T>& GenericEnumerator<T>::operator=(const GenericEnumerator<T> &copy)
            {
                if(this != &copy)
                {
                    m_elements = copy.m_elements;
                    m_collectionModifiedCookie = copy.m_collectionModifiedCookie;
                    m_length = copy.m_length;
                    m_index = copy.m_index;
                    m_cookie = copy.m_cookie;
                }

                return *this;
            }

            template <typename T>
            GenericEnumerator<T>& GenericEnumerator<T>::operator=(GenericEnumerator<T> &&mov) noexcept
            {
                if(this != &copy)
                {
                    if(this != &copy)
                    {
                        m_elements = mov.m_elements;
                        m_collectionModifiedCookie = mov.m_collectionModifiedCookie;
                        m_length = mov.m_length;
                        m_index = mov.m_index;
                        m_cookie = mov.m_cookie;
                    }
                }

                return *this;
            }

            //
            // Summary:
            //     Gets the element in the collection at the current position of the enumerator.
            //
            // Returns:
            //     The element in the collection at the current position of the enumerator.
            template <typename T>
            const T& GenericEnumerator<T>::Current() const &
            {
                if(m_index < 0)
                {
                    throw new InvalidOperationException("Enumeration not started.");
                }

                if(m_index >= m_length)
                {
                    throw new InvalidOperationException("Enumeration has ended.");
                }

                if(m_cookie != *m_collectionModifiedCookie)
                {
                    throw InvalidOperationException("The collection has been modified.");
                }

                return m_elements[m_index];
            }

            //
            // Summary:
            //     Gets the element in the collection at the current position of the enumerator.
            //
            // Returns:
            //     The element in the collection at the current position of the enumerator.
            template <typename T>
            T& GenericEnumerator<T>::Current() &
            {
                if(m_index < 0)
                {
                    throw new InvalidOperationException("Enumeration not started.");
                }

                if(m_index >= m_length)
                {
                    throw new InvalidOperationException("Enumeration has ended.");
                }

                if(m_cookie != *m_collectionModifiedCookie)
                {
                    throw InvalidOperationException("The collection has been modified.");
                }

                return m_elements[m_index];
            }

            //
            // Summary:
            //     Advances the enumerator to the next element of the collection.
            //
            // Returns:
            //     true if the enumerator was successfully advanced to the next element; false if
            //     the enumerator has passed the end of the collection.
            //
            // Exceptions:
            //   T:System.InvalidOperationException:
            //     The collection was modified after the enumerator was created.
            template <typename T>
            bool GenericEnumerator<T>::MoveNext()
            {
                if(m_index < m_length - 1)
                {
                    ++m_index;
                    return true;
                }
                else
                {
                    m_index = m_length;
                }

                return false;
            }

            //
            // Summary:
            //     Sets the enumerator to its initial position, which is before the first element
            //     in the collection.
            //
            // Exceptions:
            //   T:System.InvalidOperationException:
            //     The collection was modified after the enumerator was created.
            template <typename T>
            void GenericEnumerator<T>::Reset()
            {
                m_index = -1;
            }
        }
    }
}

#endif