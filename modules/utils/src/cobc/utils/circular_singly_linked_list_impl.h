/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_CIRCULAR_SINGLY_LINKED_LIST_IMPL_H
#define COBC_CIRCULAR_SINGLY_LINKED_LIST_IMPL_H

#include "circular_singly_linked_list.h"

// ----------------------------------------------------------------------------
template <typename T>
cobc::CircularSinglyLinkedList<T>::CircularSinglyLinkedList() :
    mHead(0)
{
}

template <typename T>
cobc::CircularSinglyLinkedList<T>::~CircularSinglyLinkedList()
{
}

template <typename T>
bool
cobc::CircularSinglyLinkedList<T>::isEmpty() const
{
    return (mHead == 0);
}

template <typename T>
T*
cobc::CircularSinglyLinkedList<T>::first()
{
    return mHead->mNext;
}

template <typename T>
const T*
cobc::CircularSinglyLinkedList<T>::first() const
{
    return mHead->mNext;
}

template <typename T>
T*
cobc::CircularSinglyLinkedList<T>::last()
{
    return mHead;
}

template <typename T>
const T*
cobc::CircularSinglyLinkedList<T>::last() const
{
    return mHead;
}

template <typename T>
template <typename Condition>
T*
cobc::CircularSinglyLinkedList<T>::get(Condition condition)
{
    T* result = 0;
    if (mHead != 0)
    {
        T* current = mHead;
        do
        {
            current = current->mNext;
            if (condition(*current))
            {
                result = current;
            }
        } while ((result == 0) && (current != mHead));
    }
    return result;
}

template <typename T>
T*
cobc::CircularSinglyLinkedList<T>::getN(size_t n)
{
    T* result = 0;
    if (mHead != 0)
    {
        n++;
        T* current = mHead;
        do
        {
            current = current->mNext;
            n--;
        } while ((n > 0) && (current != mHead));

        if (n == 0)
        {
            // current points to the n-th element.
            result = current;
        }
    }

    return result;
}

template <typename T>
void
cobc::CircularSinglyLinkedList<T>::reset()
{
    mHead = 0;
}

// ----------------------------------------------------------------------------
template <typename T>
void
cobc::CircularSinglyLinkedList<T>::prepend(T* node)
{
    if (mHead == 0)
    {
        mHead = node;
        node->mNext = node;
    }
    else
    {
        node->mNext = mHead->mNext;
        mHead->mNext = node;
    }
}

// ----------------------------------------------------------------------------
template <typename T>
void
cobc::CircularSinglyLinkedList<T>::append(T* node)
{
    if (mHead == 0)
    {
        mHead = node;
        node->mNext = node;
    }
    else
    {
        node->mNext = mHead->mNext;
        mHead->mNext = node;
        mHead = node;
    }
}

template <typename T>
void
cobc::CircularSinglyLinkedList<T>::insert(T* node)
{
    if (mHead == 0)
    {
        // Node is the first in the list
        mHead = node;
        node->mNext = node;
    }
    else
    {
        // Traverse list until the list ends or a "bigger" entry is found.
        T* current = mHead;
        T* result = 0;
        do
        {
            current = current->mNext;
            if (*node < *current->mNext)
            {
                result = current;
            }
        } while ((result == 0) && (current != mHead));

        if ((result == 0) || (current == mHead))
        {
            // Add to end of the list, move head pointer to the new entry
            node->mNext = mHead->mNext;
            mHead->mNext = node;
            mHead = node;
        }
        else
        {
            // Add to the middle of the list
            node->mNext = result->mNext;
            result->mNext = node;
        }
    }
}

//// ----------------------------------------------------------------------------
template <typename T>
bool
cobc::CircularSinglyLinkedList<T>::removeNode(T* node)
{
    bool nodeFound = false;
    if (mHead != 0)
    {
        T* previous = 0;
        T* current  = mHead;

        // Iterate trough the list until the node is found
        T* result = 0;
        do
        {
            previous = current;
            current = current->mNext;
            if (current == node)
            {
                result = node;
            }
        } while ((result == 0) && (current != mHead));

        if (result != 0)
        {
            if (mHead == node)
            {
                if (mHead == mHead->mNext)
                {
                    mHead = 0;
                }
                else
                {
                    mHead = previous;
                }
            }
            previous->mNext = result->mNext;
            nodeFound = true;
        }
    }

    return nodeFound;
}

