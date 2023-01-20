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

#ifndef OUTPOST_PARAMETER_PARAMETER_IMPL_H_
#define OUTPOST_PARAMETER_PARAMETER_IMPL_H_

#include "parameter.h"

namespace outpost
{
namespace parameter
{
template <uint16_t index>
ParameterList IndexedParameterList<index>::list;

template <typename T>
Parameter<T>::Parameter(IDType id,
                        const T& initialValue,
                        const outpost::time::SpacecraftElapsedTime& time,
                        ParameterList& list,
                        bool hasMultipleWriters) :
    ParameterBase(list)
{
    if (id == invalidID)
    {
        mID = invalidID;
        mMultipleWriters = false;
        mCurrentElementIsFirst = true;
        mAssignedIdInvalid = true;
    }
    else
    {
        mID = id;

        mMultipleWriters = hasMultipleWriters;

        mElements[0].data = initialValue;
        mElements[0].changeTime = time;

        mCurrentElementIsFirst = true;

        mAssignedIdInvalid = false;
    }
}

template <typename T>
OperationResult
Parameter<T>::initialize(IDType id,
                         const T& initialValue,
                         const outpost::time::SpacecraftElapsedTime& time,
                         bool hasMultipleWriters)
{
    if (mID != invalidID)
    {
        return OperationResult::alreadyInitialized;
    }

    if (id == invalidID)
    {
        mAssignedIdInvalid = true;
        return OperationResult::invalidParameter;
    }

    mID = id;

    mMultipleWriters = hasMultipleWriters;

    mElements[0].data = initialValue;
    mElements[0].changeTime = time;

    mCurrentElementIsFirst = true;
    mAssignedIdInvalid = false;
    return OperationResult::success;
}

template <typename T>
OperationResult
Parameter<T>::getValue(T& store, outpost::time::SpacecraftElapsedTime* time) const
{
    if (mID == invalidID)
    {
        return OperationResult::notInitialized;
    }
    Element tmp;
    bool before;
    bool after;
    unsigned int Try = 0;
    do
    {
        before = mCurrentElementIsFirst;
        if (before)
        {
            tmp = mElements[0];
        }
        else
        {
            tmp = mElements[1];
        }

        after = mCurrentElementIsFirst;
        Try++;
    } while ((before != after)
             && (Try < maxReadTries));  // Fails if during read a write
                                        // occurred, each try needs its own write to fail

    if (before != after)
    {
        return OperationResult::tooManyConcurrentWrites;
    }

    store = tmp.data;
    if (nullptr != time)
    {
        *time = tmp.changeTime;
    }

    return OperationResult::success;
}

template <typename T>
OperationResult
Parameter<T>::setValue(const T& data, const outpost::time::SpacecraftElapsedTime& time)
{
    if (mID == invalidID)
    {
        return OperationResult::notInitialized;
    }

    if (!mMultipleWriters)
    {
        if (mCurrentElementIsFirst)
        {
            mElements[1].changeTime = time;
            mElements[1].data = data;
            mCurrentElementIsFirst = false;
        }
        else
        {
            mElements[0].changeTime = time;
            mElements[0].data = data;
            mCurrentElementIsFirst = true;
        }

        return OperationResult::success;
    }
    else
    {
        if (!mWriteInProgress.test_and_set())
        {
            if (mCurrentElementIsFirst)
            {
                mElements[1].changeTime = time;
                mElements[1].data = data;
                mCurrentElementIsFirst = false;
            }
            else
            {
                mElements[0].changeTime = time;
                mElements[0].data = data;
                mCurrentElementIsFirst = true;
            }

            mWriteInProgress.clear();
            return OperationResult::success;
        }
        else
        {
            return OperationResult::concurrentWrite;
        }
    }
}

}  // namespace parameter
}  // namespace outpost

#endif
