#include "pch.h"
#include "CppUnitTest.h"
#include "../DotNetNative/MemoryUtil.h"
#include "../DotNetNative/System/StringBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DotNetNative;
using namespace DotNetNative::System;

namespace UnitTests
{
    TEST_CLASS(StringTests)
    {
        TEST_METHOD(ConstructorAscii)
        {
            Assert::IsTrue(String("Hello World!") == "Hello World!");
        }
    };
}