/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
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

#ifndef COBC_UTILS_FIXED_ORDERED_MAP_H
#define COBC_UTILS_FIXED_ORDERED_MAP_H

#include <stdint.h>
#include <stddef.h>

#include "bounded_array.h"

namespace cobc
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
    /**
     * Create list from an array without explicit size.
     *
     * \param entries
     *      C style array of entries.
     */
    template <size_t N>
    explicit inline
    FixedOrderedMap(Entry (&entries)[N]) :
        mEntries(entries),
        mNumberOfEntries(N)
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
    FixedOrderedMap(Entry* entries,
                    size_t numberOfEntries);

    explicit inline
    FixedOrderedMap(BoundedArray<Entry> array) :
        mEntries(&array[0]),
        mNumberOfEntries(array.getNumberOfElements())
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
    Entry*
    getEntry(Key key);

    /**
     * Convert to a bounded array.
     */
    inline BoundedArray<Entry>
    asArray() const
    {
        return cobc::BoundedArray<Entry>(mEntries, mNumberOfEntries);
    }

private:
    Entry* const mEntries;
    const size_t mNumberOfEntries;
};

}

#include "fixed_ordered_map_impl.h"

#endif // COBC_UTILS_FIXED_ORDERED_MAP_H
