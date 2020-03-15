#ifndef _DOTNETNATIVE_SYSTEM_COLLECTIONS_LIST_H_
#define _DOTNETNATIVE_SYSTEM_COLLECTIONS_LIST_H_

#include "IList.h"
#include "../Array.h"
#include "../Action.h"
#include "IComparer.h"
#include "../Exception.h"

namespace DotNetNative
{
    namespace System
    {
        namespace Collections
        {
            template <typename T>
            class List
                : public virtual IList<T>
            {
            private:
                constexpr int DefaultCapacity = 4;

                Array<T> m_items;
                int      m_size;
                int      m_version;

            private:
                T& AddWithResize(const T &item);
                T& AddWithResize(T &&item);
                void EnsureCapacity(const int min);

            public
                List();
                List(const List<T> &copy);
                List(List<T> &&mov) noexcept;
                List(const IEnumerable<T> collection);
                List(const int capacity);
                virtual ~List() {}

                List<T>& operator=(const List<T> &copy);
                List<T>& operator=(List<T> &&src);

                virtual const T& operator[](const int64_t index) const & override;
                virtual T& operator[](const int64_t index) & override;

                // Returns the index of a particular item, if it is in the list.
                // Returns -1 if the item isn't in the list.
                virtual int IndexOf(const T &item) const override;
                int LastIndexOf(const T &item) const;

                // Inserts value into the list at position index.
                // index must be non-negative and less than or equal to the 
                // number of elements in the list.  If index equals the number
                // of items in the list, then value is appended to the end.
                virtual void Insert(int index, const T &item) override;
                virtual void Insert(int index, T &&item) override;

                // Inserts the elements of the given collection at a given index. If
                // required, the capacity of the list is increased to twice the previous
                // capacity or the new size, whichever is larger.  Ranges may be added
                // to the end of the list by setting index to the List's size.
                //
                void InsertRange(int index, const IEnumerable<T> &collection);

                // Removes the item at position index.
                virtual void RemoveAt(int index) override;

                T& Add(const T &item);
                T& Add(T &&item);

                // Adds the elements of the given collection to the end of this list. If
                // required, the capacity of the list is increased to twice the previous
                // capacity or the new size, whichever is larger.
                //
                void AddRange(const IEnumerable<T> &collection);

                T* Find(const Predicate<T> &match) const;
                T* FindLast(const Predicate<T> &match) const;
                
                int FindIndex(const int startIndex, const int count, const Predicate<T> &match) const;
                inline int FindIndex(const Predicate<T> &match) const { return FindIndex(0, m_size, match); }
                inline int FindIndex(const int startIndex, const Predicate<T> &match) const { return FindIndex(startIndex, m_size - startIndex, match); }

                int FindLastIndex(const int startIndex, const int count, const Predicate<T> &match) const;
                inline int FindLastIndex(const Predicate<T> &match) const { return FindLastIndex(0, m_size, match); }
                inline int FindLastIndex(const int startIndex, const Predicate<T> &match) const { return FindLastIndex(startIndex, m_size - startIndex, match); }

                void ForEach(const std::function<void(T&)> action);
                void ForEach(const std::function<void(const T&)> action) const;

                List<T> GetRange(const int index, const int count) const;

                // This method removes all items which matches the predicate.
                // The complexity is O(n).
                int RemoveAll(const Predicate<T> &match);

                // Removes a range of elements from this list.
                void RemoveRange(const int index, const int count);

                // Clears the contents of List.
                void Clear();

                // Reverses the elements in a range of this list. Following a call to this
                // method, an element in the range given by index and count
                // which was previously located at index i will now be located at
                // index index + (index + count - i - 1).
                //
                void Reverse(const int index, const int count);

                // Reverses the elements in this list.
                inline void Reverse() { Reverse(0, Count()); }

                // ToArray returns an array containing the contents of the List.
                // This requires copying the List, which is an O(n) operation.
                Array<T> ToArray() const;

                int Capacity() const;
                int Capacity(const int newCapacity);

                virtual int64_t Count() const override { return m_size; }
                virtual bool IsFixedSize() const noexcept override { return false; }
            };

            ///////////////////////////////////////////////////// List /////////////////////////////////////////////////////

            template <typename T>
            List<T>::List()
                : m_size(0)
                , m_version(0)
            {
            }

