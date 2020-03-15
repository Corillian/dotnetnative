#include "RegistryKey.h"
#include "../../System/Exception.h"
#include "../../System/Environment.h"
#include "../../System/Collections/List.h"

#include <Shlwapi.h>

#include <cassert>
#include <exception>

namespace DotNetNative
{
    namespace Internal
    {
        namespace Win32
        {
            static constexpr bool IsPredefinedKey(const HKEY key)
            {
                return key != HKEY_CLASSES_ROOT
                    && key != HKEY_LOCAL_MACHINE
                    && key != HKEY_CURRENT_CONFIG
                    && key != HKEY_USERS
                    && key != HKEY_CURRENT_USER
                    && key != static_cast<HKEY>(0);
            }

            RegistryKey::RegistryKey(const HKEY key)
                : m_key(key)
            {
            }

            RegistryKey::RegistryKey(const RegistryKey &key)
                : m_key(key.m_key)
            {
                if(!IsPredefinedKey(m_key))
                {
                    m_key = SHRegDuplicateHKey(m_key);
                }
            }

            RegistryKey::RegistryKey(RegistryKey &&key) noexcept
                : m_key(key.m_key)
            {
                key.m_key = static_cast<HKEY>(0);
            }

            RegistryKey::~RegistryKey()
            {
                if(!IsPredefinedKey(m_key))
                {
                    ::RegCloseKey(m_key);

                    m_key = static_cast<HKEY>(0);
                }
            }

            RegistryKey& RegistryKey::operator=(const RegistryKey &key)
            {
                if(this != &key)
                {
                    if(!IsPredefinedKey(m_key))
                    {
                        ::RegCloseKey(m_key);
                    }

                    m_key = key.m_key;

                    if(!IsPredefinedKey(m_key))
                    {
                        m_key = SHRegDuplicateHKey(m_key);
                    }
                }

                return *this;
            }

            RegistryKey& RegistryKey::operator=(RegistryKey &&key) noexcept
            {
                if(this != &key)
                {
                    if(!IsPredefinedKey(m_key))
                    {
                        ::RegCloseKey(m_key);
                    }

                    m_key = key.m_key;

                    key.m_key = static_cast<HKEY>(0);
                }

                return *this;
            }

            void RegistryKey::DeleteValue(System::String name, bool throwOnMissingValue)
            {
                static_assert(sizeof(WCHAR) == sizeof(utf16char));

                const utf16char *rawName = name;

                LSTATUS errorCode = ::RegDeleteValueW(m_key, reinterpret_cast<const WCHAR*>(rawName));

                //
                // From windows 2003 server, if the name is too long we will get error code ERROR_FILENAME_EXCED_RANGE  
                // This still means the name doesn't exist. We need to be consistent with previous OS.
                //
                if(errorCode == ERROR_FILE_NOT_FOUND
                    || errorCode == ERROR_FILENAME_EXCED_RANGE)
                {
                    if(throwOnMissingValue)
                    {
                        throw System::ArgumentException("Registry sub-key value does not exist.");
                    }
                    else
                    {
                        // Otherwise, reset and just return giving no indication to the user.
                        // (For compatibility)
                        errorCode = 0;
                    }
                }

                // We really should throw an exception here if errorCode was bad,
                // but we can't for compatibility reasons.
                assert(errorCode == 0);
            }

            unique_ptr<RegistryKey> RegistryKey::OpenSubKey(const System::String &name)
            {
                return OpenSubKey(name, false);
            }

            unique_ptr<RegistryKey> RegistryKey::OpenSubKey(const System::String &name, const bool writable)
            {
                // Make sure that the name does not contain double slashes
                //assert(!name.Contains(L"\\\\"));

                static_assert(sizeof(WCHAR) == sizeof(utf16char));

                const utf16char *rawName = name;
                HKEY subKey;

                const LSTATUS ret = ::RegOpenKeyExW(
                    m_key,
                    reinterpret_cast<const WCHAR*>(rawName),
                    0,
                    writable ? (KEY_READ | KEY_WRITE) : KEY_READ,
                    &subKey);

                if(ret == ERROR_SUCCESS)
                {
                    try
                    {
                        return DNN_make_unique(RegistryKey, subKey);
                    }
                    catch(...)
                    {
                        ::RegCloseKey(subKey);

                        std::rethrow_exception(std::current_exception());
                    }
                }

                // Return null if we didn't find the key.
                if(ret == ERROR_ACCESS_DENIED || ret == ERROR_BAD_IMPERSONATION_LEVEL)
                {
                    // We need to throw SecurityException here for compatibility reasons,
                    // although UnauthorizedAccessException will make more sense.
                    throw System::SecurityException("You are not authorized to access the registry key.");
                }

                return unique_ptr<RegistryKey>();
            }

