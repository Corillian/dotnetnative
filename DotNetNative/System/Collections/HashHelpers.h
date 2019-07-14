#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_HASHHELPERS_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_HASHHELPERS_H_

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            class HashHelpers
            {
            private:
                HashHelpers() = delete;
                HashHelpers(const HashHelpers &copy) = delete;
                HashHelpers(HashHelpers &&mov) = delete;
                ~HashHelpers() = delete;

            public:
                static bool IsPrime(const int candidate);
                static int GetPrime(const int min);
                static int ExpandPrime(const int oldSize);
            };
        }
    }
}

#endif