/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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

#ifndef COBC_LIST_IMPL_H
#define COBC_LIST_IMPL_H

#include "list.h"

// ----------------------------------------------------------------------------
template <typename T>
cobc::List<T>::List() :
    head(0)
{
}

template <typename T>
cobc::List<T>::~List()
{
}

template <typename T>
bool
cobc::List<T>::isEmpty() const
{
    return (head == 0);
}

template <typename T>
T*
cobc::List<T>::first()
{
    return head;
}

template <typename T>
const T*
cobc::List<T>::first() const
{
    return head;
}

template <typename T>
template <typename Condition>
T*
cobc::List<T>::get(Condition condition)
{
    T* current = head;
    while ((current != 0) && !condition(*current))
    {
        current = current->next;
    }
    return current;
}

template <typename T>
void
cobc::List<T>::reset()
{
    head = 0;
}

// ----------------------------------------------------------------------------
template <typename T>
void
cobc::List<T>::add(T* node)
{
    node->next = head;
    head = node;
}

template <typename T>
void
cobc::List<T>::insert(T* node)
{
    if ((head == 0) || (*node < *head))
    {
        // Node is the first in the list
        node->next = head;
        head = node;
    }
    else
    {
        // Traverse list until the list ends or a "bigger" entry is found.
        T* current = head;
        while ((current->next != 0) && (*current->next < *node))
        {
            current = current->next;
        }

        // Add to list
        node->next = current->next;
        current->next = node;
    }
}

// ----------------------------------------------------------------------------
template <typename T>
bool
cobc::List<T>::removeNode(T* node)
{
    bool nodeFound = false;
    if (head != 0)
    {
        if (head == node)
        {
            // Entry is the first one in the list
            head = head->next;
            nodeFound = true;
        }
        else
        {
            T* previous = head;
            T* current  = head->next;

            // Iterate trough the list until the node is found
            while ((current != 0) && (current != node))
            {
                previous = current;
                current = current->next;
            }

            if (current != 0)
            {
                previous->next = current->next;
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
cobc::List<T>::remove(Condition condition)
{
    T* node = 0;

    if (head != 0)
    {
        if (condition(*head))
        {
            // Entry is the first one in the list
            node = head;
            head = head->next;
        }
        else
        {
            T* previous = head;
            node        = head->next;

            // Iterate trough the list until the node is found
            while ((node != 0) && !condition(*node))
            {
                previous = node;
                node = node->next;
            }

            if (node != 0)
            {
                // remove node from the list
                previous->next = node->next;
            }
        }
    }

    return node;
}

template <typename T>
template <typename Condition>
void
cobc::List<T>::removeAll(Condition condition)
{
    if (head != 0)
    {
        // List is not empty
        T* previous = head;
        T* current  = head->next;

        // Iterate trough the list and check all nodes. Iteration is started
        // at the entry after the first one as the first entry needs a special
        // handling.
        while (current != 0)
        {
            if (condition(*current))
            {
                previous->next = current->next;
            }
            else
            {
                previous = current;
            }
            current = current->next;
        }

        // Check first entry in the list
        if (condition(*head))
        {
            head = head->next;
        }
    }
}

template <typename T>
template <typename Condition, typename PostCondition>
void
cobc::List<T>::removeAll(Condition condition, PostCondition postCondition)
{
    if (head != 0)
    {
        // List is not empty
        T* previous = head;
        T* current  = head->next;

        // Iterate trough the list and check all nodes. Iteration is started
        // at the entry after the first one as the first entry needs a special
        // handling.
        while (current != 0)
        {
            T* node = current;
            if (condition(*node))
            {
                postCondition(*node);
                previous->next = current->next;
            }
            else
            {
                previous = current;
            }
            current = current->next;
        }

        // Check first entry in the list
        if (condition(*head))
        {
            postCondition(*head);
            head = head->next;
        }
    }
}

// ----------------------------------------------------------------------------
template <typename T>
void
cobc::List<T>::removeFirst()
{
    if (head != 0)
    {
        T* next = head->next;
        head->next = 0;
        head = next;
    }
}

// ----------------------------------------------------------------------------
template <typename T>
cobc::List<T>::Iterator::Iterator() :
    node(0)
{
}

template <typename T>
cobc::List<T>::Iterator::Iterator(T* node) :
    node(node)
{
}

template <typename T>
cobc::List<T>::Iterator::Iterator(const Iterator& other) :
    node(other.node)
{
}

// ----------------------------------------------------------------------------
template <typename T>
typename cobc::List<T>::Iterator&
cobc::List<T>::Iterator::operator=(const Iterator& other)
{
    // Handles self assignment correctly
    this->node = other.node;
    return *this;
}

template <typename T>
typename cobc::List<T>::Iterator&
cobc::List<T>::Iterator::operator++()
{
    this->node = this->node->next;
    return *this;
}

template <typename T>
bool
cobc::List<T>::Iterator::operator==(
const Iterator& other) const
{
    return (node == other.node);
}

template <typename T>
bool
cobc::List<T>::Iterator::operator!=(
const Iterator& other) const
{
    return (node != other.node);
}

template <typename T>
T&
cobc::List<T>::Iterator::operator*()
{
    return this->node;
}

template <typename T>
T*
cobc::List<T>::Iterator::operator->()
{
    return &this->node;
}

// ----------------------------------------------------------------------------
template <typename T>
typename cobc::List<T>::Iterator
cobc::List<T>::begin()
{
    return Iterator(this->head);
}

template <typename T>
typename cobc::List<T>::Iterator
cobc::List<T>::end()
{
    return Iterator(0);
}

#endif // COBC_LIST_IMPL_H
