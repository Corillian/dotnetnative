#ifndef _DOTNETNATIVE_SYSTEM_STRING_H_
#define _DOTNETNATIVE_SYSTEM_STRING_H_

#include "Char.h"
#include "Collections/IEnumerable.h"
#include "Exception.h"

namespace DotNetNative
{
    namespace System
    {
        class String
            : public Object
            , public Collections::IEnumerable<utf16char>
        {
            friend class StringBuilder;
        private:
            shared_ptr<utf16char[]> m_string;
            int                     m_length;

        private:
            String(const shared_ptr<utf16char[]> &str, const int length);
            String(shared_ptr<utf16char[]> &&str, const int length);

        public:
            String() noexcept;
            String(const char *str);
            String(const char *str, const int length);
            String(const utf16char *str);
            String(const utf16char *str, const int length);
            String(const Char *str);
            String(const Char *str, const int length);
            String(const String &copy);
            String(String &&mov) noexcept;
            virtual ~String(){}

            String& operator=(const String &copy);
            String& operator=(String &&mov) noexcept;

            utf16char operator[](const int index) const;
            operator const utf16char*() const noexcept;

            bool Equals(const String &obj) const noexcept;
            virtual String ToString() override;

            //
            // Summary:
            //     Returns an enumerator that iterates through the collection.
            //
            // Returns:
            //     An enumerator that can be used to iterate through the collection.
            virtual unique_ptr<Collections::IEnumerator<utf16char>> GetEnumerator() override;

            inline int Length() const noexcept { return m_length; }

            static bool IsNullOrEmpty(const String &str);
            static bool IsNullOrWhiteSpace(const String &str);

            friend bool operator==(const String &str1, const char *str2);
            friend bool operator==(const String &str1, const utf16char *str2);
        };

        bool operator==(const String &str1, const String &str2);
        bool operator==(const char *str1, const String &str2);
        bool operator==(const String &str1, const char *str2);
        bool operator==(const utf16char *str1, const String &str2);
        bool operator==(const String &str1, const utf16char *str2);
    }
}

#endif