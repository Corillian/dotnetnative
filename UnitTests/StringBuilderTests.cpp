#include "pch.h"
#include "CppUnitTest.h"
#include "../DotNetNative/MemoryUtil.h"
#include "../DotNetNative/System/StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DotNetNative;
using namespace DotNetNative::System;

namespace UnitTests
{
    TEST_CLASS(StringBuilderTests)
    {
        TEST_METHOD(ConstructorAscii)
        {
            StringBuilder bldr("Hello World!");

            Assert::IsTrue(bldr.ToString().Equals(String("Hello World!")));
        }
    };
}