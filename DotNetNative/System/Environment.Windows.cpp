#include "Environment.h"
#include "../Internal/Win32/GlobalWindows.h"
#include "Exception.h"
#include "../Internal/Win32/Interop.Windows.h"
#include "Collections/Dictionary.h"
#include "../Internal/Win32/RegistryKey.h"

namespace DotNetNative
{
    namespace System
    {
        String Environment::GetEnvironmentVariableCore(const String &variable)
        {
            static_assert(sizeof(WCHAR) == sizeof(utf16char));

            const utf16char *varName = variable;
            WCHAR buffer[128];

            DWORD requiredSize = ::GetEnvironmentVariableW(reinterpret_cast<LPCWSTR>(varName), buffer, std::size(buffer));

            if(requiredSize == 0 && ::GetLastError() == ERROR_ENVVAR_NOT_FOUND)
            {
                return String();
            }

            if(requiredSize <= std::size(buffer))
            {
                return String(reinterpret_cast<const utf16char*>(buffer), requiredSize);
            }

            unique_ptr<WCHAR[]> dynamicBuffer = DNN_make_unique_array(WCHAR[], requiredSize);

            requiredSize = ::GetEnvironmentVariableW(reinterpret_cast<LPCWSTR>(varName), buffer, std::size(buffer));

            if(requiredSize == 0 && ::GetLastError() == ERROR_ENVVAR_NOT_FOUND)
            {
                return String();
            }

            return String(reinterpret_cast<const utf16char*>(dynamicBuffer.get()), requiredSize);
        }

        void Environment::SetEnvironmentVariableCore(const String &key, const String &value)
        {
            const utf16char *keyPtr = key;
            const utf16char *valuePtr = value;

            if(!::SetEnvironmentVariableW(reinterpret_cast<const WCHAR*>(keyPtr), reinterpret_cast<const WCHAR*>(valuePtr)))
            {
                const int errorCode = ::GetLastError();

                switch(errorCode)
                {
                case ERROR_ENVVAR_NOT_FOUND:
                    {
                        // Allow user to try to clear a environment variable
                        return;
                    }
                case ERROR_FILENAME_EXCED_RANGE:
                    {
                        // The error message from Win32 is "The filename or extension is too long",
                        // which is not accurate.
                        //throw new ArgumentException(SR.Argument_LongEnvVarValue);
                        throw new ArgumentException("Variable name exceeded allowable length.");
                    }
                case ERROR_NOT_ENOUGH_MEMORY:
                case ERROR_NO_SYSTEM_RESOURCES:
                    {
                        //throw new OutOfMemoryException(Interop.Kernel32.GetMessage(errorCode));
                        throw new OutOfMemoryException();
                    }
                default:
                    {
                        //throw new ArgumentException(Interop.Kernel32.GetMessage(errorCode));
                        throw new ArgumentException();
                    }
                }
            }
        }

        unique_ptr<Collections::IDictionary<String, String>> Environment::GetEnvironmentVariables()
        {
            static_assert(sizeof(WCHAR) == sizeof(utf16char));

            WCHAR *pStrings = ::GetEnvironmentStringsW();

            if(!pStrings)
            {
                throw new OutOfMemoryException();
            }

            unique_ptr<Collections::IDictionary<String, String>> results(DNN_New Collections::Dictionary<String, String>());

            try
            {
                // Format for GetEnvironmentStrings is:
                // [=HiddenVar=value\0]* [Variable=value\0]* \0
                // See the description of Environment Blocks in MSDN's
                // CreateProcess page (null-terminated array of null-terminated strings).

                // Search for terminating \0\0 (two unicode \0's).
                WCHAR *p = pStrings;

                while(!(*p == L'\0' && *(p + 1) == L'\0'))
                {
                    p++;
                }

                int blockLength = static_cast<int>(reinterpret_cast<ptrdiff_t>(p - pStrings)) + 1;
                WCHAR *block = pStrings;

                // Format for GetEnvironmentStrings is:
                // (=HiddenVar=value\0 | Variable=value\0)* \0
                // See the description of Environment Blocks in MSDN's
                // CreateProcess page (null-terminated array of null-terminated strings).
                // Note the =HiddenVar's aren't always at the beginning.

                // Copy strings out, parsing into pairs and inserting into the table.
                // The first few environment variable entries start with an '='.
                // The current working directory of every drive (except for those drives
                // you haven't cd'ed into in your DOS window) are stored in the 
                // environment block (as =C:=pwd) and the program's exit code is 
                // as well (=ExitCode=00000000).

                for(int i = 0; i < blockLength; ++i)
                {
                    int startKey = i;

                    // Skip to key. On some old OS, the environment block can be corrupted.
                    // Some will not have '=', so we need to check for '\0'. 
                    while(block[i] != L'=' && block[i] != L'\0')
                    {
                        i++;
                    }

                    if(block[i] == L'\0')
                    {
                        continue;
                    }

                    // Skip over environment variables starting with '='
                    if(i - startKey == 0)
                    {
                        while(block[i] != 0)
                        {
                            i++;
                        }

                        continue;
                    }

                    String key(reinterpret_cast<utf16char*>(block) + startKey, i - startKey);
                    i++;  // skip over '='

                    int startValue = i;

                    while(block[i] != 0)
                    {
                        i++; // Read to end of this entry 
                    }

                    String value(reinterpret_cast<utf16char*>(block) + startValue, i - startValue); // skip over 0 handled by for loop's i++

                    try
                    {
                        results->Add(std::move(key), std::move(value));
                    }
                    catch(ArgumentException)
                    {
                        // Throw and catch intentionally to provide non-fatal notification about corrupted environment block
                    }
                }
            }
            catch(...)
            {
                ::FreeEnvironmentStringsW(pStrings);

                std::rethrow_exception(std::current_exception());
            }

            ::FreeEnvironmentStringsW(pStrings);

            return results;
        }

        static Internal::Win32::RegistryKey OpenEnvironmentKeyIfExists(bool fromMachine, bool writable)
        {
            Internal::Win32::RegistryKey baseKey;
            String keyName;

            if(fromMachine)
            {
                baseKey = Registry.LocalMachine;
                keyName = "System\\CurrentControlSet\\Control\\Session Manager\\Environment";
            }
            else
            {
                baseKey = Registry.CurrentUser;
                keyName = "Environment";
            }

            return baseKey.OpenSubKey(keyName, writable: writable);
        }
    }
}