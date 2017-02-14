/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
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

#ifndef COBC_UTILS_FIXED_ORDERED_MAP_IMPL_H
#define COBC_UTILS_FIXED_ORDERED_MAP_IMPL_H

#include "fixed_ordered_map.h"

template <typename Entry, typename Key>
outpost::FixedOrderedMap<Entry, Key>::FixedOrderedMap(Entry* entries,
                                                   size_t numberOfEntries) :
    mEntries(entries),
    mNumberOfEntries(numberOfEntries)
{
}


template <typename Entry, typename Key>
const Entry*
outpost::FixedOrderedMap<Entry, Key>::getEntry(Key key) const
{
    int imax = static_cast<int>(mNumberOfEntries) - 1;
    int imin = 0;

    Entry* entry = 0;
    while ((imax >= imin) && (entry == 0))
    {
        int mid = imin + ((imax - imin) / 2);

        if (mEntries[mid].mKey == key)
        {
            entry = &mEntries[mid];
        }
        else if (mEntries[mid].mKey < key)
        {
            imin = mid + 1;
        }
        else
        {
            imax = mid - 1;
        }
    }

    return entry;
}

#endif
