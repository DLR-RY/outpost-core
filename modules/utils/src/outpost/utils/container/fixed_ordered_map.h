/*
 * Copyright (c) 2015-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_FIXED_ORDERED_MAP_H
#define OUTPOST_UTILS_FIXED_ORDERED_MAP_H

#include <outpost/base/slice.h>
#include <outpost/utils/iterator.h>

#include <stddef.h>
#include <stdint.h>

namespace outpost
{
/**
 * Performs a binary search over the ID of all stored functions.
 *
 * The `Entry` type need to implement the following interface:
 * \code
 * class Interface
 * {
 * public:
 *     Key mKey;
 * };
 * \endcode
 *
 * The `mKey` member is used to find a specific entry in the list of entries.
 *
 * \author  Fabian Greif
 */
template <typename Entry, typename Key>
class FixedOrderedMap
{
public:
    typedef DefaultArrayIterator<Entry> Iterator;
    typedef DefaultConstArrayIterator<Entry> ConstIterator;

    /**
     * Create list from an array without explicit size.
     *
     * \param entries
     *      C style array of entries.
     */
    template <size_t N>
    explicit inline FixedOrderedMap(Entry (&entries)[N]) : mEntries(entries), mNumberOfEntries(N)
    {
    }

    /**
     * Constructor.
     *
     * \param entries
     *      List of all entries. The entries must be ordered with a
     *      ascending mKey value!
     * \param numberOfEntries
     *      Number of entries in the list. The number of entries must be
     *      greater than zero!
     */
    FixedOrderedMap(Entry* entries, size_t numberOfEntries);

    explicit inline FixedOrderedMap(Slice<Entry> array) :
        mEntries(&array[0]), mNumberOfEntries(array.getNumberOfElements())
    {
    }

    /**
     * Get the number of entries.
     *
     * \return  Number of entries.
     */
    inline size_t
    getNumberOfElements() const
    {
        return mNumberOfEntries;
    }

    /**
     * Find a entry.
     *
     * Uses a binary search over the list of entries.
     *
     * \param key
     *      Key identifying the entry.
     * \return
     *      Pointer to the entry or 0 if no entry with the
     *      requested key is found.
     */
    const Entry*
    getEntry(Key key) const;

    inline Entry*
    getEntry(Key key)
    {
        // Reuse the const version of this function to avoid code duplication.
        return const_cast<Entry*>(static_cast<const FixedOrderedMap*>(this)->getEntry(key));
    }

    /**
     * Convert to a bounded array.
     */
    inline Slice<Entry>
    asArray() const
    {
        return outpost::Slice<Entry>::unsafe(mEntries, mNumberOfEntries);
    }

    inline Iterator
    begin()
    {
        return Iterator(&mEntries[0]);
    }

    inline Iterator
    end()
    {
        return Iterator(&mEntries[mNumberOfEntries]);
    }

    inline ConstIterator
    begin() const
    {
        return ConstIterator(&mEntries[0]);
    }

    inline ConstIterator
    end() const
    {
        return ConstIterator(&mEntries[mNumberOfEntries]);
    }

private:
    Entry* const mEntries;
    const size_t mNumberOfEntries;
};

}  // namespace outpost

#include "fixed_ordered_map_impl.h"

#endif
