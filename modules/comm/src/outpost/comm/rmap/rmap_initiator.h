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

#ifndef OUTPOST_COMM_RMAP_INITIATOR_H_
#define OUTPOST_COMM_RMAP_INITIATOR_H_

#include <outpost/rtos.h>
#include <outpost/time/duration.h>
#include <outpost/smpc.h>
#include <outpost/hal/spacewire.h>

#include "rmap_packet.h"
#include "rmap_transaction.h"
#include "rmap_status.h"

namespace outpost
{
namespace comm
{
class RmapInitiator : public outpost::rtos::Thread
{
    friend class TestingRmap;

public:
    static uint16_t transactionId;

    enum Operation
    {
        OPERATION_READ = 0x01, OPERATION_WRITE = 0x02
    };

    struct ErrorCounters
    {
        ErrorCounters() :
                mDiscardedReceivedPackets(0), mErrorneousReplyPackets(0),
                mErrorInRmapReplyPacketProcessing(0)
        {

        }

        size_t mDiscardedReceivedPackets;
        size_t mErrorneousReplyPackets;
        size_t mErrorInRmapReplyPacketProcessing;
    };

    struct Buffer
    {
        Buffer() :
                mLength(0)
        {
            memset(mData, 0, sizeof(mData));
        }

        bool
        addData(uint8_t *buffer, uint16_t len)
        {
            bool result = false;

            if (len < sizeof(mData))
            {
                mLength = len;
                memcpy(mData, buffer, mLength);
                result = true;
            }
            return result;
        }

        void
        getData(uint8_t *buffer)
        {
            memcpy(buffer, mData, mLength);
            memset(mData, 0, sizeof(mData));
            mLength = 0;
        }

    private:
        uint8_t mData[1024];
        uint16_t mLength;
    };

    struct TransactionsList
    {

        TransactionsList() :
                mTransactions(), mIndex(0)
        {
        }

        ~TransactionsList()
        {

        }

        uint8_t
        getActiveTransactions()
        {
            uint8_t active = 0;

            for (uint8_t i = 0; i < maxConcurrentTransactions; i++)
            {
                if (mTransactions[i].getTransactionID() != 0)
                {
                    active++;
                }
            }
            return active;
        }

        void
        addTransaction(RmapTransaction *trans)
        {
            mTransactions[mIndex] = *trans;
            mIndex =
                    (mIndex == (maxConcurrentTransactions - 1) ? 0 : mIndex + 1);
        }

        void
        removeTransaction(uint16_t tid)
        {
            for (uint8_t i = 0; i < maxConcurrentTransactions; i++)
            {
                if (mTransactions[i].getTransactionID() == tid)
                {
                    mTransactions[i].reset();
                    break;
                }
            }
        }

        RmapTransaction*
        getTransaction(uint16_t tid)
        {
            bool found = false;
            uint8_t i;

            for (i = 0; i < maxConcurrentTransactions; i++)
            {
                if (mTransactions[i].getTransactionID() == tid)
                {
                    found = true;
                    break;
                }
            }

            if (found)
            {
                return &mTransactions[i];
            }
            else
            {
                return NULL;
            }
        }

        bool
        isTransactionIdUsed(uint16_t tid)
        {
            bool used = false;
            uint8_t i;

            for (i = 0; i < maxConcurrentTransactions; i++)
            {
                if (mTransactions[i].getTransactionID() == tid)
                {
                    used = true;
                    break;
                }
            }

            return used;
        }

        RmapTransaction*
        getFreeTransaction()
        {
            for (uint8_t i = 0; i < maxConcurrentTransactions; i++)
            {
                if (mTransactions[i].getState()
                        == RmapTransaction::NotInitiated)
                {
                    return &mTransactions[i];
                }
            }
            return NULL;
        }

        RmapTransaction mTransactions[maxConcurrentTransactions];
        uint8_t mIndex;
    };

    struct DiscardedPackets
    {
        DiscardedPackets() :
                mPackets(), mIndex(0)
        {
        }
        ~DiscardedPackets()
        {
        }

        void
        addDiscardedPacket(RmapPacket *packet)
        {
            mPackets[mIndex] = packet;
            mIndex =
                    (mIndex == (maxConcurrentTransactions - 1) ? 0 : mIndex + 1);
        }

        RmapPacket *mPackets[maxConcurrentTransactions];
        uint8_t mIndex;
    };

    RmapInitiator(hal::SpaceWire &spw, RmapTargetsList *list);
    ~RmapInitiator();

    /**
     * Writes remote memory. For blocking write, the method blocks the current
     * thread and waits for the desired reply until specific time interval. For
     * asynchronous write nonBlocking mode should be used.
     *
     * @param targetNodeName
     *      Name of the target which is commands, targets are preinitialized list
     *      and searched against the name provided
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param data
     *      Pointer to the data bytes
     *
     * @param length
     *      Length of the data bytes being written
     *
     * @param timeout
     *      Timeout in case of blocking transaction
     *
     * @return
     *      True in case of successful operations, false for any errors encountered
     */
    bool
    write(char *targetNodeName,
          uint32_t memoryAddress,
          uint8_t *data,
          uint32_t length,
          outpost::time::Duration timeout = outpost::time::Seconds(1));

