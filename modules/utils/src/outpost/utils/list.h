/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef COBC_LIST_H
#define COBC_LIST_H

#include <stddef.h>

namespace outpost
{
/**
 * Singly-linked list with external storage.
 *
 * The nodes must provide a "mNext"-member.
 *
 * \author    Fabian Greif
 */
template <typename T>
class List
{
public:
    /**
     * Construct an empty list.
     */
    List();

    /**
     * Destructor.
     *
     * \warning
     *     The items contained in the list are not destroyed by deleting
     *     the list. The list does not take ownership of items added to it,
     *     therefore all items have to be destroyed by their original creator.
     */
    ~List();

    /**
     * Remove all entries from the list.
     *
     * The nodes are not changed only the link to the first node is
     * removed!
     *
     * O(1)
     */
    void
    reset();

    /**
     * Check if the list is empty.
     */
    bool
    isEmpty() const;

    /**
     * Get first element of the list.
     *
     * O(1)
     */
    T*
    first();

    const T*
    first() const;

    template <typename Condition>
    T*
    get(Condition condition);

    /**
     * Get the nth element of the list.
     *
     * O(N)
     */
    T*
    getN(size_t n);

    /**
     * Add a node to the front of the list.
     *
     * O(1)
     */
    void
    prepend(T* node);

    /**
     * Insert a node sorted into the list.
     *
     * The class T has to be comparable via operator<. This comparison
     * shall produce a strict weak ordering of the elements.
     *
     * O(N)
     */
    void
    insert(T* node);

    /**
     * Remove a node from the list.
     *
     * As this is a singly linked list this method as to iterate through
     * the list to find the predecessor.
     *
     * O(N)
     *
     * \retval \c true if the node was found and removed from list,
     * \retval \c false if the node is not in the list.
     */
    bool
    removeNode(T* node);

    /**
     * Remove the first node that satisfy the given condition.
     *
     * O(N)
     *
     * \return  Node that has been removed from the list.
     */
    template <typename Condition>
    T*
    remove(Condition condition);

    /**
     * Remove all nodes that satisfy the given condition.
     *
     * O(N)
     */
    template <typename Condition>
    void
    removeAll(Condition condition);

    /**
     * Remove all nodes that satisfy the given condition and apply
     * a postcondition to the removed nodes.
     *
     * O(N)
     */
    template <typename Condition, typename PostCondition>
    void
    removeAll(Condition condition, PostCondition postCondition);

    /**
     * O(N)
     */
    size_t
    size() const;

    /**
     * Remove the first node from the list.
     *
     * O(1)
     */
    void
    removeFirst();

    class Iterator
    {
    public:
        friend class List;
        friend class ConstIterator;

        Iterator();

        Iterator(const Iterator& other);

        Iterator&
        operator=(const Iterator& other);

        Iterator&
        operator++();

        bool
        operator==(const Iterator& other) const;

        bool
        operator!=(const Iterator& other) const;

        T&
        operator*();

        T*
        operator->();

    private:
        explicit
        Iterator(T* node);

        /// Pointer to the current node. Set to NULL if end of list.
        T* mNode;
    };

    class ConstIterator
    {
    public:
        friend class List;

        ConstIterator();

        ConstIterator(const ConstIterator& other);

        ConstIterator(const Iterator& other);

        ConstIterator&
        operator=(const ConstIterator& other);

        ConstIterator&
        operator++();

        bool
        operator==(const ConstIterator& other) const;

        bool
        operator!=(const ConstIterator& other) const;

        const T&
        operator*() const;

        const T*
        operator->() const;

    private:
        explicit
        ConstIterator(T* node);

        /// Pointer to the current node. Set to NULL if end of list.
        T* mNode;
    };

    Iterator
    begin();

    Iterator
    end();

    ConstIterator
    begin() const;

    ConstIterator
    end() const;

private:
    T* mHead;

    // disable copy constructor
    List(const List&);

    // disable assignment operator
    List&
    operator=(const List&);
};
}

#include "list_impl.h"

#endif // COBC_LIST_H
