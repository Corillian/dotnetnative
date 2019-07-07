#ifndef _DOTNETNATIVE_SYSTEM_OBJECT_H_
#define _DOTNETNATIVE_SYSTEM_OBJECT_H_

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
        };
    }
}
#endif