            template <typename T>
            List<T>::List(const List<T> &copy)
                : m_items(copy.m_items)
                , m_size(copy.m_size)
                , m_version(copy.m_version)
            {
            }

            template <typename T>
            List<T>::List(List<T> &&mov) noexcept
                : m_items(std::move(mov.m_items))
                , m_size(mov.m_size)
                , m_version(0)
            {
                mov.m_size = 0;
                ++mov.m_version;
            }

            template <typename T>
            List<T>::List(const IEnumerable<T> collection)
                : m_size(0)
                , m_version(0)
            {
                unique_ptr<IEnumerator<T>> iter = collection.GetEnumerator();

                while(iter->MoveNext())
                {
                    // NOTE: Add() is *NOT* virtual and *MUST* stay that way or else this needs to
                    // be switched to a non-virtual implementation
                    Add(iter->Current());
                }
            }

            template <typename T>
            List<T>::List(const int capacity)
                : m_items(capacity)
                , m_size(0)
                , m_version(0)
            {
            }

            template <typename T>
            List<T>& List<T>::operator=(const List<T> &copy)
            {
                if(this != &copy)
                {
                    m_items = copy.m_items;
                    m_size = copy.m_size;
                    ++m_version;
                }

                return *this;
            }

            template <typename T>
            List<T>& List<T>::operator=(List<T> &&src)
            {
                if(this != &copy)
                {
                    m_items = std::move(src);
                    m_size = src.m_size;
                    ++m_version;

                    src.m_size = 0;
                    ++src.m_version;
                }

                return *this;
            }

            template <typename T>
            const T& List<T>::operator[](const int64_t index) const &
            {
                return m_items[index]
            }

            template <typename T>
            T& List<T>::operator[](const int64_t index) &
            {
                return m_items[index]
            }

            // Returns the index of a particular item, if it is in the list.
            // Returns -1 if the item isn't in the list.
            template <typename T>
            int List<T>::IndexOf(const T &item) const
            {
                return static_cast<int>(Array<T>::IndexOf(m_items, item));
            }

            template <typename T>
            int List<T>::LastIndexOf(const T &item) const
            {
                return static_cast<int>(Array<T>::LastIndexOf(m_items, item));
            }

            template <typename T>
            void List<T>::Insert(int index, const T &item)
            {
                if(index > m_size)
                {
                    throw ArgumentOutOfRangeException();
                }

                if(m_size == m_items.Length())
                {
                    EnsureCapacity(m_size + 1);
                }

                if(index < m_size)
                {
                    Array.Copy(m_items, index, m_items, index + 1, m_size - index);
                }

                m_items[index] = item;
                ++m_size;
                ++m_version;
            }

            template <typename T>
            void List<T>::Insert(int index, T &&item)
            {
                if(index > m_size)
                {
                    throw ArgumentOutOfRangeException();
                }

                if(m_size == m_items.Length())
                {
                    EnsureCapacity(m_size + 1);
                }

                if(index < m_size)
                {
                    Array.Copy(m_items, index, m_items, index + 1, m_size - index);
                }

                m_items[index] = std::move(item);
                ++m_size;
                ++m_version;
            }

            template <typename T>
            void List<T>::InsertRange(int index, const IEnumerable<T> &collection)
            {
                if(static_cast<int64_t>(index) > m_size)
                {
                    throw ArgumentOutOfRangeException();
                }

                unique_ptr<IEnumerator<T>> iter = collection.GetEnumerator();

                while(iter->MoveNext())
                {
                    Insert(index++, iter->Current());
                }

                ++m_version;
            }

            template <typename T>
            void List<T>::RemoveAt(int index)
            {
                if(index >= m_size)
                {
                    throw ArgumentOutOfRangeException("index out of range.");
                }

                --m_size;

                if(index < m_size)
                {
                    Array.Copy(m_items, index + 1, m_items, index, m_size - index);
                }

                if(!std::is_trivially_destructible<T>::value || !std::is_trivially_constructible<T>::value)
                {
                    T *item = &m_items[m_size];

                    item->~T();
                    new (item) T();
                }

                ++m_version;
            }

            template <typename T>
            T& List<T>::Add(const T &item)
            {
                ++m_version;

                const int size = m_size;

                if(m_size < m_items.Length())
                {
                    m_items[size] = item;
                    ++m_size;
                }
                else
                {
                    AddWithResize(item);
                }

                return m_items[size];
            }

