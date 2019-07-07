#ifndef _DOTNETNATIVE_SYSTEM_EXCEPTION_H_
#define _DOTNETNATIVE_SYSTEM_EXCEPTION_H_

#include <exception>
#include <stdexcept>

namespace DotNetNative
{
    namespace System
    {
        class Exception : public std::exception
        {
        public:
            Exception() noexcept {}
            Exception(const char *message);
        };

        class SystemException : public Exception
        {
        public:
            SystemException() noexcept {}
            SystemException(const char *message);
        };

        class ArgumentException : public SystemException
        {
        public:
            ArgumentException() noexcept {}
            ArgumentException(const char *message);
        };

        class ArgumentNullException : public SystemException
        {
        public:
            ArgumentNullException() noexcept {}
            ArgumentNullException(const char *message);
        };

        class ArgumentOutOfRangeException : public SystemException
        {
        public:
            ArgumentOutOfRangeException() noexcept {}
            ArgumentOutOfRangeException(const char *message);
        };

        class NullReferenceException : public SystemException
        {
        public:
            NullReferenceException() noexcept {}
            NullReferenceException(const char *message);
        };

        class NotImplementedException : public SystemException
        {
        public:
            NotImplementedException() noexcept {}
            NotImplementedException(const char *message);
        };

        class NotSupportedException : public SystemException
        {
        public:
            NotSupportedException() noexcept {}
            NotSupportedException(const char *message);
        };

        class InvalidOperationException : public SystemException
        {
        public:
            InvalidOperationException() noexcept {}
            InvalidOperationException(const char *message);
        };

        class IndexOutOfRangeException : public SystemException
        {
        public:
            IndexOutOfRangeException() noexcept {}
            IndexOutOfRangeException(const char *message);
        };

        class FormatException : public SystemException
        {
        public:
            FormatException() noexcept {}
            FormatException(const char *message);
        };
    }
}

#endif