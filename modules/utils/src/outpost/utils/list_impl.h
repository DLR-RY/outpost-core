/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 * - 2015, Janosch Reinking (DLR RY-AVS)
 * - 2016, Jan Sommer (DLR SC-SRV)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_LIST_IMPL_H
#define COBC_LIST_IMPL_H

#include "list.h"

// ----------------------------------------------------------------------------
template <typename T>
outpost::List<T>::List() :
    mHead(0)
{
}

template <typename T>
outpost::List<T>::~List()
{
}

template <typename T>
bool
outpost::List<T>::isEmpty() const
{
    return (mHead == 0);
}

template <typename T>
T*
outpost::List<T>::first()
{
    return mHead;
}

template <typename T>
const T*
outpost::List<T>::first() const
{
    return mHead;
}

template <typename T>
template <typename Condition>
T*
outpost::List<T>::get(Condition condition)
{
    T* current = mHead;
    while ((current != 0) && !condition(*current))
    {
        current = current->mNext;
    }
    return current;
}

template <typename T>
T*
outpost::List<T>::getN(size_t n)
{
    T* current = mHead;
    while ((current != 0) && (n > 0))
    {
        current = current->mNext;
        n--;
    }

    if (n > 0)
    {
        // n is greater than the size
        current = 0;
    }
    else
    {
        // current points to the n-th element.
    }

    return current;
}

template <typename T>
void
outpost::List<T>::reset()
{
    mHead = 0;
}

// ----------------------------------------------------------------------------
template <typename T>
void
outpost::List<T>::prepend(T* node)
{
    node->mNext = mHead;
    mHead = node;
}

template <typename T>
void
outpost::List<T>::insert(T* node)
{
    if ((mHead == 0) || (*node < *mHead))
    {
        // Node is the first in the list
        node->mNext = mHead;
        mHead = node;
    }
    else
    {
        // Traverse list until the list ends or a "bigger" entry is found.
        T* current = mHead;
        while ((current->mNext != 0) && (*current->mNext < *node))
        {
            current = current->mNext;
        }

        // Add to list
        node->mNext = current->mNext;
        current->mNext = node;
    }
}

// ----------------------------------------------------------------------------
template <typename T>
bool
outpost::List<T>::removeNode(T* node)
{
    bool nodeFound = false;
    if (mHead != 0)
    {
        if (mHead == node)
        {
            // Entry is the first one in the list
            mHead = mHead->mNext;
            nodeFound = true;
        }
        else
        {
            T* previous = mHead;
            T* current  = mHead->mNext;

            // Iterate trough the list until the node is found
            while ((current != 0) && (current != node))
            {
                previous = current;
                current = current->mNext;
            }

            if (current != 0)
            {
                previous->mNext = current->mNext;
                nodeFound = true;
            }
        }
    }

    return nodeFound;
}

// ----------------------------------------------------------------------------
template <typename T>
template <typename Condition>
T*
outpost::List<T>::remove(Condition condition)
{
    T* node = 0;

    if (mHead != 0)
    {
        if (condition(*mHead))
        {
            // Entry is the first one in the list
            node = mHead;
            mHead = mHead->mNext;
        }
        else
        {
            T* previous = mHead;
            node        = mHead->mNext;

            // Iterate trough the list until the node is found
            while ((node != 0) && !condition(*node))
            {
                previous = node;
                node = node->mNext;
            }

            if (node != 0)
            {
                // remove node from the list
                previous->mNext = node->mNext;
            }
        }
    }

    return node;
}

template <typename T>
template <typename Condition>
void
outpost::List<T>::removeAll(Condition condition)
{
    if (mHead != 0)
    {
        // List is not empty
        T* previous = mHead;
        T* current  = mHead->mNext;

        // Iterate trough the list and check all nodes. Iteration is started
        // at the entry after the first one as the first entry needs a special
        // handling.
        while (current != 0)
        {
            if (condition(*current))
            {
                previous->mNext = current->mNext;
            }
            else
            {
                previous = current;
            }
            current = current->mNext;
        }

        // Check first entry in the list
        if (condition(*mHead))
        {
            mHead = mHead->mNext;
        }
    }
}

