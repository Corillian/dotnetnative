#include "String.h"
#include "Exception.h"
#include "CharEnumerator.h"

namespace DotNetNative
{
    namespace System
    {
        String::String() noexcept
            : m_length(0)
        {
        }

        String::String(const shared_ptr<utf16char[]> &str, const int length)
            : m_string(str)
            , m_length(length)
        {
        }

        String::String(shared_ptr<utf16char[]> &&str, const int length)
            : m_string(std::move(str))
            , m_length(length)
        {
        }

        String::String(const utf16char *str)
            : m_length(0)
        {
            if(!str)
            {
                throw ArgumentNullException();
            }

            while(str[m_length])
            {
                ++m_length;
            }

            m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

            memcpy_s(m_string.get(), sizeof(utf16char) * (static_cast<size_t>(m_length) + 1), str, sizeof(utf16char) * m_length);

            m_string.get()[m_length] = 0;
        }

        String::String(const utf16char *str, const int length)
            : m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException();
            }

            if(!str)
            {
                throw ArgumentNullException();
            }

            if(m_length > 0)
            {
                m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

                memcpy_s(m_string.get(), sizeof(utf16char) * (static_cast<size_t>(m_length) + 1), str, sizeof(utf16char) * m_length);

                m_string.get()[m_length] = 0;
            }
        }

        String::String(const char *str)
            : m_length(0)
        {
            if(!str)
            {
                throw ArgumentNullException();
            }

            while(str[m_length])
            {
                ++m_length;
            }

            m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

            for(int i = 0; i < m_length; ++i)
            {
                // We assume ASCII
                m_string.get()[i] = static_cast<utf16char>(str[i]);
            }

            m_string.get()[m_length] = 0;
        }

        String::String(const char *str, const int length)
            : m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException();
            }

            if(!str)
            {
                throw ArgumentNullException();
            }

            if(m_length > 0)
            {
                m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

                for(int i = 0; i < m_length; ++i)
                {
                    // We assume ASCII
                    m_string.get()[i] = static_cast<utf16char>(str[i]);
                }

                m_string.get()[m_length] = 0;
            }
        }
        
        String::String(const Char *str)
            : m_length(0)
        {
            if(!str)
            {
                throw ArgumentNullException();
            }

            while(static_cast<utf16char>(str[m_length]))
            {
                ++m_length;
            }

            m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

            for(int i = 0; i < m_length; ++i)
            {
                m_string.get()[i] = static_cast<utf16char>(str[i]);
            }

            m_string.get()[m_length] = 0;
        }

        String::String(const Char *str, const int length)
            : m_length(length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException();
            }

            if(!str)
            {
                throw ArgumentNullException();
            }

            if(m_length > 0)
            {
                m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

                for(int i = 0; i < m_length; ++i)
                {
                    // We assume ASCII
                    m_string.get()[i] = static_cast<utf16char>(str[i]);
                }

                m_string.get()[m_length] = 0;
            }
        }

        String::String(const String &copy)
            : m_string(copy.m_string)
            , m_length(copy.m_length)
        {
        }

        String::String(String &&mov) noexcept
            : m_string(std::move(mov.m_string))
            , m_length(mov.m_length)
        {
            mov.m_length = 0;
        }

        String& String::operator=(const String &copy)
        {
            if(this != &copy)
            {
                m_length = copy.m_length;
                m_string = copy.m_string;
            }

            return *this;
        }

        String& String::operator=(String &&mov) noexcept
        {
            if(this != &mov)
            {
                m_string = std::move(mov.m_string);
                m_length = mov.m_length;

                mov.m_length = 0;
            }

            return *this;
        }

        String::operator const utf16char*() const noexcept
        {
            return static_cast<const utf16char*>(m_string.get());
        }

        utf16char String::operator[](const int index) const
        {
            if(index < 0 && index >= m_length)
            {
                throw IndexOutOfRangeException();
            }

            return m_string.get()[index];
        }

        unique_ptr<Collections::IEnumerator<utf16char>> String::GetEnumerator()
        {
            return unique_ptr<Collections::IEnumerator<utf16char>>(DNN_New CharEnumerator(m_string, m_length));
        }

        bool String::Equals(const String &obj) const noexcept
        {
            if(this == &obj)
            {
                return true;
            }

            if(m_length != obj.m_length)
            {
                return false;
            }

            if(m_length == 0)
            {
                return true;
            }

            return memcmp(m_string.get(), obj.m_string.get(), sizeof(utf16char) * m_length) == 0;
        }

        String String::ToString()
        {
            return *this;
        }

        bool String::IsNullOrEmpty(const String &str)
        {
            return 0u >= static_cast<unsigned int>(str.Length());
        }

        bool String::IsNullOrWhiteSpace(const String &str)
        {
            // avoid the bounds checking for each iteration
            const utf16char *strPtr = str;

            for(int i = 0; i < str.Length(); ++i)
            {
                if(!Char::IsWhiteSpace(strPtr[i]))
                {
                    return false;
                }
            }

            return true;
        }

        bool operator==(const String &str1, const String &str2)
        {
            return str1.Equals(str2);
        }

        bool operator==(const char *str1, const String &str2)
        {
            return str2 == str1;
        }

        bool operator==(const String &str1, const char *str2)
        {
            if(!str2 || !*str2)
            {
                return str1.Length() == 0;
            }

            if(str1.Length() == 0)
            {
                return false;
            }

            const utf16char *str1Ptr = str1.m_string.get();

            while(true)
            {
                if(*str1Ptr == 0)
                {
                    return *str2 == 0;
                }

                if(*str2 == 0)
                {
                    return *str1Ptr == 0;
                }

                if(*str1Ptr != *str2)
                {
                    return false;
                }

                ++str1Ptr;
                ++str2;
            }

            // This should be inaccessible
            return true;
        }

        bool operator==(const utf16char *str1, const String &str2)
        {
            return str2 == str1;
        }

        bool operator==(const String &str1, const utf16char *str2)
        {
            if(!str2 || !*str2)
            {
                return str1.Length() == 0;
            }

            if(str1.Length() == 0)
            {
                return false;
            }

            const utf16char *str1Ptr = str1.m_string.get();

            while(true)
            {
                if(*str1Ptr == 0)
                {
                    return *str2 == 0;
                }

                if(*str2 == 0)
                {
                    return *str1Ptr == 0;
                }

                if(*str1Ptr != *str2)
                {
                    return false;
                }

                ++str1Ptr;
                ++str2;
            }

            // This should be inaccessible
            return true;
        }
    }
}