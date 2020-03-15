#include "Interop.Windows.h"

#include <iterator>

namespace DotNetNative
{
    namespace Internal
    {
        namespace Win32
        {
            static System::String GetAndTrimString(const WCHAR *buffer, int length)
            {
                static_assert(sizeof(WCHAR) == sizeof(utf16char));

                while(length > 0 && buffer[length - 1] <= 32)
                {
                    length--; // trim off spaces and non-printable ASCII chars at the end of the resource
                }

                return System::String(reinterpret_cast<const utf16char*>(buffer), length);
            }

            System::String GetWindowsErrorMessage(const DWORD errorCode, const HMODULE moduleHandle)
            {
                DWORD flags = FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY;

                if(moduleHandle)
                {
                    flags |= FORMAT_MESSAGE_FROM_HMODULE;
                }

                // First try to format the message into the stack based buffer.  Most error messages willl fit.
                WCHAR stackBuffer[256]; // arbitrary stack limit

                int length = ::FormatMessageW(flags, moduleHandle, errorCode, 0, stackBuffer, std::size(stackBuffer), nullptr);

                if(length > 0)
                {
                    return GetAndTrimString(stackBuffer, std::size(stackBuffer));
                }

                // We got back an error.  If the error indicated that there wasn't enough room to store
                // the error message, then call FormatMessage again, but this time rather than passing in
                // a buffer, have the method allocate one, which we then need to free.
                if(::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
                    WCHAR *nativeMsgPtr = nullptr;

                    int length = FormatMessageW(flags | FORMAT_MESSAGE_ALLOCATE_BUFFER, moduleHandle, errorCode, 0, reinterpret_cast<LPWSTR>(&nativeMsgPtr), 0, nullptr);

                    if(length > 0)
                    {
                        System::String result = GetAndTrimString(nativeMsgPtr, length);

                        ::LocalFree(nativeMsgPtr);

                        return result;
                    }

                    ::LocalFree(nativeMsgPtr);
                }

                // Couldn't get a message, so manufacture one.
                //return String::Format("Unknown error (0x{0:x})", errorCode);
                return System::String("Unknown error.");
            }
        }
    }
}