template <typename T>
template <typename Condition, typename PostCondition>
void
outpost::List<T>::removeAll(Condition condition, PostCondition postCondition)
{
    if (mHead != 0)
    {
        // List is not empty
        T* previous = mHead;
        T* current  = mHead->mNext;

        // Iterate trough the list and check all nodes. Iteration is started
        // at the entry after the first one as the first entry needs a special
        // handling.
        while (current != 0)
        {
            T* node = current;
            if (condition(*node))
            {
                previous->mNext = current->mNext;
                current = current->mNext;

                postCondition(*node);
            }
            else
            {
                previous = current;
                current = current->mNext;
            }
        }

        // Check first entry in the list
        if (condition(*mHead))
        {
            T* node = mHead;
            mHead = mHead->mNext;

            postCondition(*node);
        }
    }
}

// ----------------------------------------------------------------------------
template <typename T>
void
outpost::List<T>::removeFirst()
{
    if (mHead != 0)
    {
        T* next = mHead->mNext;
        mHead->mNext = 0;
        mHead = next;
    }
}

// ----------------------------------------------------------------------------
template<typename T>
size_t
outpost::List<T>::size() const
{
    size_t numberOfElements = 0;
    T* current = mHead;
    while (current != 0)
    {
        current = current->mNext;
        numberOfElements++;
    }
    return numberOfElements;
}

// ----------------------------------------------------------------------------
template <typename T>
outpost::List<T>::Iterator::Iterator() :
    mNode(0)
{
}

template <typename T>
outpost::List<T>::Iterator::Iterator(T* node) :
    mNode(node)
{
}

template <typename T>
outpost::List<T>::Iterator::Iterator(const Iterator& other) :
    mNode(other.mNode)
{
}

template <typename T>
typename outpost::List<T>::Iterator&
outpost::List<T>::Iterator::operator=(const Iterator& other)
{
    // Handles self assignment correctly
    this->mNode = other.mNode;
    return *this;
}

template <typename T>
typename outpost::List<T>::Iterator&
outpost::List<T>::Iterator::operator++()
{
    this->mNode = this->mNode->mNext;
    return *this;
}

template <typename T>
bool
outpost::List<T>::Iterator::operator==(const Iterator& other) const
{
    return (mNode == other.mNode);
}

template <typename T>
bool
outpost::List<T>::Iterator::operator!=(const Iterator& other) const
{
    return (mNode != other.mNode);
}

template <typename T>
T&
outpost::List<T>::Iterator::operator*()
{
    return *this->mNode;
}

template <typename T>
T*
outpost::List<T>::Iterator::operator->()
{
    return this->mNode;
}

// ----------------------------------------------------------------------------
template <typename T>
outpost::List<T>::ConstIterator::ConstIterator() :
    mNode(0)
{
}

template <typename T>
outpost::List<T>::ConstIterator::ConstIterator(T* node) :
    mNode(node)
{
}

template <typename T>
outpost::List<T>::ConstIterator::ConstIterator(const ConstIterator& other) :
    mNode(other.mNode)
{
}

template <typename T>
outpost::List<T>::ConstIterator::ConstIterator(const Iterator& other) :
    mNode(other.mNode)
{
}

template <typename T>
typename outpost::List<T>::ConstIterator&
outpost::List<T>::ConstIterator::operator=(const ConstIterator& other)
{
    // Handles self assignment correctly
    this->mNode = other.mNode;
    return *this;
}

template <typename T>
typename outpost::List<T>::ConstIterator&
outpost::List<T>::ConstIterator::operator++()
{
    this->mNode = this->mNode->mNext;
    return *this;
}

template <typename T>
bool
outpost::List<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return (mNode == other.mNode);
}

template <typename T>
bool
outpost::List<T>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return (mNode != other.mNode);
}

template <typename T>
const T&
outpost::List<T>::ConstIterator::operator*() const
{
    return *this->mNode;
}

template <typename T>
const T*
outpost::List<T>::ConstIterator::operator->() const
{
    return this->mNode;
}

// ----------------------------------------------------------------------------
template <typename T>
typename outpost::List<T>::Iterator
outpost::List<T>::begin()
{
    Iterator it(this->mHead);
    return it;
}

template <typename T>
typename outpost::List<T>::Iterator
outpost::List<T>::end()
{
    Iterator it(0);
    return it;
}

template <typename T>
typename outpost::List<T>::ConstIterator
outpost::List<T>::begin() const
{
    ConstIterator it(this->mHead);
    return it;
}

template <typename T>
typename outpost::List<T>::ConstIterator
outpost::List<T>::end() const
{
    ConstIterator it(0);
    return it;
}

#endif // COBC_LIST_IMPL_H
