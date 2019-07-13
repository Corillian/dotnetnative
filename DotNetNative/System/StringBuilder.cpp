#include "StringBuilder.h"
#include "Exception.h"

#include <cassert>
#include <algorithm>

namespace DotNetNative
{
    namespace System
    {
        static constexpr int DefaultBlockSize = 1024;

        StringBuilder::StringBuilder()
            : m_currentBlock(nullptr)
            , m_length(0)
            , m_capacity(0)
        {
        }

        StringBuilder::StringBuilder(const String &str)
            : m_currentBlock(nullptr)
            , m_length(str.Length())
            , m_capacity(str.Length())
        {
            if(str.Length() > 0)
            {
                m_blocks = DNN_make_unique(Block);
                m_blocks->m_nextBlock = nullptr;
                m_blocks->m_characters = DNN_make_unique_array(utf16char[], str.Length());
                m_blocks->m_blockLength = str.Length();
                m_blocks->m_offset = 0;
                m_blocks->m_count = str.Length();

                m_currentBlock = m_blocks.get();

                const size_t copySize = sizeof(utf16char) * str.Length();

                memcpy_s(m_blocks->m_characters.get(), copySize, str.m_string.get(), copySize);
            }
        }

        StringBuilder::StringBuilder(const utf16char *str, const int length)
            : m_currentBlock(nullptr)
            , m_length(length)
            , m_capacity(length)
        {
            if(!str)
            {
                throw ArgumentNullException("str");
            }

            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length");
            }

            if(length > 0)
            {
                m_blocks = DNN_make_unique(Block);
                m_blocks->m_nextBlock = nullptr;
                m_blocks->m_characters = DNN_make_unique_array(utf16char[], length);
                m_blocks->m_blockLength = length;
                m_blocks->m_offset = 0;
                m_blocks->m_count = length;

                m_currentBlock = m_blocks.get();

                const size_t copySize = sizeof(utf16char) * length;

                memcpy_s(m_blocks->m_characters.get(), copySize, str, copySize);
            }
        }

        StringBuilder::StringBuilder(const char *str, const int length)
            : m_currentBlock(nullptr)
            , m_length(length)
            , m_capacity(length)
        {
            if(!str)
            {
                throw ArgumentNullException("str");
            }

            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length");
            }

