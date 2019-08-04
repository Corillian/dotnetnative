#ifndef _DOTNETNATIVE_SYSTEM_OBJECTIMPL_H_
#define _DOTNETNATIVE_SYSTEM_OBJECTIMPL_H_

#include "String.h"

namespace DotNetNative
{
    namespace System
    {
        //////////////////////////////////////////////////////// Helpers ////////////////////////////////////////////////////////

        namespace Internal
        {
            inline int PointerToHashCode(const void *ptr)
            {
                if(sizeof(ptr) > 4)
                {
                    return (reinterpret_cast<ptrdiff_t>(ptr) & 0xffffffff) ^ ((reinterpret_cast<ptrdiff_t>(ptr) >> 32) & 0xffffffff);
                }

                return static_cast<int>(reinterpret_cast<ptrdiff_t>(ptr));
            }

            template <typename T>
            struct ObjectHelper
            {
                static String ToString(T &obj) { throw NotImplementedException(); }
                static int GetHashCode(const T &obj) { throw NotImplementedException(); }
            };

            template <>
            struct ObjectHelper<Object>
            {
                static String ToString(Object &obj) { return obj.ToString(); }
                static int GetHashCode(const Object &obj) { return obj.GetHashCode(); }
            };

            template <>
            struct ObjectHelper<Object*>
            {
                static String ToString(Object *&obj) { return obj->ToString(); }
                static int GetHashCode(const Object *&obj) { return obj->GetHashCode(); }
            };

            template <>
            struct ObjectHelper<char>
            {
                static String ToString(char &obj) { return String(&obj, 1); }
                static int GetHashCode(const char &obj) { return static_cast<int>(obj); }
            };

            template <>
            struct ObjectHelper<char*>
            {
                static String ToString(char *&obj) { return String(obj, 1); }
                static int GetHashCode(const char *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<unsigned char>
            {
                static String ToString(unsigned char &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned char &obj) { return static_cast<int>(obj); }
            };

            template <>
            struct ObjectHelper<unsigned char*>
            {
                static String ToString(unsigned char *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }
                    
                    return String("null");
                }

                static int GetHashCode(const unsigned char *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<short>
            {
                static String ToString(short &obj) { throw NotImplementedException(); }
                static int GetHashCode(const short &obj) { return static_cast<int>(obj); }
            };

            template <>
            struct ObjectHelper<short*>
            {
                static String ToString(short *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const short *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<unsigned short>
            {
                static String ToString(unsigned short &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned short &obj) { return static_cast<int>(obj); }
            };

            template <>
            struct ObjectHelper<unsigned short*>
            {
                static String ToString(unsigned short *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const unsigned short *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<int>
            {
                static String ToString(int &obj) { throw NotImplementedException(); }
                static int GetHashCode(const int &obj) { return obj; }
            };

            template <>
            struct ObjectHelper<int*>
            {
                static String ToString(int *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const int *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<unsigned int>
            {
                static String ToString(unsigned int &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned int &obj) { return static_cast<int>(obj); }
            };

            template <>
            struct ObjectHelper<unsigned int*>
            {
                static String ToString(unsigned int *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const unsigned int *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<long long>
            {
                static String ToString(long long &obj) { throw NotImplementedException(); }
                static int GetHashCode(const long long &obj)
                {
                    return (obj & 0xffffffff) ^ ((obj >> 32) & 0xffffffff);
                }
            };

            template <>
            struct ObjectHelper<long long*>
            {
                static String ToString(long long *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const long long *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<unsigned long long>
            {
                static String ToString(unsigned long long &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned long long &obj)
                {
                    return (obj & 0xffffffff) ^ ((obj >> 32) & 0xffffffff);
                }
            };

            template <>
            struct ObjectHelper<unsigned long long*>
            {
                static String ToString(unsigned long long *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const unsigned long long *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<float>
            {
                static String ToString(float &obj) { throw NotImplementedException(); }
                static int GetHashCode(const float &obj)
                {
                    static_assert(sizeof(int) == sizeof(float));

                    return *reinterpret_cast<const int*>(&obj);
                }
            };

            template <>
            struct ObjectHelper<float*>
            {
                static String ToString(float *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const float *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<double>
            {
                static String ToString(double &obj) { throw NotImplementedException(); }
                static int GetHashCode(const double &obj)
                {
                    return (*reinterpret_cast<const unsigned long long*>(&obj) & 0xffffffff) ^ ((*reinterpret_cast<const unsigned long long*>(&obj) >> 32) & 0xffffffff);
                }
            };

            template <>
            struct ObjectHelper<double*>
            {
                static String ToString(double *&obj)
                {
                    if(obj)
                    {
                        throw NotImplementedException();
                    }

                    return String("null");
                }

                static int GetHashCode(const double *&obj) { return PointerToHashCode(obj); }
            };

            template <>
            struct ObjectHelper<bool>
            {
                static String ToString(bool &obj) { return obj ? String("True") : String("False"); }
                static int GetHashCode(const bool &obj) { return obj; }
            };

            template <>
            struct ObjectHelper<bool*>
            {
                static String ToString(bool *&obj)
                {
                    if(obj)
                    {
                        return *obj ? String("True") : String("False");
                    }

                    return String("null");
                }

                static int GetHashCode(const bool *&obj) { return PointerToHashCode(obj); }
            };
        }

        //////////////////////////////////////////////////////// Object ////////////////////////////////////////////////////////

        template <typename T>
        String Object::ToString(T &obj)
        {
            return Internal::ObjectHelper<T>::ToString(obj);
        }

        template <typename T>
        int Object::GetHashCode(const T &obj)
        {
            return Internal::ObjectHelper<T>::GetHashCode(obj);
        }
    }
}

#endif