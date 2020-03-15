#include "Environment.h"

#include <cassert>

namespace DotNetNative
{
    namespace System
    {
        const String Environment::NewLine("\r\n");

        static bool ValidateAndConvertRegistryTarget(EnvironmentVariableTarget target)
        {
            assert(target != EnvironmentVariableTarget::Process);

            if(target == EnvironmentVariableTarget::Machine)
            {
                return true;
            }
                

            if(target == EnvironmentVariableTarget::User)
            {
                return false;
            }

            throw new ArgumentOutOfRangeException("Unrecognized EnvironmentVariableTarget value.");
        }

        String Environment::GetEnvironmentVariable(const String &variable)
        {
            return Environment::GetEnvironmentVariableCore(variable);
        }

        String Environment::GetEnvironmentVariable(const String &variable, const EnvironmentVariableTarget target)
        {
            if(target == EnvironmentVariableTarget::Process)
            {
                return GetEnvironmentVariableCore(variable);
            }
                
            return GetEnvironmentVariableFromRegistry(variable, ValidateAndConvertRegistryTarget(target));
        }

        unique_ptr<Collections::IDictionary<String, String>> Environment::GetEnvironmentVariables(const EnvironmentVariableTarget target);
        String Environment::ExpandEnvironmentVariables(const String &name);

        void Environment::SetEnvironmentVariable(const String &key, const String &value);
        void Environment::SetEnvironmentVariable(const String &key, const String &value, const EnvironmentVariableTarget target);
    }
}