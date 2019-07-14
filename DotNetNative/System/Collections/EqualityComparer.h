#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_EQUALITYCOMPARER_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_EQUALITYCOMPARER_H_

#include "IEqualityComparer.h"
#include "../IEquatable.h"
#include "../Object.h"
#include "../../GlobalDefs.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename T>
            class EqualityComparer
                : public virtual Object
                , public virtual IEqualityComparer<T>
            {
            public:
                EqualityComparer() = default;
                EqualityComparer(const EqualityComparer<T> &copy) = default;
                EqualityComparer(EqualityComparer<T> &&mov) noexcept = default;
                virtual ~EqualityComparer() {}
            };

            template <typename T>
            class GenericEqualityComparer
                : public virtual EqualityComparer<T>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<T> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<T> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<T> &other) const noexcept { return true; }
                virtual bool Equals(const T &x, const T &y) const noexcept override;
                virtual int GetHashCode(const T &obj) const override;
            };

            /*template <typename T>
            class GenericEqualityComparer
                : public virtual EqualityComparer<T*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<T*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<T*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<T*> &other) const noexcept { return true; }
                virtual bool Equals(const T *&x, const T *&y) const noexcept override
                {
                    return x == y || (x && y && x->Equals(*y));
                }

                virtual int GetHashCode(const T *&obj) const override
                {
                    return obj ? obj->GetHashCode() : 0;
                }
            };*/

            template <>
            class GenericEqualityComparer<int8_t>
                : public virtual EqualityComparer<int8_t>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<int8_t> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<int8_t> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<int8_t> &other) const noexcept { return true; }
                virtual bool Equals(const int8_t &x, const int8_t &y) const noexcept { return x == y; }
                virtual int GetHashCode(const int8_t &obj) const override { return static_cast<int>(HashTrivialObject32<int8_t>(obj)); }
            };

            template <>
            class GenericEqualityComparer<int8_t*>
                : public virtual EqualityComparer<int8_t*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<int8_t*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<int8_t*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<int8_t*> &other) const noexcept { return true; }
                virtual bool Equals(int8_t *const &x, int8_t *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(int8_t *const &obj) const override { return static_cast<int>(HashTrivialObject32<int8_t*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint8_t>
                : public virtual EqualityComparer<uint8_t>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint8_t> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint8_t> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint8_t> &other) const noexcept { return true; }
                virtual bool Equals(const uint8_t &x, const uint8_t &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const uint8_t &obj) const override { return static_cast<int>(HashTrivialObject32<uint8_t>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint8_t*>
                : public virtual EqualityComparer<uint8_t*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint8_t*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint8_t*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint8_t*> &other) const noexcept { return true; }
                virtual bool Equals(uint8_t *const &x, uint8_t *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(uint8_t *const &obj) const override { return static_cast<int>(HashTrivialObject32<uint8_t*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<int16_t>
                : public virtual EqualityComparer<int16_t>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<int16_t> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<int16_t> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<int16_t> &other) const noexcept { return true; }
                virtual bool Equals(const int16_t &x, const int16_t &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const int16_t &obj) const override { return static_cast<int>(HashTrivialObject32<int16_t>(obj)); }
            };

            template <>
            class GenericEqualityComparer<int16_t*>
                : public virtual EqualityComparer<int16_t*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<int16_t*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<int16_t*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<int16_t*> &other) const noexcept { return true; }
                virtual bool Equals(int16_t *const &x, int16_t *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(int16_t *const &obj) const override { return static_cast<int>(HashTrivialObject32<int16_t*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<utf16char>
                : public virtual EqualityComparer<utf16char>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<utf16char> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<utf16char> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<utf16char> &other) const noexcept { return true; }
                virtual bool Equals(const utf16char &x, const utf16char &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const utf16char &obj) const override { return static_cast<int>(HashTrivialObject32<utf16char>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint16_t*>
                : public virtual EqualityComparer<utf16char*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint16_t*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint16_t*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint16_t*> &other) const noexcept { return true; }
                virtual bool Equals(uint16_t *const &x, uint16_t *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(uint16_t *const &obj) const override { return static_cast<int>(HashTrivialObject32<uint16_t*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<int>
                : public virtual EqualityComparer<int>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<int> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<int> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<int> &other) const noexcept { return true; }
                virtual bool Equals(const int &x, const int &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const int &obj) const override { return static_cast<int>(HashTrivialObject32<int>(obj)); }
            };

            template <>
            class GenericEqualityComparer<int*>
                : public virtual EqualityComparer<int*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<int*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<int*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<int*> &other) const noexcept { return true; }
                virtual bool Equals(int *const &x, int *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(int *const &obj) const override { return static_cast<int>(HashTrivialObject32<int*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint32_t>
                : public virtual EqualityComparer<uint32_t>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint32_t> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint32_t> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint32_t> &other) const noexcept { return true; }
                virtual bool Equals(const uint32_t &x, const uint32_t &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const uint32_t &obj) const override { return static_cast<int>(HashTrivialObject32<uint32_t>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint32_t*>
                : public virtual EqualityComparer<uint32_t*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint32_t*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint32_t*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint32_t*> &other) const noexcept { return true; }
                virtual bool Equals(uint32_t *const &x, uint32_t *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(uint32_t *const &obj) const override { return static_cast<int>(HashTrivialObject32<uint32_t*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<long long>
                : public virtual EqualityComparer<long long>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<long long> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<long long> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<long long> &other) const noexcept { return true; }
                virtual bool Equals(const long long &x, const long long &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const long long &obj) const override { return static_cast<int>(HashTrivialObject32<long long>(obj)); }
            };

            template <>
            class GenericEqualityComparer<long long*>
                : public virtual EqualityComparer<long long*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<long long*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<long long*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<long long*> &other) const noexcept { return true; }
                virtual bool Equals(long long *const &x, long long *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(long long *const &obj) const override { return static_cast<int>(HashTrivialObject32<long long*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint64_t>
                : public virtual EqualityComparer<uint64_t>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint64_t> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint64_t> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint64_t> &other) const noexcept { return true; }
                virtual bool Equals(const uint64_t &x, const uint64_t &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const uint64_t &obj) const override { return static_cast<int>(HashTrivialObject32<uint64_t>(obj)); }
            };

            template <>
            class GenericEqualityComparer<uint64_t*>
                : public virtual EqualityComparer<uint64_t*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<uint64_t*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<uint64_t*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<uint64_t*> &other) const noexcept { return true; }
                virtual bool Equals(uint64_t *const &x, uint64_t *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(uint64_t *const &obj) const override { return static_cast<int>(HashTrivialObject32<uint64_t*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<float>
                : public virtual EqualityComparer<float>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<float> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<float> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<float> &other) const noexcept { return true; }
                virtual bool Equals(const float &x, const float &y) const noexcept override { return std::abs(x - y) <= FLT_EPSILON; }
                virtual int GetHashCode(const float &obj) const override { return static_cast<int>(HashTrivialObject32<float>(obj)); }
            };

            template <>
            class GenericEqualityComparer<float*>
                : public virtual EqualityComparer<float*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<float*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<float*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<float*> &other) const noexcept { return true; }
                virtual bool Equals(float *const &x, float *const &y) const noexcept override { return x == y || (x && y && std::abs(*x - *y) <= FLT_EPSILON); }
                virtual int GetHashCode(float *const &obj) const override { return static_cast<int>(HashTrivialObject32<float*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<double>
                : public virtual EqualityComparer<double>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<double> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<double> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<double> &other) const noexcept { return true; }
                virtual bool Equals(const double &x, const double &y) const noexcept override { return std::abs(x - y) <= DBL_EPSILON; }
                virtual int GetHashCode(const double &obj) const override { return static_cast<int>(HashTrivialObject32<double>(obj)); }
            };

            template <>
            class GenericEqualityComparer<double*>
                : public virtual EqualityComparer<double*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<double*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<double*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<double*> &other) const noexcept { return true; }
                virtual bool Equals(double *const &x, double *const &y) const noexcept override { return x == y || (x && y && std::abs(*x - *y) <= DBL_EPSILON); }
                virtual int GetHashCode(double *const &obj) const override { return static_cast<int>(HashTrivialObject32<double*>(obj)); }
            };

            template <>
            class GenericEqualityComparer<bool>
                : public virtual EqualityComparer<bool>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<bool> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<bool> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<bool> &other) const noexcept { return true; }
                virtual bool Equals(const bool &x, const bool &y) const noexcept override { return x == y; }
                virtual int GetHashCode(const bool &obj) const override { return static_cast<int>(HashTrivialObject32<bool>(obj)); }
            };

            template <>
            class GenericEqualityComparer<bool*>
                : public virtual EqualityComparer<bool*>
            {
            public:
                GenericEqualityComparer() = default;
                GenericEqualityComparer(const GenericEqualityComparer<bool*> &copy) = default;
                GenericEqualityComparer(GenericEqualityComparer<bool*> &&mov) noexcept = default;
                virtual ~GenericEqualityComparer() {}

                constexpr bool Equals(const GenericEqualityComparer<bool*> &other) const noexcept { return true; }
                virtual bool Equals(bool *const &x, bool *const &y) const noexcept override { return x == y || (x && y && *x == *y); }
                virtual int GetHashCode(bool *const &obj) const override { return static_cast<int>(HashTrivialObject32<bool*>(obj)); }
            };

            /////////////////////////////////////////// GenericEqualityComparer ///////////////////////////////////////////

            template <typename T>
            bool GenericEqualityComparer<T>::Equals(const T &x, const T &y) const noexcept
            {
                return x.Equals(y);
            }

            template <typename T>
            int GenericEqualityComparer<T>::GetHashCode(const T &obj) const
            {
                return obj.GetHashCode();
            }
        }
    }
}

#endif