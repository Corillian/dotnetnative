#ifndef _DOTNETNATIVE_SYSTEM_STRING_H_
#define _DOTNETNATIVE_SYSTEM_STRING_H_

#include "Char.h"
#include "IEnumerable.h"

namespace DotNetNative
{
    namespace System
    {
        class String
            : public Object
            , public IEnumerable<utf16char>
        {
        private:
            shared_ptr<utf16char[]> m_string;
            int                     m_length;

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

            //
            // Summary:
            //     Returns an enumerator that iterates through the collection.
            //
            // Returns:
            //     An enumerator that can be used to iterate through the collection.
            virtual unique_ptr<IEnumerator<utf16char>> GetEnumerator() override;

            inline int Length() const noexcept { return m_length; }
        };
    }
}

#endif