    /**
     * Writes remote memory. For blocking write, the method blocks the current
     * thread and waits for the desired reply until specific time interval. For
     * asynchronous write nonBlocking mode should be used.
     *
     * @param targetNode
     *      Pointer to the target node object
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param data
     *      Pointer to the data bytes
     *
     * @param length
     *      Length of the data bytes being written
     *
     * @param timeout
     *      Timeout in case of blocking transaction
     *
     * @return
     *      True in case of successful operations, false for any errors encountered
     */
    bool
    write(RmapTargetNode *targetNode,
          uint32_t memoryAddress,
          uint8_t *data,
          uint32_t length,
          outpost::time::Duration timeout = outpost::time::Seconds(1));

    /**
     * Read from remote memory. The method blocks the current
     * thread and waits for the desired reply until specific time interval.
     *
     * @param targetNodeName
     *      Name of the target which is commands, targets are preinitialized list
     *      and searched against the name provided
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param data
     *      Pointer to the data bytes
     *
     * @param length
     *      Length of the data bytes being written
     *
     * @param timeout
     *      Timeout in case of blocking transaction
     *
     * @return
     *      True in case of successful operations, false for any errors encountered
     */
    bool
    read(char * targetNodeName,
         uint32_t memoryAddress,
         uint32_t length,
         uint8_t* buffer,
         outpost::time::Duration timeout = outpost::time::Milliseconds(1000));

    /**
     * Read from remote memory. The method blocks the current
     * thread and waits for the desired reply until specific time interval.
     *
     * @param targetNode
     *      Pointer to the target node object
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param data
     *      Pointer to the data bytes
     *
     * @param length
     *      Length of the data bytes being written
     *
     * @param timeout
     *      Timeout in case of blocking transaction
     *
     * @return
     *      True in case of successful operations, false for any errors encountered
     */

    bool
    read(RmapTargetNode* rmapTargetNode,
         uint32_t memoryAddress,
         uint32_t length,
         uint8_t *buffer,
         outpost::time::Duration timeout = outpost::time::Milliseconds(1000));

    inline bool
    isReplyModeSet() const
    {
        return mReplyMode;
    }

    inline void
    setReplyMode()
    {
        mReplyMode = true;
    }

    inline void
    unsetReplyMode()
    {
        mReplyMode = false;
    }

    inline bool
    isIncrementMode() const
    {
        return mIncrementMode;
    }

    inline void
    setIncrementMode()
    {
        mIncrementMode = true;
    }

    inline void
    unsetIncrementMode()
    {
        mIncrementMode = false;
    }

    inline bool
    isVerifyMode() const
    {
        return mVerifyMode;
    }

    inline void
    setVerifyMode()
    {
        mVerifyMode = true;
    }

    inline void
    unsetVerifyMode()
    {
        mVerifyMode = false;
    }

    inline void
    setInitiatorLogicalAddress(uint8_t initiatorLogicalAddress)
    {
        mInitiatorLogicalAddress = initiatorLogicalAddress;
    }

    size_t
    getActiveTransactions()
    {
        return mTransactionsList.getActiveTransactions();
    }

private:

    virtual void
    run();

    void
    stop()
    {
        if (mStopped == false)
        {
            mStopped = true;
        }
    }

    inline bool
    isStopped()
    {
        return mStopped;
    }

    inline bool
    isStarted()
    {
        return !mStopped;
    }

    bool
    sendPacket(RmapTransaction* transaction, uint8_t *data, uint16_t length);

    bool
    receivePacket(RmapPacket *rxedPacket);

    void
    replyPacketReceived(RmapPacket* packet);

    RmapTransaction*
    resolveTransaction(RmapPacket* packet);

    uint16_t
    getNextAvailableTransactionID();

    size_t
    getTotalDiscardedReplyPackets();

    outpost::BoundedArray<RmapPacket*>
    getDiscardedReplyPackets();

    hal::SpaceWire& mSpW;
    RmapTargetsList *mTargetNodes;
    outpost::rtos::Mutex mOperationLock;
    uint8_t mInitiatorLogicalAddress;
    bool mIncrementMode;
    bool mVerifyMode;
    bool mReplyMode;
    bool mStopped;
    TransactionsList mTransactionsList;
    uint16_t mLatestAssignedTransactionID;
    DiscardedPackets mDiscardedPackets;
    ErrorCounters mCounters;
    Buffer mRxData;
};

typedef outpost::BoundedArray<const uint8_t> NonRmapDataType;
extern outpost::smpc::Topic<NonRmapDataType> nonRmapPacketReceived;

}
}

#endif /* OUTPOST_COMM_RMAP_INITIATOR_H_ */