            System::Array<System::String[]> RegistryKey::GetSubKeyNames() const;
            System::Array<System::String[]> RegistryKey::GetValueNames() const;

            shared_ptr<IRegistryKeyValue> RegistryKey::ReadBinary(const System::String &name, DWORD type, DWORD dataSize) const
            {
                System::Array<uint8_t> result(dataSize);
                shared_ptr<IRegistryKeyValue> data;

                LSTATUS ret = ::RegQueryValueExW(
                    m_key,
                    reinterpret_cast<LPCWSTR>(static_cast<const utf16char*>(name)),
                    nullptr,
                    &type,
                    result.operator uint8_t *(),
                    &dataSize);

                if(ret == ERROR_SUCCESS)
                {
                    data = DNN_make_shared(RegistryKeyValueBinary, std::move(result));
                }

                return data;
            }

            shared_ptr<IRegistryKeyValue> RegistryKey::ReadQword(const System::String &name, DWORD type, DWORD dataSize) const
            {
                if(dataSize > 8)
                {
                    return ReadBinary(name, type, dataSize);
                }

                assert(dataSize == 8);

                long long blob = 0;
                shared_ptr<IRegistryKeyValue> data;

                LSTATUS ret = ::RegQueryValueExW(
                    m_key,
                    reinterpret_cast<LPCWSTR>(static_cast<const utf16char*>(name)),
                    nullptr,
                    &type,
                    reinterpret_cast<LPBYTE>(&blob),
                    &dataSize);

                if(ret == ERROR_SUCCESS)
                {
                    data = DNN_make_shared(RegistryKeyValueQWORD, blob);
                }

                return data;
            }

            shared_ptr<IRegistryKeyValue> RegistryKey::ReadString(const System::String &name, DWORD type, DWORD dataSize) const
            {
                if((dataSize & 1) == 1)
                {
                    if(dataSize == UINT_MAX)
                    {
                        throw System::IO::IOException("Registry value size overflow.");
                    }

                    ++dataSize;
                }

                System::Array<uint8_t> result(dataSize);
                shared_ptr<IRegistryKeyValue> data;

                LSTATUS ret = ::RegQueryValueExW(
                    m_key,
                    reinterpret_cast<LPCWSTR>(static_cast<const utf16char*>(name)),
                    nullptr,
                    &type,
                    result.operator uint8_t *(),
                    &dataSize);

                if(ret == ERROR_SUCCESS)
                {
                    const utf16char *wideStr = reinterpret_cast<const utf16char*>(result.operator const uint8_t*());
                    const DWORD strLen = dataSize / 2;

                    if(strLen > 0 && wideStr[strLen - 1] == 0)
                    {
                        data = DNN_make_shared(RegistryKeyValueString, System::String(wideStr, strLen - 1));
                    }
                    else
                    {
                        data = DNN_make_shared(RegistryKeyValueString, System::String(wideStr, strLen));
                    }
                }

                return data;
            }

