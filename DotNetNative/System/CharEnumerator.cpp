#include "CharEnumerator.h"
#include "Exception.h"

namespace DotNetNative
{
    namespace System
    {
        CharEnumerator::CharEnumerator(const shared_ptr<utf16char[]> &chars, const int length)
            : m_chars(chars)
            , m_length(length)
            , m_index(-1)
            , m_current(0)
        {
            if(!chars && length > 0)
            {
                throw ArgumentNullException();
            }

            if(length < 0)
            {
                throw ArgumentOutOfRangeException();
            }
        }

        CharEnumerator::CharEnumerator(const CharEnumerator &copy)
            : m_chars(copy.m_chars)
            , m_length(copy.m_length)
            , m_index(copy.m_index)
            , m_current(copy.m_current)
        {
        }

        CharEnumerator::CharEnumerator(CharEnumerator &&mov) noexcept
            : m_chars(std::move(mov.m_chars))
            , m_length(mov.m_length)
            , m_index(mov.m_index)
            , m_current(mov.m_current)
        {
            mov.m_length = 0;
            mov.m_index = -1;
            mov.m_current = 0;
        }

        CharEnumerator& CharEnumerator::operator=(const CharEnumerator &copy)
        {
            if(this != &copy)
            {
                m_chars = copy.m_chars;
                m_length = copy.m_length;
                m_index = copy.m_index;
                m_current = copy.m_current;
            }

            return *this;
        }

        CharEnumerator& CharEnumerator::operator=(CharEnumerator &&mov) noexcept
        {
            if(this != &mov)
            {
                m_chars = std::move(mov.m_chars);
                m_length = mov.m_length;
                m_index = mov.m_index;
                m_current = mov.m_current;

                mov.m_length = 0;
                mov.m_index = -1;
                mov.m_current = 0;
            }

            return *this;
        }

        //
        // Summary:
        //     Gets the element in the collection at the current position of the enumerator.
        //
        // Returns:
        //     The element in the collection at the current position of the enumerator.
        const utf16char& CharEnumerator::Current() const &
        {
            if(m_index < 0)
            {
                throw new InvalidOperationException("Enumeration not started.");
            }

            if(m_index >= m_length)
            {
                throw new InvalidOperationException("Enumeration has ended.");
            }

            return m_current;
        }

        //
        // Summary:
        //     Gets the element in the collection at the current position of the enumerator.
        //
        // Returns:
        //     The element in the collection at the current position of the enumerator.
        utf16char& CharEnumerator::Current() &
        {
            if(m_index < 0)
            {
                throw new InvalidOperationException("Enumeration not started.");
            }

            if(m_index >= m_length)
            {
                throw new InvalidOperationException("Enumeration has ended.");
            }

            return m_current;
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
        bool CharEnumerator::MoveNext()
        {
            if(m_index < m_length - 1)
            {
                ++m_index;
                m_current = m_chars.get()[m_index];
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
        void CharEnumerator::Reset()
        {
            m_index = -1;
            m_current = 0;
        }
    }
}