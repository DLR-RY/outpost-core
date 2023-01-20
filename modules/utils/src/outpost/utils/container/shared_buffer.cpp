/*
 * Copyright (c) 2017-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017-2018, Jan-Gerd Mess (DLR RY-AVS)
 * - 2018, Fabian Greif (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#include "shared_buffer.h"

namespace outpost
{
namespace utils
{
outpost::rtos::Mutex SharedBuffer::mMutex;

SharedBuffer::SharedBuffer() : mReferenceCounter(0), mBuffer(outpost::Slice<uint8_t>::empty())
{
}

SharedBuffer::~SharedBuffer()
{
}

void
SharedBuffer::incrementCount()
{
    SharedBuffer::incrementCountAtomic(mReferenceCounter);
}

void
SharedBuffer::decrementCount()
{
    SharedBuffer::decrementCountAtomic(mReferenceCounter);
}

bool
SharedBufferPointer::getChild(SharedChildPointer& ptr,
                              uint16_t type,
                              size_t pOffset,
                              size_t length) const
{
    bool res = false;
    if (isUsed())
    {
        if (((mOffset + pOffset + length) <= mOffset + mLength))
        {
            ptr = SharedChildPointer(mPtr, *this);
            ptr.mType = type;
            ptr.mOffset = mOffset + pOffset;
            ptr.mLength = length;
            res = true;
        }
    }
    return res;
}

bool
ConstSharedBufferPointer::getChild(ConstSharedChildPointer& ptr,
                                   uint16_t type,
                                   size_t pOffset,
                                   size_t length) const
{
    bool res = false;
    if (isUsed())
    {
        if (((mOffset + pOffset + length) <= mOffset + mLength))
        {
            ptr = ConstSharedChildPointer(mPtr, *this);
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
