#include "Exception.h"

namespace DotNetNative
{
    namespace System
    {
        ///////////////////////////////////////////////////// Exception /////////////////////////////////////////////////////
        Exception::Exception(const char *message)
            : std::exception(message)
        {
        }

        ///////////////////////////////////////////////////// SystemException /////////////////////////////////////////////////////

        SystemException::SystemException(const char *message)
            : Exception(message)
        {
        }

        ///////////////////////////////////////////////////// ArgumentException /////////////////////////////////////////////////////

        ArgumentException::ArgumentException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// ArgumentNullException /////////////////////////////////////////////////////

        ArgumentNullException::ArgumentNullException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// ArgumentOutOfRangeException /////////////////////////////////////////////////////

        ArgumentOutOfRangeException::ArgumentOutOfRangeException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// NullReferenceException /////////////////////////////////////////////////////

        NullReferenceException::NullReferenceException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// NotImplementedException /////////////////////////////////////////////////////

        NotImplementedException::NotImplementedException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// NotSupportedException /////////////////////////////////////////////////////

        NotSupportedException::NotSupportedException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// InvalidOperationException /////////////////////////////////////////////////////

        InvalidOperationException::InvalidOperationException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// IndexOutOfRangeException /////////////////////////////////////////////////////

        IndexOutOfRangeException::IndexOutOfRangeException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// FormatException /////////////////////////////////////////////////////

        FormatException::FormatException(const char *message)
            : SystemException(message)
        {
        }

        ///////////////////////////////////////////////////// KeyNotFoundException /////////////////////////////////////////////////////

        KeyNotFoundException::KeyNotFoundException(const char *message)
            : SystemException(message)
        {
        }
    }
}