            shared_ptr<IRegistryKeyValue> RegistryKey::GetValue(const System::String &name) const
            {
                DWORD type = REG_NONE;
                DWORD dataSize = 0;

                LSTATUS ret = ::RegQueryValueExW(
                    m_key,
                    reinterpret_cast<LPCWSTR>(static_cast<const utf16char*>(name)),
                    nullptr,
                    &type,
                    nullptr,
                    &dataSize);

                if(ret != ERROR_SUCCESS)
                {
                    // For stuff like ERROR_FILE_NOT_FOUND, we want to return null.
                    // Some OS's returned ERROR_MORE_DATA even in success cases, so we 
                    // want to continue on through the function. 
                    if(ret != ERROR_MORE_DATA)
                    {
                        return nullptr;
                    }
                }

                if(dataSize < 0)
                {
                    assert(false);
                    dataSize = 0;
                }

                shared_ptr<IRegistryKeyValue> data;

                switch(type)
                {
                case REG_NONE:
                case REG_DWORD_BIG_ENDIAN:
                case REG_BINARY:
                    {
                        data = ReadBinary(name, type, dataSize);
                        break;
                    }
                case REG_QWORD:
                    {
                        data = ReadQword(name, type, dataSize);
                        break;
                    }
                case REG_DWORD:
                    {
                        if(dataSize > 4)
                        {
                            data = ReadQword(name, type, dataSize);

                            break;
                        }

                        assert(dataSize == 4);

                        unsigned int blob = 0;

                        ret = ::RegQueryValueExW(
                            m_key,
                            reinterpret_cast<LPCWSTR>(static_cast<const utf16char*>(name)),
                            nullptr,
                            &type,
                            reinterpret_cast<LPBYTE>(&blob),
                            &dataSize);

                        if(ret == ERROR_SUCCESS)
                        {
                            data = DNN_make_shared(RegistryKeyValueDWORD, blob);
                        }

                        break;
                    }

                case REG_SZ:
                    {
                        data = ReadString(name, type, dataSize);
                        break;
                    }
                case REG_EXPAND_SZ:
                    {
                        shared_ptr<IRegistryKeyValue> tmp = ReadString(name, type, dataSize);

                        if(tmp)
                        {
                            RegistryKeyValueString *str = reinterpret_cast<RegistryKeyValueString*>(tmp.get());

                            System::String expanded = System::Environment::ExpandEnvironmentVariables(str->Value());

                            data = DNN_make_shared(RegistryKeyValueString, std::move(expanded));
                        }
                        
                        break;
                    }
                case REG_MULTI_SZ:
                    {
                        if((dataSize & 1) == 1)
                        {
                            if(dataSize == UINT_MAX)
                            {
                                throw System::IO::IOException("Registry value size overflow.");
                            }

                            ++dataSize;
                        }

                        System::Array<uint8_t> result(dataSize + 2);
                        shared_ptr<IRegistryKeyValue> data;

                        LSTATUS ret = ::RegQueryValueExW(
                            m_key,
                            reinterpret_cast<LPCWSTR>(static_cast<const utf16char*>(name)),
                            nullptr,
                            &type,
                            result.operator uint8_t *(),
                            &dataSize);

                        if(ret != ERROR_SUCCESS)
                        {
                            break;
                        }

                        const utf16char *wideStr = reinterpret_cast<const utf16char*>(result.operator const uint8_t*());
                        const DWORD strLen = dataSize / 2;

                        if(strLen > 0 && wideStr[strLen - 1] != 0)
                        {
                            assert(result.Length() >= dataSize + 2);
                            
                            reinterpret_cast<utf16char*>(result.operator uint8_t*())[strLen] = 0;
                        }
                        
                        DWORD cur = 0;
                        DWORD len = strLen;
                        System::Collections::List<System::String> strings;

                        while(cur < len)
                        {
                            DWORD nextNull = cur;

                            while(nextNull < len && wideStr[nextNull] != 0)
                            {
                                ++nextNull;
                            }

                            System::String toAdd;
                            bool hasString = false;

                            if(nextNull < len)
                            {
                                assert(wideStr[nextNull] == 0);

                                if(nextNull - cur > 0)
                                {
                                    toAdd = System::String(wideStr + cur, nextNull - cur);
                                    hasString = true;
                                }
                                else
                                {
                                    // we found an empty string.  But if we're at the end of the data,
                                    // it's just the extra null terminator.
                                    if(nextNull != len - 1)
                                    {
                                        hasString = true;
                                    }
                                }
                            }
                            else
                            {
                                toAdd = System::String(wideStr + cur, len - cur);
                                hasString = true;
                            }

                            cur = nextNull + 1;

                            if(hasString)
                            {
                                strings.Add(std::move(toAdd));
                            }
                        }

                        data = DNN_make_shared(RegistryKeyValueMultiString, strings.ToArray());

                        break;
                    }
                case REG_LINK:
                default:
                    {
                        break;
                    }
                }

                return data;
            }
        }
    }
}