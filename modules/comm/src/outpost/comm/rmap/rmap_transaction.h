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
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------
#ifndef OUTPOST_COMM_RMAP_TRANSACTION_H_
#define OUTPOST_COMM_RMAP_TRANSACTION_H_

#include <outpost/rtos.h>
#include <outpost/time/duration.h>

#include "rmap_packet.h"

namespace outpost
{
namespace comm
{
class RmapTransaction
{
public:
    enum State : uint8_t
    {
        NotInitiated = 0x00,
        Initiated = 0x01,
        CommandSent = 0x02,
        ReplyReceived = 0x03,
        Timeout = 0x04,
    };

    RmapTransaction() :
            mTargetLogicalAddress(0), mInitiatorLogicalAddress(0),
            mTransactionID(0), mTimeoutDuration(outpost::time::Milliseconds(0)),
            mState(NotInitiated), mBlockingMode(false), mReplyPacket(),
            mCommandPacket(),
            mReplyLock(outpost::rtos::BinarySemaphore::State::acquired)
    {
    }
    ~RmapTransaction()
    {

    }

    inline void
    setInitiatorLogicalAddress(uint8_t addr)
    {
        mInitiatorLogicalAddress = addr;
    }

    inline uint8_t
    getInitiatorLogicalAddress() const
    {
        return mInitiatorLogicalAddress;
    }

    inline void
    setTargetLogicalAddress(uint8_t addr)
    {
        mTargetLogicalAddress = addr;
    }

    inline uint8_t
    getTargetLogicalAddress() const
    {
        return mTargetLogicalAddress;
    }

    inline void
    setState(State state)
    {
        mState = state;
    }

    inline uint8_t
    getState() const
    {
        return mState;
    }

    inline void
    setBlockingMode(bool state)
    {
        mBlockingMode = state;
    }

    inline bool
    isBlockingMode() const
    {
        return mBlockingMode;
    }

    inline void
    setTimeoutDuration(outpost::time::Duration timeout)
    {
        mTimeoutDuration = timeout;
    }

    inline outpost::time::Duration
    getTimeoutDuration() const
    {
        return mTimeoutDuration;
    }

    inline void
    setTransactionID(uint16_t tid)
    {
        mTransactionID = tid;
    }

    inline uint16_t
    getTransactionID() const
    {
        return mTransactionID;
    }

    inline RmapPacket*
    getCommandPacket()
    {
        return &mCommandPacket;
    }

    inline RmapPacket*
    getReplyPacket()
    {
        return &mReplyPacket;
    }

    inline void
    setReplyPacket(RmapPacket *replyPacket)
    {
        mReplyPacket = *replyPacket;
    }

    void
    blockTransaction(outpost::time::Duration timeout)
    {
        if (timeout == outpost::time::Duration::maximum())
        {
            mReplyLock.acquire();
        }
        else
        {
            mReplyLock.acquire(timeout);
        }
    }

    inline void
    releaseTransaction()
    {
        mReplyLock.release();
    }

    void
    reset()
    {
        mTargetLogicalAddress = 0;
        mInitiatorLogicalAddress = 0;
        mTransactionID = 0;
        mTimeoutDuration = outpost::time::Duration::zero();
        mState = NotInitiated;

        mBlockingMode = false;
        mCommandPacket.reset();
        mReplyPacket.reset();
        //mReplyLock.release();
    }

    inline RmapTransaction&
    operator=(const RmapTransaction& rhs)
    {
        mTargetLogicalAddress = rhs.mTargetLogicalAddress;
        mInitiatorLogicalAddress = rhs.mInitiatorLogicalAddress;
        mTransactionID = rhs.mTransactionID;
        mTimeoutDuration = rhs.mTimeoutDuration;
        mState = rhs.mState;

        mBlockingMode = rhs.mBlockingMode;
        mCommandPacket = rhs.mCommandPacket;
        mReplyPacket = rhs.mReplyPacket;

        return *this;
    }

private:
    uint8_t mTargetLogicalAddress;
    uint8_t mInitiatorLogicalAddress;
    uint16_t mTransactionID;
    outpost::time::Duration mTimeoutDuration;
    State mState;

    bool mBlockingMode;
    RmapPacket mReplyPacket;
    RmapPacket mCommandPacket;
    outpost::rtos::BinarySemaphore mReplyLock;
};
}
}

#endif
