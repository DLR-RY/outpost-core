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

#include "parameter_store.h"

#include <algorithm>

namespace outpost
{
namespace parameter
{
OperationResult
ParameterStore::initialize(ParameterList& list)
{
    ParameterList* listArray[1];
    listArray[0] = &list;

    return initialize(outpost::asSlice(listArray));
}

OperationResult
ParameterStore::initialize(outpost::Slice<ParameterList*> lists)
{
    if (mInitialized)
    {
        return OperationResult::alreadyInitialized;
    }

    size_t count = 0;
    ParameterBase* pos = nullptr;
    for (unsigned int it = 0; it < lists.getNumberOfElements(); it++)
    {
        pos = lists[it]->anchor;
        while (nullptr != pos)
        {
            if (count >= mParameters.getNumberOfElements())
            {
                return OperationResult::tooManyElements;
            }

            if (pos->hasInvalidIdAssigned())
            {
                return OperationResult::invalidParameter;
            }
            else if (!pos->isInitialized())
            {
                return OperationResult::uninitializedParameter;
            }
            mParameters[count] = pos;
            pos = pos->getNext();
            count++;
        }
    }

    mCount = count;

    ParameterBase::sort(mParameters.first(mCount));

    for (size_t i = 1; i < mCount; i++)
    {
        if (!((*mParameters[i - 1]) < (*mParameters[i])))
        {
            return OperationResult::dublicatedID;
        }
    }

    mInitialized = true;

    return OperationResult::success;
}

OperationResult
ParameterStore::getParameter(IDType id, const ParameterBase*& store) const
{
    if (!mInitialized)
    {
        return OperationResult::notInitialized;
    }

    // sanity check
    if (mCount == 0)
    {
        return OperationResult::noSuchID;
    }

    if (id == ParameterBase::invalidID)
    {
        return OperationResult::invalidParameter;
    }

    auto pointer = ParameterBase::findInSorted(mParameters.first(mCount), id);

    if (nullptr == pointer)
    {
        return OperationResult::noSuchID;
    }

    store = pointer;
    return OperationResult::success;
}

ParameterStore::Iterator
ParameterStore::begin() const
{
    ParameterStore::Iterator it;
    it.mPointer = &(this->mParameters[0]);
    return it;
}

ParameterStore::Iterator
ParameterStore::end() const
{
    // mCount is 0 if ParameterStore is not initialized
    ParameterStore::Iterator it;
    it.mPointer = &(this->mParameters[mCount]);
    return it;
}

size_t
ParameterStore::getCount() const
{
    return mCount;
}

bool
ParameterStore::isInitialized() const
{
    return mInitialized;
}

}  // namespace parameter
}  // namespace outpost
