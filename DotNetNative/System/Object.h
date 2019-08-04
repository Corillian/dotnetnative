#ifndef _DOTNETNATIVE_SYSTEM_OBJECT_H_
#define _DOTNETNATIVE_SYSTEM_OBJECT_H_

#include "Exception.h"

namespace DotNetNative
{
    namespace System
    {
        class String;

        class Object
        {
        public:
            virtual ~Object() {}

            virtual bool Equals(const Object &obj) const noexcept;
            virtual String ToString();
            virtual int GetHashCode() const;

            template <typename T>
            static String ToString(T &obj);

            template <typename T>
            static int GetHashCode(const T &obj);
        };
    }
}
#endif