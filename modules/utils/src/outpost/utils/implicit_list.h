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

#ifndef COBC_UTILS_IMPLICIT_LIST_H
#define COBC_UTILS_IMPLICIT_LIST_H

namespace outpost
{

/**
 * Static single linked list.
 *
 * TODO example
 *
 * This implementation relies for the \c next member on the fact that
 * zero-initialized static variables are initialized before any constructor
 * is called.
 *
 * See chapter 3.6.2 of the C++03 standard:
 *
 *     Objects with static storage duration (3.7.1) shall be
 *     zero-initialized (8.5) before any other initialization
 *     takes place. Zero-initialization and initialization with a
 *     constant expression are collectively called static
 *     initialization; all other initialization is dynamic
 *     initialization. Objects of POD types (3.9) with static storage
 *     duration initialized with constant expressions (5.19) shall be
 *     initialized before any dynamic initialization takes place.
 *
 * This does only apply to static member variables of classes and
 * global variables.
 */
template <typename T>
class ImplicitList
{
public:
    /**
     * Add element to the list.
     *
     * For an example on how to use this class see topic.cpp
     *
     * \param list
     *         List to which to add the element.
     * \param element
     *         Element to add to the list (mostly \c this).
     */
    inline
    ImplicitList(T*& list,
                 T* element) :
        mNext(list)
    {
        list = element;
    }

    /**
     * Get next element in the list.
     *
     * \return    Next element or zero if the end of the list is reached.
     */
    inline T*
    getNext()
    {
        return mNext;
    }

    /**
     * Remove an element from the list.
     *
     * \param head
     *         Head of the list.
     * \param element
     *         Element to remove.
     */
    static inline void
    removeFromList(T** head,
                   T* element)
    {
        if (*head != 0)
        {
            if (*head == element)
            {
                *head = element->mNext;
            }
            else
            {
                T* previous = *head;
                T* node = (*head)->mNext;

                while (node != element)
                {
                    previous = node;
                    node = node->mNext;
                }

                previous->mNext = node->mNext;
            }
        }
    }


private:
    // disable copy constructor
    ImplicitList(const ImplicitList&);

    // disable assignment operator
    ImplicitList&
    operator=(const ImplicitList&);

    /// Pointer to the next element
    T* mNext;
};

}

#endif // COBC_UTILS_IMPLICIT_LIST_H
