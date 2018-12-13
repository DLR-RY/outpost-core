/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2018, Fabian Greif (DLR RY-AVS)
 * - 2018, Olaf Maibaum (DLR SC-OSS)
 */

#ifndef OUTPOST_DEQUE_H
#define OUTPOST_DEQUE_H

#include <outpost/base/slice.h>
#include <outpost/utils/meta.h>

#include <stddef.h>
#include <stdint.h>

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
template <typename T>
class Deque
{
public:
    typedef size_t Index;
    typedef Index Size;

public:
    Deque(T* backendBuffer, size_t n);

    explicit Deque(outpost::Slice<T> backendBuffer);

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

    /**
     * \param value One value to append to the deque.
     *
     * \result True when value is appended.
     */
    bool
    append(const T& value);

    /**
     * \param values A slice to append to the deque. If not enough space is available in the deque,
     * deque is filled up with the first values from the slice until it is full.
     *
     * \result Number of appended values.
     */
    size_t
    append(const outpost::Slice<T>& values);

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
}  // namespace outpost

#include "deque_impl.h"

#endif