// ----------------------------------------------------------------------------
template <typename T>
template <typename Condition>
T*
cobc::CircularSinglyLinkedList<T>::remove(Condition condition)
{
    T* result = 0;
    if (mHead != 0)
    {
        T* previous = 0;
        T* current  = mHead;

        // Iterate trough the list until the node is found
        do
        {
            previous = current;
            current = current->mNext;
            if (condition(*current))
            {
                result = current;
            }
        } while ((result == 0) && (current != mHead));

        if (result != 0)
        {
            if (mHead == result)
            {
                if (mHead == mHead->mNext)
                {
                    mHead = 0;
                }
                else
                {
                    mHead = previous;
                }
            }
            previous->mNext = result->mNext;
        }
    }

    return result;
}

template <typename T>
template <typename Condition>
void
cobc::CircularSinglyLinkedList<T>::removeAll(Condition condition)
{
    if (mHead != 0)
    {
        T* previous = 0;
        T* current  = 0;

        T* nextPrevious = mHead;
        T* nextCurrent  = mHead->mNext;

        // Iterate trough the list until the node is found
        do
        {
            previous = nextPrevious;
            current  = nextCurrent;
            if (condition(*current))
            {
                // End of list reached
                if (mHead == current)
                {
                    if (mHead == mHead->mNext)
                    {
                        mHead = 0;
                    }
                    else
                    {
                        mHead = previous;
                        previous->mNext = current->mNext;
                    }
                    current = mHead;
                }
                else
                {
                    previous->mNext = current->mNext;

                    nextPrevious = previous;
                    nextCurrent = current->mNext;
                }
            }
            else
            {
                nextPrevious = current;
                nextCurrent = current->mNext;
            }
        } while (current != mHead);
    }
}

template <typename T>
template <typename Condition, typename PostCondition>
void
cobc::CircularSinglyLinkedList<T>::removeAll(Condition condition, PostCondition postCondition)
{
    if (mHead != 0)
    {
        T* previous = 0;
        T* current  = 0;

        T* nextPrevious = mHead;
        T* nextCurrent  = mHead->mNext;

        // Iterate trough the list until the node is found
        do
        {
            previous = nextPrevious;
            current  = nextCurrent;
            if (condition(*current))
            {
                // End of list reached
                if (mHead == current)
                {
                    if (mHead == mHead->mNext)
                    {
                        mHead = 0;
                    }
                    else
                    {
                        mHead = previous;
                        previous->mNext = current->mNext;
                    }
                    current = mHead;
                }
                else
                {
                    previous->mNext = current->mNext;

                    nextPrevious = previous;
                    nextCurrent = current->mNext;
                }

                postCondition(*current);
            }
            else
            {
                nextPrevious = current;
                nextCurrent = current->mNext;
            }
        } while (current != mHead);
    }
}

// ----------------------------------------------------------------------------
template <typename T>
void
cobc::CircularSinglyLinkedList<T>::removeFirst()
{
    if (mHead != 0)
    {
        if (mHead->mNext == mHead)
        {
            // Only one entry
            mHead = 0;
        }
        else
        {
            // Multiple entries
            T* next = mHead->mNext;
            mHead->mNext = next->mNext;
        }
    }
}

// ----------------------------------------------------------------------------
template<typename T>
size_t
cobc::CircularSinglyLinkedList<T>::size() const
{
    size_t numberOfElements = 0;
    if (mHead != 0)
    {
        T* current = mHead;
        do
        {
            current = current->mNext;
            numberOfElements++;
        } while (current != mHead);
    }
    return numberOfElements;
}

// ----------------------------------------------------------------------------
template <typename T>
cobc::CircularSinglyLinkedList<T>::Iterator::Iterator() :
    mNode(0),
    mLastNode(0)
{
}

template <typename T>
cobc::CircularSinglyLinkedList<T>::Iterator::Iterator(T* node, T* lastNode) :
    mNode(node),
    mLastNode(lastNode)
{
}

