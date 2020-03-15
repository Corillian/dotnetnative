#ifndef _DOTNETNATIVE_INTERNAL_WIN32_INTEROPWINDOWS_H_
#define _DOTNETNATIVE_INTERNAL_WIN32_INTEROPWINDOWS_H_

#include "../../System/String.h"
#include "GlobalWindows.h"

namespace DotNetNative
{
    namespace Internal
    {
        namespace Win32
        {
            System::String GetWindowsErrorMessage(const DWORD errorCode, const HMODULE moduleHandle);
            inline System::String GetWindowsErrorMessage(const DWORD errorCode) { return GetWindowsErrorMessage(errorCode, nullptr); }
        }
    }
}

#endif