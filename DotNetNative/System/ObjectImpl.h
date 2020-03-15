#ifndef _DOTNETNATIVE_SYSTEM_OBJECTIMPL_H_
#define _DOTNETNATIVE_SYSTEM_OBJECTIMPL_H_

#include "String.h"
#include <cfloat>
#include <cmath>

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
                static bool Equals(const T &x, const T &y) { throw NotImplementedException(); }
            };

            template <>
            struct ObjectHelper<Object>
            {
                static String ToString(Object &obj) { return obj.ToString(); }
                static int GetHashCode(const Object &obj) { return obj.GetHashCode(); }
                static bool Equals(const Object &x, const Object &y) { return x.Equals(y); }
            };

            template <>
            struct ObjectHelper<Object*>
            {
                static String ToString(Object *&obj) { return obj->ToString(); }
                static int GetHashCode(const Object *&obj) { return obj->GetHashCode(); }
                constexpr static bool Equals(const Object *&x, const Object *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<char>
            {
                static String ToString(char &obj) { return String(&obj, 1); }
                static int GetHashCode(const char &obj) { return static_cast<int>(obj); }
                constexpr static bool Equals(const char &x, const char &y) { return x == y; }
            };

            template <>
            struct ObjectHelper<char*>
            {
                static String ToString(char *&obj) { return String(obj, 1); }
                static int GetHashCode(const char *&obj) { return PointerToHashCode(obj); }
                constexpr static bool Equals(const char *&x, const char *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<unsigned char>
            {
                static String ToString(unsigned char &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned char &obj) { return static_cast<int>(obj); }
                constexpr static bool Equals(const unsigned char &x, const unsigned char &y) { return x == y; }
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
                constexpr static bool Equals(const unsigned char *&x, const unsigned char *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<short>
            {
                static String ToString(short &obj) { throw NotImplementedException(); }
                static int GetHashCode(const short &obj) { return static_cast<int>(obj); }
                constexpr static bool Equals(const short &x, const short &y) { return x == y; }
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
                constexpr static bool Equals(const short *&x, const short *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<unsigned short>
            {
                static String ToString(unsigned short &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned short &obj) { return static_cast<int>(obj); }
                constexpr static bool Equals(const unsigned short &x, const unsigned short &y) { return x == y; }
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
                constexpr static bool Equals(const unsigned short *&x, const unsigned short *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<int>
            {
                static String ToString(int &obj) { throw NotImplementedException(); }
                static int GetHashCode(const int &obj) { return obj; }
                constexpr static bool Equals(const int &x, const int &y) { return x == y; }
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
                constexpr static bool Equals(const int *&x, const int *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<unsigned int>
            {
                static String ToString(unsigned int &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned int &obj) { return static_cast<int>(obj); }
                constexpr static bool Equals(const unsigned int &x, const unsigned int &y) { return x == y; }
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
                constexpr static bool Equals(const unsigned int *&x, const unsigned int *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<long long>
            {
                static String ToString(long long &obj) { throw NotImplementedException(); }
                static int GetHashCode(const long long &obj)
                {
                    return (obj & 0xffffffff) ^ ((obj >> 32) & 0xffffffff);
                }

                constexpr static bool Equals(const long long &x, const long long &y) { return x == y; }
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
                constexpr static bool Equals(const long long *&x, const long long *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<unsigned long long>
            {
                static String ToString(unsigned long long &obj) { throw NotImplementedException(); }
                static int GetHashCode(const unsigned long long &obj)
                {
                    return (obj & 0xffffffff) ^ ((obj >> 32) & 0xffffffff);
                }

                constexpr static bool Equals(const unsigned long long &x, const unsigned long long &y) { return x == y; }
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
                constexpr static bool Equals(const unsigned long long *&x, const unsigned long long *&y) { return x == y; }
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

                constexpr static bool Equals(const float &x, const float &y) { return std::abs(x - y) <= FLT_EPSILON; }
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
                constexpr static bool Equals(const float *&x, const float *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<double>
            {
                static String ToString(double &obj) { throw NotImplementedException(); }
                static int GetHashCode(const double &obj)
                {
                    return (*reinterpret_cast<const unsigned long long*>(&obj) & 0xffffffff) ^ ((*reinterpret_cast<const unsigned long long*>(&obj) >> 32) & 0xffffffff);
                }

                constexpr static bool Equals(const double &x, const double &y) { return std::abs(x - y) <= DBL_EPSILON; }
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
                constexpr static bool Equals(const double *&x, const double *&y) { return x == y; }
            };

            template <>
            struct ObjectHelper<bool>
            {
                static String ToString(bool &obj) { return obj ? String("True") : String("False"); }
                static int GetHashCode(const bool &obj) { return obj; }
                constexpr static bool Equals(const bool &x, const bool &y) { return x == y; }
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
                constexpr static bool Equals(const bool *&x, const bool *&y) { return x == y; }
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

        template <typename T>
        bool Object::Equals(const T &x, const T &y)
        {
            return Internal::ObjectHelper<T>::Equals(x, y);
        }
    }
}

#endif