            template <typename T>
            T& List<T>::Add(T &&item)
            {
                ++m_version;

                const int size = m_size;

                if(m_size < m_items.Length())
                {
                    m_items[size] = std::move(item);
                    ++m_size;
                }
                else
                {
                    AddWithResize(std::move(item));
                }

                return m_items[size];
            }

            template <typename T>
            void List<T>::AddRange(const IEnumerable<T> &collection)
            {
                InsertRange(m_size, collection);
            }

            template <typename T>
            T* List<T>::Find(const Predicate<T> &match) const
            {
                if(!match)
                {
                    throw ArgumentNullException();
                }

                for(int i = 0; i < m_size; ++i)
                {
                    if(match(m_items[i]))
                    {
                        return &m_items[i];
                    }
                }

                return nullptr;
            }

            template <typename T>
            T* List<T>::FindLast(const Predicate<T> &match) const
            {
                if(!match)
                {
                    throw ArgumentNullException();
                }

                for(int i = m_size - 1; i >= 0; --i)
                {
                    if(match(m_items[i]))
                    {
                        return m_items[i];
                    }
                }

                return nullptr;
            }

            template <typename T>
            int List<T>::FindIndex(const int startIndex, const int count, const Predicate<T> &match) const
            {
                if(static_cast<uint32_t>(startIndex) > static_cast<uint32_t>(m_size))
                {
                    throw new ArgumentOutOfRangeException("Invalid index.");
                }

                if(count < 0 || startIndex > m_size - count)
                {
                    throw new ArgumentOutOfRangeException("Invalid count.");
                }

                if(!match)
                {
                    throw new ArgumentNullException("match");
                }

                int endIndex = startIndex + count;

                for(int i = startIndex; i < endIndex; ++i)
                {
                    if(match(m_items[i]))
                    {
                        return i;
                    }
                }

                return -1;
            }

            template <typename T>
            int List<T>::FindLastIndex(const int startIndex, const int count, const Predicate<T> &match) const
            {
                if(!match)
                {
                    throw ArgumentNullException("match");
                }

                if(m_size == 0)
                {
                    // Special case for 0 length List
                    if(startIndex != -1)
                    {
                        throw ArgumentOutOfRangeException("Invlaid index.");
                    }
                }
                else
                {
                    // Make sure we're not out of range
                    if(static_cast<uint32_t>(startIndex) >= static_cast<uint32_t>(m_size))
                    {
                        throw ArgumentOutOfRangeException("Invlaid index.");
                    }
                }

                // 2nd have of this also catches when startIndex == MAXINT, so MAXINT - 0 + 1 == -1, which is < 0.
                if(count < 0 || startIndex - count + 1 < 0)
                {
                    throw ArgumentOutOfRangeException("Invlaid count.");
                }

                int endIndex = startIndex - count;

                for(int i = startIndex; i > endIndex; --i)
                {
                    if(match(m_items[i]))
                    {
                        return i;
                    }
                }

                return -1
            }

            template <typename T>
            void List<T>::ForEach(const std::function<void(T&)> action)
            {
                if(!action)
                {
                    throw ArgumentNullException("action");
                }

                const int version = m_version;

                for(int i = 0; i < m_size && version == m_version; ++i)
                {
                    action(m_items[i]);
                }

                if(version != m_version)
                {
                    throw InvalidOperationException("The collection was modified.");
                }
            }

            template <typename T>
            void List<T>::ForEach(const std::function<void(const T&)> action) const
            {
                if(!action)
                {
                    throw ArgumentNullException("action");
                }

                const int version = m_version;

                for(int i = 0; i < m_size && version == m_version; ++i)
                {
                    action(m_items[i]);
                }

                if(version != m_version)
                {
                    throw InvalidOperationException("The collection was modified.");
                }
            }

            template <typename T>
            List<T> List<T>::GetRange(const int index, const int count) const
            {
                if(index < 0)
                {
                    throw ArgumentOutOfRangeException("Need non-negative index.");
                }

                if(count < 0)
                {
                    throw ArgumentOutOfRangeException("Need non-negative count.");
                }

                if(m_size - index < count)
                {
                    throw ArgumentException("Invalid offset and length.");
                }

                List<T> list(count);

                Array.Copy(_items, index, list._items, 0, count);

                list.m_size = count;

                return list;
            }

