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
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_LIST_IMPL_H
#define OUTPOST_LIST_IMPL_H

#include "list.h"

#include <type_traits>

// ----------------------------------------------------------------------------
template <typename T>
outpost::List<T>::List() : mHead(nullptr), mTail(nullptr)
{
    static_assert(std::is_base_of<outpost::ListElement, T>::value,
                  "T not derived from ListElement");
}

template <typename T>
outpost::List<T>::~List()
{
}

template <typename T>
bool
outpost::List<T>::isEmpty() const
{
    return (mHead == nullptr);
}

template <typename T>
T*
outpost::List<T>::first()
{
    return static_cast<T*>(mHead);
}

template <typename T>
const T*
outpost::List<T>::first() const
{
    return static_cast<const T*>(mHead);
}

template <typename T>
const T*
outpost::List<T>::last() const
{
    return static_cast<const T*>(mTail);
}

template <typename T>
T*
outpost::List<T>::last()
{
    return static_cast<T*>(mTail);
}

template <typename T>
template <typename Condition>
T*
outpost::List<T>::get(Condition condition)
{
    T* current = static_cast<T*>(mHead);
    while ((current != nullptr) && !condition(*current))
    {
        current = static_cast<T*>(current->mNext);
    }
    return current;
}

template <typename T>
T*
outpost::List<T>::getN(size_t n)
{
    ListElement* current = mHead;
    while ((current != nullptr) && (n > 0))
    {
        current = current->mNext;
        n--;
    }

    if (n > 0)
    {
        // n is greater than the size
        current = nullptr;
    }
    else
    {
        // current points to the n-th element.
    }

    return static_cast<T*>(current);
}

template <typename T>
void
outpost::List<T>::reset()
{
    mHead = nullptr;
    mTail = nullptr;
}

// ----------------------------------------------------------------------------
template <typename T>
void
outpost::List<T>::prepend(T* node)
{
    // for empty list we alos have to adapt the tail
    if (mTail == nullptr)
    {
        mTail = node;
    }

    node->mNext = mHead;
    mHead = node;
}

template <typename T>
void
outpost::List<T>::append(T* node)
{
    if (mHead == nullptr)
    {
        mHead = node;
    }
    else
    {
        mTail->mNext = node;
    }
    mTail = node;
    node->mNext = nullptr;
}

