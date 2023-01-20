/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_FIXED_ORDERED_MAP_IMPL_H
#define OUTPOST_UTILS_FIXED_ORDERED_MAP_IMPL_H

#include "fixed_ordered_map.h"

template <typename Entry, typename Key>
outpost::FixedOrderedMap<Entry, Key>::FixedOrderedMap(Entry* entries, size_t numberOfEntries) :
    mEntries(entries), mNumberOfEntries(numberOfEntries)
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
