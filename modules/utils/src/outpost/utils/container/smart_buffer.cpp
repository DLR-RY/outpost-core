/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Jan-Gerd Mess (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "smart_buffer.h"

namespace outpost
{
namespace utils
{
outpost::rtos::Mutex SmartBuffer::mMutex;

SmartBuffer::SmartBuffer() : mReferenceCounter(0), mBuffer(0), mNumElements(0)
{
}

SmartBuffer::~SmartBuffer()
{
}

void
SmartBuffer::incrementCount()
{
    SmartBuffer::incrementCountAtomic(mReferenceCounter);
}

void
SmartBuffer::decrementCount()
{
    SmartBuffer::decrementCountAtomic(mReferenceCounter);
}

bool
SmartBufferPointer::getChild(ChildSmartPointer& ptr,
                             uint16_t type,
                             size_t pOffset,
                             size_t length) const
{
    bool res = false;
    if (isValid())
    {
        if (mOffset + pOffset + length <= mPtr->mNumElements && length > 0)
        {
            ptr = ChildSmartPointer(mPtr, *this);
            ptr.mType = type;
            ptr.mOffset = mOffset + pOffset;
            ptr.mLength = length;
            res = true;
        }
    }
    return res;
}
}  // namespace utils
}  // namespace outpost
