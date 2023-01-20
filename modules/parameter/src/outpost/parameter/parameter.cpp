/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include "parameter.h"

#include <algorithm>

namespace outpost
{
namespace parameter
{
constexpr IDType ParameterBase::invalidID;

bool
ParameterBase::operator<(const ParameterBase& other) const
{
    return mID < other.mID;
}

IDType
ParameterBase::getID() const
{
    return mID;
}

void
ParameterBase::activateMultipleWritersSupport()
{
    mMultipleWriters = true;
}

bool
ParameterBase::isInitialized() const
{
    return mID != invalidID;
}

bool
ParameterBase::hasInvalidIdAssigned() const
{
    return mAssignedIdInvalid;
}

ParameterBase*
ParameterBase::findInSorted(outpost::Slice<ParameterBase*> slice, IDType id)
{
    outpost::Slice<const ParameterBase*> tmp = outpost::Slice<const ParameterBase*>::unsafe(
            const_cast<const ParameterBase**>(&slice[0]), slice.getNumberOfElements());
    int pos = findInternal(tmp, id);
    if (pos == -1)
    {
        return nullptr;
    }
    else
    {
        return slice[pos];
    }
}

const ParameterBase*
ParameterBase::findInSorted(outpost::Slice<const ParameterBase*> slice, IDType id)
{
    int pos = findInternal(slice, id);
    if (pos == -1)
    {
        return nullptr;
    }
    else
    {
        return slice[pos];
    }
}

int
ParameterBase::findInternal(outpost::Slice<const ParameterBase*> slice, IDType id)
{
    int l = 0;
    int r = slice.getNumberOfElements() - 1;
    size_t index = slice.getNumberOfElements();

    // Own binary sear as std::bin_search does require the search element of same type
    while (l <= r && index == slice.getNumberOfElements())
    {
        int m = l + (r - l) / 2;

        if ((slice[m])->mID == id)
        {
            index = m;
        }
        else if ((slice[m])->mID < id)
        {
            l = m + 1;
        }
        else
        {
            r = m - 1;
        }
    }

    if (index == slice.getNumberOfElements())
    {
        return -1;
    }
    else
    {
        return index;
    }
}

void
ParameterBase::sort(outpost::Slice<ParameterBase*> slice)
{
    if (slice.getNumberOfElements() > 1)
    {
        std::sort(&slice[0], &slice[slice.getNumberOfElements()], comparePointedParameter);
    }
}

void
ParameterBase::sort(outpost::Slice<const ParameterBase*> slice)
{
    if (slice.getNumberOfElements() > 1)
    {
        std::sort(&slice[0], &slice[slice.getNumberOfElements()], comparePointedParameter);
    }
}

}  // namespace parameter
}  // namespace outpost
