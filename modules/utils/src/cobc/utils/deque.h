/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_DEQUE_HPP
#define COBC_DEQUE_HPP

#include <stdint.h>
#include <stddef.h>

#include <cobc/utils.h>

namespace cobc
{
/**
 * Double ended queue.
 *
 * Internally organized as a ring buffer.
 *
 *                 tail --\                 /-- head
 *                        |                 |
 *              +------+------+---- ----+------+------+
 *            0 |      | data |   ...   | data |      | N-1
 *              +------+------+---- ----+------+------+
 *                 |      |                 |      |
 *     prepend() --/      |                 |      \-- append()
 *                        |                 |
 *           getFront() --/                 \-- getBack()
 *
 * \tparam  T   Type of the elements
 *
 * \warning This class don't check if the container is empty before
 *          a pop-operation. You have to do this by yourself!
 *
 * \author  Fabian Greif
 */
template<typename T>
class Deque
{
public:
    typedef size_t Index;
    typedef Index Size;

public:
    Deque(T* backendBuffer,
          size_t n);

    inline bool
    isEmpty() const;

    inline bool
    isFull() const;

    inline Size
    getSize() const;

    inline Size
    getMaxSize() const;

    inline Size
    getAvailableSpace() const;

    /**
     * Clear the queue.
     *
     * \warning    This will discard all the items in the container
     */
    void
    clear();


    inline T&
    getFront();

    inline const T&
    getFront() const;

    inline T&
    getBack();

    inline const T&
    getBack() const;


    bool
    append(const T& value);

    bool
    prepend(const T& value);

    void
    removeBack();

    void
    removeFront();

private:
    // disable copy constructor
    Deque(const Deque&);

    // disable assignment operator
    Deque&
    operator =(const Deque&);

    T* const buffer;
    const Size maxSize;

    Index head;
    Index tail;
    Size size;
};
}

#include "deque_impl.h"

#endif // COBC_DEQUE_HPP