template <typename T>
cobc::CircularSinglyLinkedList<T>::Iterator::Iterator(const Iterator& other) :
    mNode(other.mNode),
    mLastNode(other.mLastNode)
{
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::Iterator&
cobc::CircularSinglyLinkedList<T>::Iterator::operator=(const Iterator& other)
{
    // Handles self assignment correctly
    this->mNode = other.mNode;
    this->mLastNode = other.mLastNode;
    return *this;
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::Iterator&
cobc::CircularSinglyLinkedList<T>::Iterator::operator++()
{
    if (mNode == mLastNode)
    {
        mNode = 0;
    }
    else
    {
        mNode = mNode->mNext;
    }
    return *this;
}

template <typename T>
bool
cobc::CircularSinglyLinkedList<T>::Iterator::operator==(const Iterator& other) const
{
    return (mNode == other.mNode);
}

template <typename T>
bool
cobc::CircularSinglyLinkedList<T>::Iterator::operator!=(const Iterator& other) const
{
    return (mNode != other.mNode);
}

template <typename T>
T&
cobc::CircularSinglyLinkedList<T>::Iterator::operator*()
{
    return *this->mNode;
}

template <typename T>
T*
cobc::CircularSinglyLinkedList<T>::Iterator::operator->()
{
    return this->mNode;
}

// ----------------------------------------------------------------------------
template <typename T>
cobc::CircularSinglyLinkedList<T>::ConstIterator::ConstIterator() :
    mNode(0),
    mLastNode(0)
{
}

template <typename T>
cobc::CircularSinglyLinkedList<T>::ConstIterator::ConstIterator(T* node, T* lastNode) :
    mNode(node),
    mLastNode(lastNode)
{
}

template <typename T>
cobc::CircularSinglyLinkedList<T>::ConstIterator::ConstIterator(const Iterator& other) :
    mNode(other.mNode),
    mLastNode(other.mLastNode)
{
}

template <typename T>
cobc::CircularSinglyLinkedList<T>::ConstIterator::ConstIterator(const ConstIterator& other) :
    mNode(other.mNode),
    mLastNode(other.mLastNode)
{
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::ConstIterator&
cobc::CircularSinglyLinkedList<T>::ConstIterator::operator=(const ConstIterator& other)
{
    // Handles self assignment correctly
    this->mNode = other.mNode;
    this->mLastNode = other.mLastNode;
    return *this;
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::ConstIterator&
cobc::CircularSinglyLinkedList<T>::ConstIterator::operator++()
{
    if (mNode == mLastNode)
    {
        mNode = 0;
    }
    else
    {
        mNode = mNode->mNext;
    }
    return *this;
}

template <typename T>
bool
cobc::CircularSinglyLinkedList<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return (mNode == other.mNode);
}

template <typename T>
bool
cobc::CircularSinglyLinkedList<T>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return (mNode != other.mNode);
}

template <typename T>
const T&
cobc::CircularSinglyLinkedList<T>::ConstIterator::operator*() const
{
    return *this->mNode;
}

template <typename T>
const T*
cobc::CircularSinglyLinkedList<T>::ConstIterator::operator->() const
{
    return this->mNode;
}

// ----------------------------------------------------------------------------
template <typename T>
typename cobc::CircularSinglyLinkedList<T>::Iterator
cobc::CircularSinglyLinkedList<T>::begin()
{
    Iterator it(0, 0);
    if (mHead != 0)
    {
        it = Iterator(mHead->mNext, mHead);
    }
    return it;
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::Iterator
cobc::CircularSinglyLinkedList<T>::end()
{
    Iterator it(0, 0);
    return it;
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::ConstIterator
cobc::CircularSinglyLinkedList<T>::begin() const
{
    ConstIterator it(0, 0);
    if (mHead != 0)
    {
        it = ConstIterator(mHead->mNext, mHead);
    }
    return it;
}

template <typename T>
typename cobc::CircularSinglyLinkedList<T>::ConstIterator
cobc::CircularSinglyLinkedList<T>::end() const
{
    ConstIterator it(0, 0);
    return it;
}

#endif
