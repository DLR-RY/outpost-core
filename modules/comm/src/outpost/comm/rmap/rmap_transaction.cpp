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
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 */

#include "rmap_transaction.h"

using namespace outpost::comm;

RmapTransaction::RmapTransaction() :
    mTargetLogicalAddress(0),
    mInitiatorLogicalAddress(0),
    mTransactionID(0),
    mTimeoutDuration(outpost::time::Duration::zero()),
    mState(State::notInitiated),
    mBlockingMode(false),
    mReplyPacket(),
    mCommandPacket(),
    mReplyLock(outpost::rtos::BinarySemaphore::State::released),
    mBuffer()
{
}

RmapTransaction::~RmapTransaction()
{
}

void
RmapTransaction::reset()
{
    mTargetLogicalAddress = 0;
    mInitiatorLogicalAddress = 0;
    mTransactionID = 0;
    mTimeoutDuration = outpost::time::Duration::zero();
    mState = State::notInitiated;
    mBlockingMode = false;
    mReplyPacket.reset();
    mCommandPacket.reset();
    mBuffer = outpost::hal::SpWMessage();
}
