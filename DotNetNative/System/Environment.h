#ifndef _DOTNETNATIVE_SYSTEM_ENVIRONMENT_H_
#define _DOTNETNATIVE_SYSTEM_ENVIRONMENT_H_

#include "String.h"
#include "Collections/IDictionary.h"

namespace DotNetNative
{
    namespace System
    {
        //
        // Summary:
        //     Specifies the location where an environment variable is stored or retrieved in
        //     a set or get operation.
        enum class EnvironmentVariableTarget
        {
            //
            // Summary:
            //     The environment variable is stored or retrieved from the environment block associated
            //     with the current process.
            Process = 0,
            //
            // Summary:
            //     The environment variable is stored or retrieved from the HKEY_CURRENT_USER\Environment
            //     key in the Windows operating system registry.
            User = 1,
            //
            // Summary:
            //     The environment variable is stored or retrieved from the HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session
            //     Manager\Environment key in the Windows operating system registry.
            Machine = 2
        };

        class Environment
        {
        public:
            static const String NewLine;
            static constexpr bool Is64BitProcess = sizeof(void*) == 8;
            static constexpr bool UserInteractive = true;

        private:
            Environment() = delete;
            Environment(const Environment &copy) = delete;
            Environment(Environment &&mov) = delete;
            ~Environment() = delete;

            static String GetEnvironmentVariableCore(const String &variable);
            static void SetEnvironmentVariableCore(const String &key, const String &value);
            static unique_ptr<Collections::IDictionary<String, String>> GetEnvironmentVariables();

        public:
            static String GetEnvironmentVariable(const String &variable);
            static String GetEnvironmentVariable(const String &variable, const EnvironmentVariableTarget target);
            static unique_ptr<Collections::IDictionary<String, String>> GetEnvironmentVariables(const EnvironmentVariableTarget target);
            static String ExpandEnvironmentVariables(const String &name);

            static void SetEnvironmentVariable(const String &key, const String &value);
            static void SetEnvironmentVariable(const String &key, const String &value, const EnvironmentVariableTarget target);
        };
    }
}

#endif