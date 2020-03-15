#ifndef _DOTNETNATIVE_SYSTEM_ACTION_H_
#define _DOTNETNATIVE_SYSTEM_ACTION_H_

#include <functional>

namespace DotNetNative
{
    namespace System
    {
        template <typename... TArgs>
        using Action = std::function<void(TArgs... args)>;

        template <typename T>
        using Comparison = std::function<int(const T&, const T&)>;

        template <typename TInput, typename TOutput>
        using Converter = std::function<void(const TInput&, TOutput&)>;

        template <typename T>
        using Predicate = std::function<bool(const T&)>;
    }
}

#endif