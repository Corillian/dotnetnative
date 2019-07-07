#include "pch.h"
#include "CppUnitTest.h"
#include "../DotNetNative/MemoryUtil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(MemoryTests)
	{
    private:
        class TestObj
        {
        private:
            bool *m_bool;

        public:
            TestObj()
                : m_bool(nullptr)
            {
            }

            TestObj(bool *boolPtr)
                : m_bool(boolPtr)
            {
                *boolPtr = true;
            }

            ~TestObj()
            {
                if(m_bool)
                {
                    *m_bool = false;
                }
            }

            inline void setBool(bool *boolPtr) noexcept { m_bool = boolPtr; }
            inline bool* getBool() const noexcept { return m_bool; }
        };

	public:
		
        void CreateTestDescriptors(bool *isAllocated, DotNetNative::Memory::AllocatorDescriptors &descriptors)
        {
            descriptors.m_alloc = [isAllocated](size_t size)
            {
                *isAllocated = true;

                return ::malloc(size);
            };

            descriptors.m_debugAlloc = [isAllocated](size_t size, const char *fileName, int lineNumber)
            {
                *isAllocated = true;

                return ::malloc(size);
            };

            descriptors.m_free = [isAllocated](void *memory)
            {
                ::free(memory);

                *isAllocated = false;
            };

            descriptors.m_debugFree = [isAllocated](void *memory, const char *fileName, int lineNumber)
            {
                ::free(memory);

                *isAllocated = false;
            };
        }

		TEST_METHOD(TestNewDelete)
		{
            DotNetNative::Memory::AllocatorDescriptors descriptors;
            bool isAllocated = false;

            CreateTestDescriptors(&isAllocated, descriptors);

            DotNetNative::Memory::SetAllocators(std::move(descriptors));

            int *test = new int(21);

            Assert::IsTrue(isAllocated);

            delete test;

            Assert::IsFalse(isAllocated);

            test = new int[21];

            Assert::IsTrue(isAllocated);

            delete[] test;

            Assert::IsFalse(isAllocated);

            // clear allocators
            DotNetNative::Memory::SetAllocators(DotNetNative::Memory::AllocatorDescriptors());

            bool objIsAllocated1 = false;
            bool objIsAllocated2 = false;

            TestObj *obj = new TestObj(&objIsAllocated1);

            Assert::IsTrue(objIsAllocated1);

            delete obj;

            Assert::IsFalse(objIsAllocated1);

            obj = new TestObj[2] { &objIsAllocated1, &objIsAllocated2 };

            Assert::IsTrue(objIsAllocated1);
            Assert::IsTrue(objIsAllocated2);

            delete[] obj;

            Assert::IsFalse(objIsAllocated1);
            Assert::IsFalse(objIsAllocated2);
		}

        TEST_METHOD(TestUniquePtr)
        {
            DotNetNative::Memory::AllocatorDescriptors descriptors;
            bool isAllocated = false;

            CreateTestDescriptors(&isAllocated, descriptors);

            DotNetNative::Memory::SetAllocators(std::move(descriptors));

            DotNetNative::unique_ptr<int> test = DNN_make_unique(int, 21);

            Assert::IsTrue(isAllocated);

            test.reset();

            Assert::IsFalse(isAllocated);

            // clear allocators
            DotNetNative::Memory::SetAllocators(DotNetNative::Memory::AllocatorDescriptors());

            bool objIsAllocated1 = false;

            DotNetNative::unique_ptr<TestObj> obj = DNN_make_unique(TestObj, &objIsAllocated1);

            Assert::IsTrue(objIsAllocated1);

            obj.reset();

            Assert::IsFalse(objIsAllocated1);
        }

        TEST_METHOD(TestUniquePtrArray)
        {
            DotNetNative::Memory::AllocatorDescriptors descriptors;
            bool isAllocated = false;

            CreateTestDescriptors(&isAllocated, descriptors);

            DotNetNative::Memory::SetAllocators(std::move(descriptors));

            DotNetNative::unique_ptr<int[]> test = DNN_make_unique_array(int[], 21);

            Assert::IsTrue(isAllocated);

            test.reset();

            Assert::IsFalse(isAllocated);

            // clear allocators
            DotNetNative::Memory::SetAllocators(DotNetNative::Memory::AllocatorDescriptors());

            bool objIsAllocated1 = true;
            bool objIsAllocated2 = true;

            DotNetNative::unique_ptr<TestObj[]> obj = DNN_make_unique_array(TestObj[], 2);

            Assert::IsNull(obj.get()[0].getBool());
            Assert::IsNull(obj.get()[1].getBool());

            obj.get()[0].setBool(&objIsAllocated1);
            obj.get()[1].setBool(&objIsAllocated2);

            obj.reset();

            Assert::IsFalse(objIsAllocated1);
            Assert::IsFalse(objIsAllocated2);
        }

        TEST_METHOD(TestSharedPtr)
        {
            DotNetNative::Memory::AllocatorDescriptors descriptors;
            bool isAllocated = false;

            CreateTestDescriptors(&isAllocated, descriptors);

            DotNetNative::Memory::SetAllocators(std::move(descriptors));

            std::shared_ptr<int> test = DNN_make_shared(int, 21);

            Assert::IsTrue(isAllocated);

            test.reset();

            Assert::IsFalse(isAllocated);

            // clear allocators
            DotNetNative::Memory::SetAllocators(DotNetNative::Memory::AllocatorDescriptors());

            bool objIsAllocated1 = false;

            DotNetNative::shared_ptr<TestObj> obj = DNN_make_shared(TestObj, &objIsAllocated1);

            Assert::IsTrue(objIsAllocated1);

            obj.reset();

            Assert::IsFalse(objIsAllocated1);
        }
	};
}
