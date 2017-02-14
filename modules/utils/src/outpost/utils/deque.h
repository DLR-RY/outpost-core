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

#ifndef COBC_DEQUE_H
#define COBC_DEQUE_H

#include <stdint.h>
#include <stddef.h>

#include "meta.h"
#include "bounded_array.h"

namespace outpost
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

    explicit
    Deque(outpost::BoundedArray<T> backendBuffer);

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
    operator=(const Deque&);

    T* const mBuffer;
    const Size mMaxSize;

    Index mHead;
    Index mTail;
    Size mSize;
};
}

#include "deque_impl.h"

#endif
