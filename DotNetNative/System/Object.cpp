#include "Object.h"
#include "String.h"

namespace DotNetNative
{
    namespace System
    {
        bool Object::Equals(const Object &obj) const noexcept
        {
            return this == &obj;
        }

        String Object::ToString()
        {
            return String("System.Object");
        }

        int Object::GetHashCode() const
        {
            ptrdiff_t ptrVal = reinterpret_cast<ptrdiff_t>(this);

            if(sizeof(void*) >= 8)
            {
                return static_cast<int>((ptrVal & 0xffffffff) ^ ((ptrVal >> 32) & 0xffffffff));
            }

            return static_cast<int>(ptrVal);
        }

        template <>
        String Object::ToString<Object>(Object &obj)
        {
            return obj.ToString();
        }

        template <>
        String Object::ToString<Object*>(Object *&obj)
        {
            if(obj)
            {
                return obj->ToString();
            }

            return String("null");
        }

        template <>
        String Object::ToString<char*>(char *&obj)
        {
            if(obj)
            {
                return String(obj);
            }

            return String("null");
        }

        template <>
        String Object::ToString<char>(char &obj)
        {
            return String(&obj, 1);
        }

        template <>
        String Object::ToString<utf16char*>(utf16char *&obj)
        {
            if(obj)
            {
                return String(obj);
            }

            return String("null");
        }

        template <>
        String Object::ToString<utf16char>(utf16char &obj)
        {
            return String(&obj, 1);
        }

        template <>
        String Object::ToString<unsigned char>(unsigned char &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<unsigned char*>(unsigned char *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<short>(short &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<short*>(short *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<unsigned int>(unsigned int &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<unsigned int*>(unsigned int *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<int>(int &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<int*>(int *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<unsigned long long>(unsigned long long &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<unsigned long long*>(unsigned long long *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<long long>(long long &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<long long*>(long long *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<float>(float &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<float*>(float *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }

        template <>
        String Object::ToString<double>(double &obj)
        {
            throw NotImplementedException();
        }

        template <>
        String Object::ToString<double*>(double *&obj)
        {
            if(obj)
            {
                throw NotImplementedException();
            }

            return String("null");
        }
    }
}