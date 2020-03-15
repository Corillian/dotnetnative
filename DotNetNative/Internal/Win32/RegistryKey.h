#ifndef _DOTNETNATIVE_INTERNAL_WIN32_REGISTRYKEY_H_
#define _DOTNETNATIVE_INTERNAL_WIN32_REGISTRYKEY_H_

#include "../../System/ObjectImpl.h"
#include "../../System/Array.h"
#include "GlobalWindows.h"

namespace DotNetNative
{
    namespace Internal
    {
        namespace Win32
        {
            enum class RegistryKeyValueType
            {
                DWORD,
                QWORD,
                Binary,
                String,
                MultiString
            };

            class IRegistryKeyValue
            {
            public:
                virtual ~IRegistryKeyValue() {}

                virtual RegistryKeyValueType ValueType() const noexcept = 0;
            };

            class RegistryKey : public virtual System::Object
            {
            private:
                // MSDN defines the following limits for registry key names & values:
                // Key Name: 255 characters
                // Value name:  16,383 Unicode characters
                // Value: either 1 MB or current available memory, depending on registry format.
                static constexpr int MaxKeyLength = 255;
                static constexpr int MaxValueLength = 16383;

                HKEY m_key;

            private:
                shared_ptr<IRegistryKeyValue> ReadBinary(const System::String &name, DWORD type, DWORD dataSize) const;
                shared_ptr<IRegistryKeyValue> ReadQword(const System::String &name, DWORD type, DWORD dataSize) const;
                shared_ptr<IRegistryKeyValue> ReadString(const System::String &name, DWORD type, DWORD dataSize) const;

            public:
                RegistryKey(const HKEY key);
                RegistryKey(const RegistryKey &key);
                RegistryKey(RegistryKey &&key) noexcept;
                ~RegistryKey();

                RegistryKey& operator=(const RegistryKey &key);
                RegistryKey& operator=(RegistryKey &&key) noexcept;

                void DeleteValue(System::String name, bool throwOnMissingValue);
                unique_ptr<RegistryKey> OpenSubKey(const System::String &name);
                unique_ptr<RegistryKey> OpenSubKey(const System::String &name, const bool writable);
                System::Array<System::String[]> GetSubKeyNames() const;
                System::Array<System::String[]> GetValueNames() const;
                shared_ptr<IRegistryKeyValue> GetValue(const System::String &name) const;
            };

            class RegistryKeyValueDWORD : public IRegistryKeyValue
            {
            private:
                const unsigned int m_value;

            public:
                inline RegistryKeyValueDWORD(const unsigned int value) : m_value(value) {}
                virtual ~RegistryKeyValueDWORD() {}

                virtual RegistryKeyValueType ValueType() const noexcept override { return RegistryKeyValueType::DWORD; }

                inline unsigned int Value() const noexcept { return m_value; }
            };

            class RegistryKeyValueQWORD : public IRegistryKeyValue
            {
            private:
                const unsigned long long m_value;

            public:
                inline RegistryKeyValueQWORD(const unsigned long long value) : m_value(value) {}
                virtual ~RegistryKeyValueQWORD() {}

                virtual RegistryKeyValueType ValueType() const noexcept override { return RegistryKeyValueType::QWORD; }

                inline unsigned long long Value() const noexcept { return m_value; }
            };

            class RegistryKeyValueBinary : public IRegistryKeyValue
            {
            private:
                System::Array<uint8_t> m_binary;

            public:
                inline RegistryKeyValueBinary(System::Array<uint8_t> &&binary) : m_binary(std::move(binary)) {}
                virtual ~RegistryKeyValueBinary() {}

                virtual RegistryKeyValueType ValueType() const noexcept override { return RegistryKeyValueType::Binary; }

                inline const System::Array<uint8_t>& Value() const noexcept { return m_binary; }
            };

            class RegistryKeyValueString : public IRegistryKeyValue
            {
            private:
                System::String m_str;

            public:
                inline RegistryKeyValueString(System::String &&str) : m_str(std::move(str)) {}
                virtual ~RegistryKeyValueString() {}

                virtual RegistryKeyValueType ValueType() const noexcept override { return RegistryKeyValueType::String; }

                inline const System::String& Value() const noexcept { return m_str; }
            };

            class RegistryKeyValueMultiString : public IRegistryKeyValue
            {
            private:
                System::Array<System::String> m_strings;

            public:
                inline RegistryKeyValueMultiString(System::Array<System::String> &&strings) : m_strings(std::move(strings)) {}
                virtual ~RegistryKeyValueMultiString() {}

                virtual RegistryKeyValueType ValueType() const noexcept override { return RegistryKeyValueType::String; }

                inline const System::Array<System::String>& Value() const noexcept { return m_strings; }
            };
        }
    }
}

#endif