            if(length > 0)
            {
                m_blocks = DNN_make_unique(Block);
                m_blocks->m_nextBlock = nullptr;
                m_blocks->m_characters = DNN_make_unique_array(utf16char[], length);
                m_blocks->m_blockLength = length;
                m_blocks->m_offset = 0;
                m_blocks->m_count = length;

                m_currentBlock = m_blocks.get();

                for(int i = 0; i < length; ++i)
                {
                    m_blocks->m_characters.get()[i] = static_cast<utf16char>(str[i]);
                }
            }
        }

        StringBuilder::StringBuilder(const int capacity)
            : m_currentBlock(nullptr)
            , m_length(0)
            , m_capacity(capacity)
        {
            if(capacity < 0)
            {
                throw ArgumentOutOfRangeException("capacity");
            }

            if(capacity > 0)
            {
                m_blocks = DNN_make_unique(Block);
                m_blocks->m_nextBlock = nullptr;
                m_blocks->m_characters = DNN_make_unique_array(utf16char[], capacity);
                m_blocks->m_blockLength = capacity;
                m_blocks->m_offset = 0;
                m_blocks->m_count = 0;

                m_currentBlock = m_blocks.get();
            }
        }

        StringBuilder::StringBuilder(const StringBuilder &copy)
            : m_currentBlock(nullptr)
            , m_string(copy.m_string)
            , m_length(copy.m_length)
            , m_capacity(copy.m_length)
        {
            if(copy.m_length > 0)
            {
                m_blocks = DNN_make_unique(Block);
                m_blocks->m_nextBlock = nullptr;
                m_blocks->m_characters = DNN_make_unique_array(utf16char[], copy.m_length);
                m_blocks->m_blockLength = copy.m_length;
                m_blocks->m_offset = 0;
                m_blocks->m_count = copy.m_length;

                m_currentBlock = m_blocks.get();

                CopyBlocks(m_blocks->m_characters.get(), copy.m_length, copy);
            }
        }

        StringBuilder::StringBuilder(StringBuilder &&mov) noexcept
            : m_blocks(std::move(mov.m_blocks))
            , m_currentBlock(mov.m_currentBlock)
            , m_string(std::move(mov.m_string))
            , m_length(mov.m_length)
            , m_capacity(mov.m_capacity)
        {
            mov.m_currentBlock = nullptr;
            mov.m_length = 0;
            mov.m_capacity = 0;
        }

        StringBuilder& StringBuilder::operator=(const StringBuilder &copy)
        {
            if(this != &copy)
            {
                m_blocks.reset();
                m_currentBlock = nullptr;
                m_string = copy.m_string;
                m_length = copy.m_length;
                m_capacity = copy.m_length;

                if(copy.m_length > 0)
                {
                    m_blocks = DNN_make_unique(Block);
                    m_blocks->m_nextBlock = nullptr;
                    m_blocks->m_characters = DNN_make_unique_array(utf16char[], copy.m_length);
                    m_blocks->m_blockLength = copy.m_length;
                    m_blocks->m_offset = 0;
                    m_blocks->m_count = copy.m_length;

                    m_currentBlock = m_blocks.get();

                    CopyBlocks(m_blocks->m_characters.get(), copy.m_length, copy);
                }
            }

            return *this;
        }

        StringBuilder& StringBuilder::operator=(StringBuilder &&mov) noexcept
        {
            if(this != &mov)
            {
                m_blocks = std::move(mov.m_blocks);
                m_currentBlock = mov.m_currentBlock;
                m_string = std::move(mov.m_string);
                m_length = mov.m_length;
                m_capacity = mov.m_capacity;

                mov.m_currentBlock = nullptr;
                mov.m_length = 0;
                mov.m_capacity = 0;
            }

            return *this;
        }

        StringBuilder& StringBuilder::operator=(const char *str)
        {
            SetLength(0);
            Append(str, static_cast<int>(std::strlen(str)));

            return *this;
        }

        StringBuilder& StringBuilder::operator=(const utf16char *str)
        {
            SetLength(0);
            Append(str, static_cast<int>(utf16len(str)));

            return *this;
        }

        StringBuilder& StringBuilder::operator=(const String &str)
        {
            SetLength(0);
            Append(str);

            return *this;
        }

        utf16char StringBuilder::operator[](const int index) const &
        {
            if(index < 0 || index >= m_length)
            {
                throw IndexOutOfRangeException();
            }

            int curLength = 0;

            for(Block *blockIter = m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
            {
                curLength += blockIter->m_count;

                if(index < curLength)
                {
                    return blockIter->m_characters.get()[blockIter->m_offset + (index - (curLength - blockIter->m_count))];
                }
            }

            throw InvalidOperationException("Could not locate string block.");
        }

        utf16char& StringBuilder::operator[](const int index) &
        {
            if(index < 0 || index >= m_length)
            {
                throw IndexOutOfRangeException();
            }

            // Assume the string is going to be modified and clear the cached string
            m_string.reset();

            int curLength = 0;

            for(Block *blockIter = m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
            {
                curLength += blockIter->m_count;

                if(index < curLength)
                {
                    return blockIter->m_characters.get()[blockIter->m_offset + (index - (curLength - blockIter->m_count))];
                }
            }

            throw InvalidOperationException("Could not locate string block.");
        }

        String StringBuilder::ToString()
        {
            if(!m_string && m_length > 0)
            {
                m_string = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(m_length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

                CopyBlocks(m_string.get(), m_length, *this);
                
                m_string.get()[m_length] = 0;
            }

            return String(m_string, m_length);
        }

        String StringBuilder::ToString(const int startIndex, const int length) const
        {
            if(startIndex < 0 || length < 0 || startIndex + length > m_length)
            {
                throw ArgumentOutOfRangeException();
            }

            shared_ptr<utf16char[]> str;

            if(length > 0)
            {
                int totalStartLength = 0;
                Block *blockIter;

                for(blockIter = m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
                {
                    totalStartLength += blockIter->m_count;

                    if(totalStartLength > startIndex)
                    {
                        totalStartLength -= blockIter->m_count;
                        break;
                    }
                }

                if(!blockIter)
                {
                    throw InvalidOperationException("Unexpected error.");
                }

                const int startBlockOffset = startIndex - totalStartLength;
                const size_t destSize = sizeof(utf16char) * length;
                str = shared_ptr<utf16char[]>(DNN_New utf16char[static_cast<size_t>(length) + 1], Deleter<utf16char[]>(), DNN_Allocator(utf16char[]));

                if(startBlockOffset + length <= blockIter->m_count)
                {
                    memcpy_s(str.get(), destSize, blockIter->m_characters.get() + blockIter->m_offset, destSize);
                }
                else
                {
                    int numChars = blockIter->m_count - startBlockOffset;

                    memcpy_s(str.get(), destSize, blockIter->m_characters.get(), sizeof(utf16char) * numChars);

                    while(blockIter = blockIter->m_nextBlock.get())
                    {
                        if(numChars + blockIter->m_count >= length)
                        {
                            const int remaining = length - numChars;

                            if(remaining > blockIter->m_count)
                            {
                                throw InvalidOperationException("Unexpected error when flattening string builder blocks.");
                            }

                            memcpy_s(str.get() + numChars, destSize - (sizeof(utf16char) * numChars), blockIter->m_characters.get() + blockIter->m_offset, remaining * sizeof(utf16char));

                            break;
                        }

                        memcpy_s(str.get() + numChars, destSize - (sizeof(utf16char) * numChars), blockIter->m_characters.get() + blockIter->m_offset, blockIter->m_count * sizeof(utf16char));

                        numChars += blockIter->m_count;

                        assert(blockIter != m_currentBlock);
                    }
                }

                str.get()[length] = 0;

            }

            return String(std::move(str), length);
        }

        int StringBuilder::EnsureCapacity(int capacity)
        {
            if(capacity < 0)
            {
                throw ArgumentOutOfRangeException("capacity");
            }

            if(m_capacity == 0)
            {
                capacity = std::max(capacity, DefaultBlockSize);

                assert(m_length == 0);
                assert(!m_blocks);
                assert(!m_currentBlock);

                m_blocks = AllocateBlock(capacity);

                m_currentBlock = m_blocks.get();
                m_capacity = capacity;
            }
            else if(m_capacity < capacity)
            {
                capacity -= m_capacity;

                capacity = std::max(capacity, DefaultBlockSize);

                assert(m_blocks);
                assert(m_currentBlock);

                Block *blockIter = m_currentBlock;

                while(blockIter->m_nextBlock)
                {
                    blockIter = blockIter->m_nextBlock.get();
                }

                assert(!m_currentBlock->m_nextBlock);

                blockIter->m_nextBlock = AllocateBlock(capacity);
                m_capacity += capacity;
            }

            return m_capacity;
        }

        StringBuilder& StringBuilder::Remove(int startIndex, int length)
        {
            if(startIndex < 0 || startIndex + length > m_length)
            {
                throw new ArgumentOutOfRangeException();
            }

            if(length == 0)
            {
                return *this;
            }

            m_length -= length;
            m_string.reset();

            Block *blockIter;
            int charOffset = 0;

            for(blockIter = m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
            {
                if(charOffset + blockIter->m_count > startIndex)
                {
                    break;
                }

                charOffset += blockIter->m_count;
            }

            if(!blockIter)
            {
                throw InvalidOperationException("Unexpected error.");
            }

            while(blockIter && length > 0)
            {
                const int blockOffset = startIndex - charOffset;

                if(blockOffset == 0 && length >= blockIter->m_count)
                {
                    length -= blockIter->m_count;
                    startIndex += blockIter->m_count;
                    charOffset += blockIter->m_count;

                    blockIter->m_offset = 0;
                    blockIter->m_count = 0;
                }
                else if(blockOffset == 0 && length < blockIter->m_count)
                {
                    blockIter->m_count -= length;
                    blockIter->m_offset += length;

                    length = 0;

                    break;
                }
                else if(blockOffset + length >= blockIter->m_count)
                {
                    const int charsRemoved = blockIter->m_count - blockOffset;

                    length -= charsRemoved;
                    startIndex += charsRemoved;
                    charOffset += charsRemoved;

                    blockIter->m_count -= charsRemoved;
                }
                else
                {
                    // we have leftover chars at the beginning and the end

                    const int endOffset = blockOffset + length;
                    const int remaining = blockIter->m_count - endOffset;

                    memmove_s(blockIter->m_characters.get() + blockIter->m_offset + blockOffset, sizeof(utf16char) * blockIter->m_count, blockIter->m_characters.get() + blockIter->m_offset + endOffset, sizeof(utf16char) * remaining);

                    blockIter->m_count = blockOffset + remaining;

                    length = 0;

                    break;
                }

                blockIter = blockIter->m_nextBlock.get();
            }

            assert(length == 0);

            return *this;
        }

        StringBuilder& StringBuilder::Append(const utf16char value)
        {
            EnsureCapacity(m_capacity + 1);

            m_string.reset();
            ++m_length;

            if(m_currentBlock->m_offset + m_currentBlock->m_count + 1 > m_currentBlock->m_blockLength)
            {
                if(!m_currentBlock->m_nextBlock)
                {
                    // This can happen if capacity is being prepended
                    m_currentBlock->m_nextBlock = AllocateBlock(DefaultBlockSize);
                    m_capacity += DefaultBlockSize;
                }

                m_currentBlock = m_currentBlock->m_nextBlock.get();

                assert(m_currentBlock->m_blockLength > 0);

                m_currentBlock->m_offset = 0;
                m_currentBlock->m_count = 1;

                m_currentBlock->m_characters.get()[0] = value;
            }
            else
            {
                m_currentBlock->m_characters.get()[m_currentBlock->m_offset + m_currentBlock->m_count] = value;
                m_currentBlock->m_count += 1;
            }

            return *this;
        }

        StringBuilder& StringBuilder::Append(const char value)
        {
            return Append(static_cast<utf16char>(value));
        }

        StringBuilder& StringBuilder::Append(const utf16char value, int repeatCount)
        {
            if(repeatCount == 0)
            {
                return *this;
            }

            EnsureCapacity(m_capacity + repeatCount);

            m_string.reset();
            m_length += repeatCount;

            if(m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
            {
                if(m_currentBlock->m_nextBlock)
                {
                    m_currentBlock = m_currentBlock->m_nextBlock.get();
                }
                else
                {
                    m_currentBlock->m_nextBlock = AllocateBlock(repeatCount);
                    m_currentBlock = m_currentBlock->m_nextBlock.get();

                    m_capacity += repeatCount;
                }

                m_currentBlock->m_offset = 0;
                m_currentBlock->m_count = 0;
            }

            while(repeatCount > 0)
            {
                const int destBlockOffset = m_currentBlock->m_offset + m_currentBlock->m_count;
                const int available = m_currentBlock->m_blockLength - destBlockOffset;
                const int iterCount = std::min(available, repeatCount);
                utf16char *destPtr = m_currentBlock->m_characters.get() + m_currentBlock->m_offset + m_currentBlock->m_count;

                for(int i = 0; i < iterCount; ++i)
                {
                    destPtr[i] = value;
                }

                m_currentBlock->m_count += iterCount;
                repeatCount -= iterCount;

                if(repeatCount > 0 && m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
                {
                    if(m_currentBlock->m_nextBlock)
                    {
                        m_currentBlock = m_currentBlock->m_nextBlock.get();
                    }
                    else
                    {
                        m_currentBlock->m_nextBlock = AllocateBlock(repeatCount);
                        m_currentBlock = m_currentBlock->m_nextBlock.get();

                        m_capacity += repeatCount;
                    }

                    m_currentBlock->m_offset = 0;
                    m_currentBlock->m_count = 0;
                }
            }

            assert(repeatCount == 0);

            return *this;
        }

        StringBuilder& StringBuilder::Append(const utf16char *value, int length)
        {
            if(length == 0)
            {
                return *this;
            }

            if(!value)
            {
                throw ArgumentNullException("value");
            }

            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length must be > 0.");
            }

            EnsureCapacity(m_capacity + length);

            m_string.reset();
            m_length += length;

            if(m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
            {
                if(m_currentBlock->m_nextBlock)
                {
                    m_currentBlock = m_currentBlock->m_nextBlock.get();
                }
                else
                {
                    m_currentBlock->m_nextBlock = AllocateBlock(length);
                    m_currentBlock = m_currentBlock->m_nextBlock.get();

                    m_capacity += length;
                }

                m_currentBlock->m_offset = 0;
                m_currentBlock->m_count = 0;
            }

            while(length > 0)
            {
                const int destBlockOffset = m_currentBlock->m_offset + m_currentBlock->m_count;
                const int available = m_currentBlock->m_blockLength - destBlockOffset;
                const int iterCount = std::min(available, length);
                utf16char *destPtr = m_currentBlock->m_characters.get() + m_currentBlock->m_offset + m_currentBlock->m_count;
                const size_t size = sizeof(utf16char) * iterCount;

                memcpy_s(destPtr, size, value, size);

                m_currentBlock->m_count += iterCount;
                length -= iterCount;
                value += iterCount;

                if(length > 0 && m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
                {
                    if(m_currentBlock->m_nextBlock)
                    {
                        m_currentBlock = m_currentBlock->m_nextBlock.get();
                    }
                    else
                    {
                        m_currentBlock->m_nextBlock = AllocateBlock(length);
                        m_currentBlock = m_currentBlock->m_nextBlock.get();

                        m_capacity += length;
                    }

                    m_currentBlock->m_offset = 0;
                    m_currentBlock->m_count = 0;
                }
            }

            return *this;
        }

        StringBuilder& StringBuilder::Append(const char *value, int length)
        {
            if(length == 0)
            {
                return *this;
            }

            if(!value)
            {
                throw ArgumentNullException("value");
            }

            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length must be > 0.");
            }

            EnsureCapacity(m_capacity + length);

            m_string.reset();
            m_length += length;

            if(m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
            {
                if(m_currentBlock->m_nextBlock)
                {
                    m_currentBlock = m_currentBlock->m_nextBlock.get();
                }
                else
                {
                    m_currentBlock->m_nextBlock = AllocateBlock(length);
                    m_currentBlock = m_currentBlock->m_nextBlock.get();

                    m_capacity += length;
                }

                m_currentBlock->m_offset = 0;
                m_currentBlock->m_count = 0;
            }

            while(length > 0)
            {
                const int destBlockOffset = m_currentBlock->m_offset + m_currentBlock->m_count;
                const int available = m_currentBlock->m_blockLength - destBlockOffset;
                const int iterCount = std::min(available, length);
                utf16char *destPtr = m_currentBlock->m_characters.get() + m_currentBlock->m_offset + m_currentBlock->m_count;

                for(int i = 0; i < iterCount; ++i)
                {
                    destPtr[i] = static_cast<utf16char>(value[i]);
                }

                m_currentBlock->m_count += iterCount;
                length -= iterCount;
                value += iterCount;

                if(length > 0 && m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
                {
                    if(m_currentBlock->m_nextBlock)
                    {
                        m_currentBlock = m_currentBlock->m_nextBlock.get();
                    }
                    else
                    {
                        m_currentBlock->m_nextBlock = AllocateBlock(length);
                        m_currentBlock = m_currentBlock->m_nextBlock.get();

                        m_capacity += length;
                    }

                    m_currentBlock->m_offset = 0;
                    m_currentBlock->m_count = 0;
                }
            }

            return *this;
        }

        StringBuilder& StringBuilder::Append(const String &value)
        {
            if(value.Length() > 0)
            {
                Append(value.m_string.get(), value.m_length);
            }

            return *this;
        }

        StringBuilder& StringBuilder::Append(const StringBuilder &value)
        {
            if(value.GetLength() == 0)
            {
                return *this;
            }

            EnsureCapacity(m_capacity + value.GetLength());

            m_string.reset();
            m_length += value.GetLength();

            if(m_currentBlock->m_offset + m_currentBlock->m_count >= m_currentBlock->m_blockLength)
            {
                if(m_currentBlock->m_nextBlock)
                {
                    m_currentBlock = m_currentBlock->m_nextBlock.get();
                }
                else
                {
                    m_currentBlock->m_nextBlock = AllocateBlock(value.GetLength());
                    m_currentBlock = m_currentBlock->m_nextBlock.get();

                    m_capacity += value.GetLength();
                }

                m_currentBlock->m_offset = 0;
                m_currentBlock->m_count = 0;
            }

            Block *srcPtr = value.m_blocks.get();

            int numChars = 0;
            int srcBlockOffset = srcPtr->m_offset;
            int srcCount = srcPtr->m_count;

            do
            {
                if(srcCount > 0)
                {
                    const int destBlockOffset = m_currentBlock->m_offset + m_currentBlock->m_count;
                    const int available = m_currentBlock->m_blockLength - destBlockOffset;

                    assert(available > 0);

                    if(available >= srcCount)
                    {
                        memcpy_s(m_currentBlock->m_characters.get() + destBlockOffset, sizeof(utf16char) * available, srcPtr->m_characters.get() + srcBlockOffset, sizeof(utf16char) * srcCount);

                        numChars += srcCount;
                        m_currentBlock->m_count += srcCount;

                        srcPtr = srcPtr->m_nextBlock.get();

                        if(srcPtr)
                        {
                            srcBlockOffset = srcPtr->m_offset;
                            srcCount = srcPtr->m_count;
                        }
                    }
                    else
                    {
                        const size_t size = sizeof(utf16char) * available;

                        memcpy_s(m_currentBlock->m_characters.get() + destBlockOffset, size, srcPtr->m_characters.get() + srcBlockOffset, size);

                        m_currentBlock->m_count += available;
                        numChars += available;
                        srcBlockOffset += available;
                        srcCount -= available;

                        assert(m_currentBlock->m_offset + m_currentBlock->m_count == m_currentBlock->m_blockLength);

                        const int totalRemaining = value.GetLength() - numChars;

                        if(totalRemaining > 0)
                        {
                            if(m_currentBlock->m_nextBlock)
                            {
                                m_currentBlock = m_currentBlock->m_nextBlock.get();
                            }
                            else
                            {
                                m_currentBlock->m_nextBlock = AllocateBlock(totalRemaining);

                                m_capacity += totalRemaining;
                            }

                            m_currentBlock->m_offset = 0;
                            m_currentBlock->m_count = 0;
                        }
                    }
                }
                else
                {
                    srcPtr = srcPtr->m_nextBlock.get();

                    if(srcPtr)
                    {
                        srcBlockOffset = srcPtr->m_offset;
                        srcCount = srcPtr->m_count;
                    }
                }

            } while(srcPtr && numChars < value.GetLength());

            assert(numChars == value.GetLength());

            return *this;
        }

        void StringBuilder::SetLength(const int length)
        {
            if(length < 0)
            {
                throw ArgumentOutOfRangeException("length must be >= 0");
            }

            if(length > m_length)
            {
                throw ArgumentOutOfRangeException("length must be less than the length of the StringBuilder.");
            }

            m_string.reset();

            if(length == m_length)
            {
                return;
            }

            if(length == 0)
            {
                if(m_length > 0)
                {
                    m_length = 0;

                    if(m_blocks)
                    {
                        for(Block *blockIter = m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
                        {
                            blockIter->m_count = 0;

                            if(blockIter == m_currentBlock)
                            {
                                break;
                            }
                        }

                        m_currentBlock = m_blocks.get();
                    }
                }
            }
            else
            {
                int numChars = 0;
                Block *blockIter;

                for(blockIter = m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
                {
                    numChars += blockIter->m_count;

                    if(numChars > length)
                    {
                        blockIter->m_count -= numChars - length;
                        break;
                    }
                }

                while(blockIter = blockIter->m_nextBlock.get())
                {
                    blockIter->m_count = 0;

                    if(blockIter == m_currentBlock)
                    {
                        break;
                    }
                }
            }
        }

        void StringBuilder::CopyBlocks(utf16char *destination, const int destSize, const StringBuilder &src)
        {
            int destOffset = 0;

            for(Block *blockIter = src.m_blocks.get(); blockIter; blockIter = blockIter->m_nextBlock.get())
            {
                if(destOffset + blockIter->m_count > destSize)
                {
                    throw IndexOutOfRangeException();
                }

                memcpy_s(destination + destOffset, sizeof(utf16char) * (destSize - destOffset), blockIter->m_characters.get() + blockIter->m_offset, sizeof(utf16char) * blockIter->m_count);

                destOffset += blockIter->m_count;

                if(blockIter == src.m_currentBlock)
                {
                    break;
                }
            }

            assert(destOffset == destSize);
        }

        unique_ptr<StringBuilder::Block> StringBuilder::AllocateBlock(const int capacity)
        {
            unique_ptr<Block> block = DNN_make_unique(Block);
            block->m_nextBlock = nullptr;
            block->m_characters = DNN_make_unique_array(utf16char[], capacity);
            block->m_blockLength = capacity;
            block->m_offset = 0;
            block->m_count = 0;

            return block;
        }
    }
}