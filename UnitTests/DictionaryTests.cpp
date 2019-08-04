#include "pch.h"
#include "CppUnitTest.h"
#include "../DotNetNative/System/Collections/Dictionary.h"
#include "../DotNetNative/System/Array.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DotNetNative;
using namespace DotNetNative::System;
using namespace DotNetNative::System::Collections;

namespace UnitTests
{
    TEST_CLASS(DictionaryTests)
    {
    public:
        TEST_METHOD(TestSimpleAdd)
        {
            unique_ptr<Array<int>> testArray;

            Dictionary<int, int> dict;

            dict.Add(1, 1);
            dict.Add(2, 2);
            dict.Add(3, 3);

            Assert::AreEqual(dict.Count(), 3LL);

            auto iter = dict.GetEnumerator();

            Assert::IsTrue(iter->MoveNext());
            Assert::IsTrue(iter->Current().Key() == 1 && iter->Current().Value() == 1);

            Assert::IsTrue(iter->MoveNext());
            Assert::IsTrue(iter->Current().Key() == 2 && iter->Current().Value() == 2);

            Assert::IsTrue(iter->MoveNext());
            Assert::IsTrue(iter->Current().Key() == 3 && iter->Current().Value() == 3);

            Assert::IsFalse(iter->MoveNext());

            Assert::AreEqual(dict[1], 1);
            Assert::AreEqual(dict[2], 2);
            Assert::AreEqual(dict[3], 3);

            Assert::IsTrue(dict.ContainsKey(1));
            Assert::IsFalse(dict.ContainsKey(0));
        }
    };
}