template <typename T>
void
outpost::List<T>::insert(T* node)
{
    node->mNext = nullptr;
    if (mHead == nullptr)
    {
        // empty list
        mHead = node;
        mTail = node;
    }
    else if (*node < *static_cast<T*>(mHead))
    {
        // Node is the first in the list
        node->mNext = mHead;
        mHead = node;
    }
    else if (*static_cast<T*>(mTail) < *node)
    {
        mTail->mNext = node;
        mTail = node;
    }
    else
    {
        // Traverse list until the list ends or a "bigger" entry is found.
        ListElement* current = mHead;
        while ((current->mNext != nullptr) && (*static_cast<T*>(current->mNext) < *node))
        {
            current = current->mNext;
        }

        // insert at end
        if (current->mNext == nullptr)
        {
            mTail = node;
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
    if (mHead != nullptr)
    {
        if (mHead == node)
        {
            // Entry is the first one in the list
            mHead = mHead->mNext;

            if (mHead == nullptr)
            {
                mTail = nullptr;
            }

            node->mNext = nullptr;
            nodeFound = true;
        }
        else
        {
            ListElement* previous = mHead;
            ListElement* current = mHead->mNext;

            // Iterate trough the list until the node is found
            while ((current != nullptr) && (current != node))
            {
                previous = current;
                current = current->mNext;
            }

            if (current != 0)
            {
                previous->mNext = current->mNext;

                if (previous->mNext == nullptr)
                {
                    mTail = previous;
                }

                node->mNext = nullptr;
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
    T* node = nullptr;

    if (mHead != nullptr)
    {
        if (condition(*static_cast<T*>(mHead)))
        {
            // Entry is the first one in the list
            node = static_cast<T*>(mHead);
            mHead = mHead->mNext;

            if (mHead == nullptr)
            {
                mTail = nullptr;
            }

            node->mNext = nullptr;
        }
        else
        {
            ListElement* previous = mHead;
            node = static_cast<T*>(mHead->mNext);

            // Iterate trough the list until the node is found
            while ((node != nullptr) && !condition(*node))
            {
                previous = node;
                node = static_cast<T*>(node->mNext);
            }

            if (node != nullptr)
            {
                // remove node from the list
                previous->mNext = node->mNext;

                if (previous->mNext == nullptr)
                {
                    mTail = previous;
                }
                node->mNext = nullptr;
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
    ListElement* previous = nullptr;
    ListElement* current = static_cast<T*>(mHead);

    // Iterate trough the list and check all nodes.
    while (current != nullptr)
    {
        if (condition(*static_cast<T*>(current)))
        {
            // the removed one is the head -> correct list
            if (previous == nullptr)
            {
                mHead = current->mNext;
                if (mHead == nullptr)
                {
                    mTail = nullptr;
                }
            }
            else
            {
                previous->mNext = current->mNext;
                if (previous->mNext == nullptr)
                {
                    mTail = previous;
                }
            }
            ListElement* tmp = current;
            current = current->mNext;
            tmp->mNext = nullptr;
        }
        else
        {
            previous = current;
            current = current->mNext;
        }
    }
}

template <typename T>
template <typename Condition, typename PostCondition>
void
outpost::List<T>::removeAll(Condition condition, PostCondition postCondition)
{
    ListElement* previous = nullptr;
    ListElement* current = static_cast<T*>(mHead);

    // Iterate trough the list and check all nodes.
    while (current != nullptr)
    {
        T* node = static_cast<T*>(current);
        if (condition(*node))
        {
            // the removed one is the head -> correct list
            if (previous == nullptr)
            {
                mHead = current->mNext;
                if (mHead == nullptr)
                {
                    mTail = nullptr;
                }
            }
            else
            {
                previous->mNext = current->mNext;
                if (previous->mNext == nullptr)
                {
                    mTail = previous;
                }
            }
            current = current->mNext;
            node->mNext = nullptr;

            postCondition(*node);
        }
        else
        {
            previous = current;
            current = current->mNext;
        }
    }
}

// ----------------------------------------------------------------------------
template <typename T>
void
outpost::List<T>::removeFirst()
{
    if (mHead != nullptr)
    {
        ListElement* next = mHead->mNext;
        mHead->mNext = nullptr;
        mHead = next;

        if (mHead == nullptr)
        {
            mTail = nullptr;
        }
    }
}

// ----------------------------------------------------------------------------
template <typename T>
size_t
outpost::List<T>::size() const
{
    size_t numberOfElements = 0;
    ListElement* current = mHead;
    while (current != nullptr)
    {
        current = current->mNext;
        numberOfElements++;
    }
    return numberOfElements;
}

// ----------------------------------------------------------------------------
template <typename T>
outpost::List<T>::Iterator::Iterator() : mNode(nullptr)
{
}

template <typename T>
outpost::List<T>::Iterator::Iterator(T* node) : mNode(node)
{
}

template <typename T>
outpost::List<T>::Iterator::Iterator(const Iterator& other) : mNode(other.mNode)
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
    this->mNode = static_cast<T*>(this->mNode->mNext);
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
T& outpost::List<T>::Iterator::operator*()
{
    return *this->mNode;
}

template <typename T>
T* outpost::List<T>::Iterator::operator->()
{
    return this->mNode;
}

// ----------------------------------------------------------------------------
template <typename T>
outpost::List<T>::ConstIterator::ConstIterator() : mNode(0)
{
}

template <typename T>
outpost::List<T>::ConstIterator::ConstIterator(const T* node) : mNode(node)
{
}

template <typename T>
outpost::List<T>::ConstIterator::ConstIterator(const ConstIterator& other) : mNode(other.mNode)
{
}

template <typename T>
outpost::List<T>::ConstIterator::ConstIterator(const Iterator& other) : mNode(other.mNode)
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
    this->mNode = static_cast<const T*>(this->mNode->mNext);
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
const T& outpost::List<T>::ConstIterator::operator*() const
{
    return *this->mNode;
}

template <typename T>
const T* outpost::List<T>::ConstIterator::operator->() const
{
    return this->mNode;
}

// ----------------------------------------------------------------------------
template <typename T>
typename outpost::List<T>::Iterator
outpost::List<T>::begin()
{
    Iterator it(static_cast<T*>(this->mHead));
    return it;
}

template <typename T>
typename outpost::List<T>::Iterator
outpost::List<T>::end()
{
    Iterator it(nullptr);
    return it;
}

template <typename T>
typename outpost::List<T>::ConstIterator
outpost::List<T>::begin() const
{
    ConstIterator it(static_cast<const T*>(this->mHead));
    return it;
}

template <typename T>
typename outpost::List<T>::ConstIterator
outpost::List<T>::end() const
{
    ConstIterator it(nullptr);
    return it;
}

#endif