            // This method removes all items which matches the predicate.
            // The complexity is O(n).
            template <typename T>
            int List<T>::RemoveAll(const Predicate<T> &match)
            {
                if(!match)
                {
                    throw ArgumentNullException("match");
                }

                int freeIndex = 0;   // the first free slot in items array

                // Find the first item which needs to be removed.
                while(freeIndex < m_size && !match(m_items[freeIndex]))
                {
                    ++freeIndex;
                }

                if(freeIndex >= m_size)
                {
                    return 0;
                }

                int current = freeIndex + 1;

                while(current < m_size)
                {
                    // Find the first item which needs to be kept.
                    while(current < m_size && match(m_items[current]))
                    {
                        ++current;
                    }

                    if(current < m_size)
                    {
                        // copy item to the free slot.
                        m_items[freeIndex++] = m_items[current++];
                    }
                }

                Array<T>::Clear(m_items, freeIndex, m_size - freeIndex);

                const int result = m_size - freeIndex;

                m_size = freeIndex;
                ++m_version;

                return result;
            }

            // Removes a range of elements from this list.
            template <typename T>
            void List<T>::RemoveRange(const int index, const int count)
            {
                if(index < 0)
                {
                    throw ArgumentOutOfRangeException("Need a non-negative index.");
                }

                if(count < 0)
                {
                    throw ArgumentOutOfRangeException("Need a non-negative count.");
                }

                if(m_size - index < count)
                {
                    throw ArgumentException("Invalid offset and length.");
                }

                if(count > 0)
                {
                    m_size -= count;

                    if(index < m_size)
                    {
                        Array.Copy(m_items, index + count, m_items, index, m_size - index);
                    }

                    ++m_version;

                    Array.Clear(m_items, m_size, count);
                }
            }

            // Clears the contents of List.
            template <typename T>
            void List<T>::Clear()
            {
                Array<T>::Clear(m_items, 0, m_size);
                m_size = 0;
                ++m_version;
            }

            // Reverses the elements in a range of this list. Following a call to this
            // method, an element in the range given by index and count
            // which was previously located at index i will now be located at
            // index index + (index + count - i - 1).
            //
            template <typename T>
            void List<T>::Reverse(const int index, const int count)
            {
                if(index < 0)
                {
                    throw ArgumentOutOfRangeException("Need a non-negative index.");
                }

                if(count < 0)
                {
                    throw ArgumentOutOfRangeException("Need a non-negative count.");
                }

                if(m_size - index < count)
                {
                    throw ArgumentException("Invalid offset and length.");
                }

                if(count > 1)
                {
                    Array.Reverse(m_items, index, count);
                }

                ++m_version;
            }

            // ToArray returns an array containing the contents of the List.
            // This requires copying the List, which is an O(n) operation.
            template <typename T>
            Array<T> List<T>::ToArray() const
            {
                if(m_size == 0)
                {
                    return Array<T>();
                }

                return Array<T>(m_items);
            }

            template <typename T>
            int List<T>::Capacity() const
            {
                return static_cast<int>(m_items.Length());
            }

            template <typename T>
            int List<T>::Capacity(const int newCapacity)
            {
                if(newCapacity < m_size)
                {
                    throw ArgumentOutOfRangeException("Invalid capacity");
                }

                if(newCapacity != m_items.Length())
                {
                    if(newCapacity > 0)
                    {
                        Array<T> newItems(value);

                        if(m_size > 0)
                        {
                            Array<T>::Move(m_items, 0, newItems, 0, m_size);
                        }

                        m_items = std::move(newItems);
                    }
                    else
                    {
                        m_items = Array<T>();
                    }
                }
            }

            template <typename T>
            T& List<T>::AddWithResize(const T &item)
            {
                EnsureCapacity(m_size + 1);
                
                m_items[m_size] = item;
                ++m_size;
            }

            template <typename T>
            T& List<T>::AddWithResize(T &&item)
            {
                EnsureCapacity(m_size + 1);

                m_items[m_size] = std::move(item);
                ++m_size;
            }

            template <typename T>
            void List<T>::EnsureCapacity(const int min)
            {
                if(m_items.Length() < min)
                {
                    int newCapacity = m_items.Length() == 0 ? DefaultCapacity : static_cast<int>(m_items.Length()) * 2;

                    if(newCapacity < min)
                    {
                        newCapacity = min;
                    }

                    Capacity(newCapacity);
                }
            }
        }
    }
}

#endif