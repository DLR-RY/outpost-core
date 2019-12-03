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

#ifndef OUTPOST_COMM_RMAP_TRANSACTION_H_
#define OUTPOST_COMM_RMAP_TRANSACTION_H_

#include "rmap_packet.h"

#include <outpost/rtos.h>
#include <outpost/time/duration.h>
#include <outpost/utils/container/shared_buffer.h>

namespace outpost
{
namespace comm
{
/**
 * RMAP transaction.
 *
 * Provides RMAP transaction level information. The class contains the command
 * and reply packets to be used by RMAP initiator.
 *
 * \author  Muhammad Bassam
 */
class RmapTransaction
{
public:
    enum State : uint8_t
    {
        notInitiated = 0x00,
        initiated = 0x01,
        commandSent = 0x02,
        replyReceived = 0x03,
        timeout = 0x04
    };

    RmapTransaction();
    ~RmapTransaction();

    /**
     * Reset or clear the contents of the transaction.
     *
     * This method is being used by the RMAP initiator for clearing the
     * completed transaction placeholder for future access.
     *
     * */
    void
    reset();

    //-------------------------------------------------------------------------
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
    setTimeoutDuration(outpost::time::Duration timeoutDuration)
    {
        mTimeoutDuration = timeoutDuration;
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
    setReplyPacket(RmapPacket* replyPacket)
    {
        mReplyPacket = *replyPacket;
    }

    inline void
    setBuffer(outpost::utils::SharedBufferPointer& buffer)
    {
        mBuffer = buffer;
    }

    /**
     * Blocks the current thread holding initiating the transaction.
     *
     * A binary semaphore is being acquired to lock the task.
     *
     * \param timeout
     *      Duration for which the thread should be blocked
     *
     * \return
     *      \retval true
     *         For successfully obtaining the lock
     *      \retval false
     *         For failure in obtaining the lock
     * */
    inline bool
    blockTransaction(outpost::time::Duration timeoutDuration)
    {
        return mReplyLock.acquire(timeoutDuration);
    }

    /**
     * Release the lock which has been acquired to lock the transaction.
     *
     * */
    inline void
    releaseTransaction()
    {
        mReplyLock.release();
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

    // Disabling copy constructor
    RmapTransaction(const RmapTransaction&) = delete;

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
    outpost::utils::SharedBufferPointer mBuffer;
};
}  // namespace comm
}  // namespace outpost

#endif
