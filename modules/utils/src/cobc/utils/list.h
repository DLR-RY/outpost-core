/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_LIST_H
#define COBC_LIST_H

namespace cobc
{
/**
 * Singly-linked list with external storage.
 *
 * The nodes must provide a next-member.
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
     * Add a node to the front of the list.
     *
     * O(1)
     */
    void
    add(T* node);

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
     * a postcondition to the freed node.
     *
     * O(N)
     */
    template <typename Condition, typename PostCondition>
    void
    removeAll(Condition condition, PostCondition postCondition);

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

        Iterator();

        Iterator(const Iterator& other);

        Iterator&
        operator =(const Iterator& other);

        Iterator&
        operator ++();

        bool
        operator ==(const Iterator& other) const;

        bool
        operator !=(const Iterator& other) const;

        T&
        operator *();

        T*
        operator->();

    private:
        Iterator(T* node);

        /// Pointer to the next node. Set to NULL if end of list.
        T* node;
    };

    Iterator
    begin();

    Iterator
    end();

private:
    T* head;

    // disable copy constructor
    List(const List&);

    // disable assignment operator
    List&
    operator =(const List&);
};
}

#include "list_impl.h"

#endif // COBC_LIST_H
