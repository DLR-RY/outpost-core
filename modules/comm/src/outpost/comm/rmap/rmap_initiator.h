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
 * - 2017, Fabian Greif (DLR RY-AVS)
 * - 2018 - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_COMM_RMAP_INITIATOR_H_
#define OUTPOST_COMM_RMAP_INITIATOR_H_

#include "rmap_options.h"
#include "rmap_packet.h"
#include "rmap_result.h"
#include "rmap_status.h"
#include "rmap_transaction.h"

#include <outpost/hal/space_wire_multi_protocol_handler.h>
#include <outpost/rtos.h>
#include <outpost/smpc.h>
#include <outpost/support/heartbeat.h>
#include <outpost/time/duration.h>
#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_object_pool.h>
#include <outpost/utils/minmax.h>
#

#include <array>

namespace outpost
{
namespace comm
{
/**
 * RMAP initiator.
 *
 * This class is a RMAP front-end which provides the interface / access methods
 * to the user for sending and receiving RMAP packets to other SpW targets. The
 * only thing required by this class from the user is the information of the
 * RMAP target. This information is provided by the RmapTargetNode and it's list.
 * The list will be filled statically and stored as it's pointer.
 *
 * The reception of RMAP packet is handled by separate thread being supplied by
 * the initiator for any asynchronous incoming packets due to some delayed transport.
 *
 * \author  Muhammad Bassam
 */
class RmapInitiator : public outpost::rtos::Thread
{
    friend class TestingRmap;

    // For parameterize the class
    static constexpr outpost::time::Duration receiveTimeout = outpost::time::Seconds(5);

    // Packet length related constants.
    static constexpr uint16_t maxReadCommandLength =
            rmap::readCommandOverhead + (2 * rmap::maxAddressLength);
    static constexpr uint16_t maxWriteCommandLength =
            rmap::writeCommandOverhead + (2 * rmap::maxAddressLength) + rmap::bufferSize;

    static constexpr uint16_t maxCommandLength = maxWriteCommandLength;

    static constexpr uint16_t maxReadReplyLength =
            rmap::readReplyOverhead + rmap::maxAddressLength + rmap::bufferSize;
    static constexpr uint16_t maxWriteReplyLength =
            rmap::writeReplyOverhead + rmap::maxAddressLength;

    static constexpr uint16_t maxReplyLength = maxReadReplyLength;

    // Interval duration to check when the dispatcher thread is running
    static constexpr outpost::time::Duration startUpWaitInterval = outpost::time::Milliseconds(1);

public:
    struct ErrorCounters
    {
        ErrorCounters() :
            mUnknownTransactionID(0),
            mPackageCrcError(0),
            mIncorrectOperation(0),
            mInvalidSize(0),
            mOperationFailed(0),
            mSpacewireFailure(0)
        {
        }

        size_t mUnknownTransactionID;  // may also include already timed out replies
        size_t mPackageCrcError;       // data or header
        size_t mIncorrectOperation;    // the received answer does not fit the request
        size_t mInvalidSize;           // size does not fit
        size_t mOperationFailed;       // remote indicated failure
        size_t mSpacewireFailure;      // SpW failure
    };

    /**
     * Handles a list of  Transaction object,
     * list not thread save.
     */
    struct TransactionsList
    {
        TransactionsList() : mTransactions()
        {
        }

        ~TransactionsList()
        {
        }

        uint8_t
        getNumberOfActiveTransactions();

        void
        removeTransaction(uint16_t tid);

        RmapTransaction*
        getTransaction(uint16_t tid);

        bool
        isTransactionIdUsed(uint16_t tid);

        RmapTransaction*
        getFreeTransaction();

        RmapTransaction mTransactions[rmap::maxConcurrentTransactions];
    };

    //--------------------------------------------------------------------------
    RmapInitiator(hal::SpaceWireMultiProtocolHandlerInterface& spw,
                  RmapTargetsList* list,
                  uint8_t priority,
                  size_t stackSize,
                  outpost::support::parameter::HeartbeatSource heartbeatSource,
                  uint8_t initiatorLogicalAddress = rmap::defaultLogicalAddress);
    ~RmapInitiator();

    /**
     * Starts the internal thread and waits till the listeners are set up
     */
    void
    init(void);

    /**
     * Writes remote memory. For blocking write, the method blocks the current
     * thread and waits for the desired reply until specific time interval. For
     * asynchronous write non blocking mode should be used, which can be done by
     * providing zero timeout in the parameter and the command shall not expect
     * reply.
     *
     * @param targetNodeName
     *      Name of the target which is commands, targets are preinitialized list
     *      and searched against the name provided
     *
     * @param options
     *      contains the options with which the command is executed
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param extendedMemoryAddress
     *      The MSB of the (40Bit) remote memory address
     *
     * @param data
     *      A Slice containing the data to write
     *
     * @param timeout
     *      Timeout in case of blocking transaction, otherwise use
     *      outpost::time::Duration::zero()
     *
     * @return
     *      Description of the result, will implicitly cast to true in success case and false
     * otherwise
     */
    RmapResult
    write(const char* targetNodeName,
          const RMapOptions& options,
          uint32_t memoryAddress,
          uint8_t extendedMemoryAdress,
          outpost::Slice<const uint8_t> const& data,
          const outpost::time::Duration& timeout = outpost::time::Seconds(1));

    /**
     * Writes remote memory. For blocking write, the method blocks the current
     * thread and waits for the desired reply until specific time interval. For
     * asynchronous write non blocking mode should be used, which can be done by
     * providing zero timeout in the parameter and the command shall not expect
     * reply.
     *
     * @param targetNode
     *      Reference to the target node object found from the list
     *
     * @param options
     *      contains the options with which the command is executed
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param extendedMemoryAddress
     *      The MSB of the (40Bit) remote memory address
     *
     * @param data
     *      A Slice containing the data to write
     *
     * @param timeout
     *      Timeout in case of blocking transaction, otherwise use
     *      outpost::time::Duration::zero()
     *
     * @return
     *      Description of the result, will implicitly cast to true in success case and false
     * otherwise
     */
    RmapResult
    write(RmapTargetNode& targetNode,
          const RMapOptions& options,
          uint32_t memoryAddress,
          uint8_t extendedMemoryAdress,
          outpost::Slice<const uint8_t> const& data,
          const outpost::time::Duration& timeout = outpost::time::Seconds(1));

    /**
     * Read from remote memory. The method blocks the current
     * thread and waits for the desired reply until specific time interval.
     *
     * @param targetNodeName
     *      Name of the target which is commands, targets are preinitialized list
     *      and searched against the name provided
     *
     * @param options
     *      contains the options with which the command is executed
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param extendedMemoryAddress
     *      The MSB of the (40Bit) remote memory address
     *
     * @param buffer
     *      A Slice where received data bytes will be stored
     *
     * @param timeout
     *      Timeout for the SpW read operation
     *
     * @return
     *      Description of the result, will implicitly cast to true in success case and false
     * otherwise
     */
    RmapResult
    read(const char* targetNodeName,
         const RMapOptions& options,
         uint32_t memoryAddress,
         uint8_t extendedMemoryAdress,
         outpost::Slice<uint8_t> const& buffer,
         const outpost::time::Duration& timeout =
                 std::numeric_limits<outpost::time::Duration>::max());

    /**
     * Read from remote memory. The method blocks the current
     * thread and waits for the desired reply until specific time interval.
     *
     * @param targetNode
     *      Reference to the target node object found from the list
     *
     * @param options
     *      contains the options with which the command is executed
     *
     * @param memoryAddress
     *      Actual remote memory address where the data is being written
     *
     * @param extendedMemoryAddress
     *      The MSB of the (40Bit) remote memory address
     *
     * @param buffer
     *      A Slice where received data bytes will be stored
     *
     * @param timeout
     *      Timeout for the SpW read operation
     *
     * @return
     *      Description of the result, will implicitly cast to true in success case and false
     * otherwise
     */

    RmapResult
    read(RmapTargetNode& rmapTargetNode,
         const RMapOptions& options,
         uint32_t memoryAddress,
         uint8_t extendedMemoryAdress,
         outpost::Slice<uint8_t> const& buffer,
         const outpost::time::Duration& timeout =
                 std::numeric_limits<outpost::time::Duration>::max());

    //--------------------------------------------------------------------------

    inline size_t
    getActiveTransactions()
    {
        return mTransactionsList.getNumberOfActiveTransactions();
    }

    inline ErrorCounters
    getErrorCounters() const
    {
        return mCounters;
    }

    inline void
    resetErrorCounters()
    {
        mCounters = ErrorCounters();
    }

private:
    virtual void
    run() override;

    /**
     * does a single step of the receive loop, called continously by receiver thread,
     * for testing needed as own function
     */
    void
    doSingleStep();

    void
    stop()
    {
        if (mStopped == false)
        {
            mStopped = true;
        }
    }

    inline bool
    isStopped() const
    {
        return mStopped;
    }

    inline bool
    isStarted() const
    {
        return !mStopped;
    }

    bool
    sendPacket(RmapTransaction* transaction);

    bool
    receivePacket(RmapPacket* rxedPacket, outpost::hal::SpWMessage& rx);

    void
    handleReplyPacket(RmapPacket* packet, const outpost::hal::SpWMessage& rx);

    RmapTransaction*
    resolveTransaction(RmapPacket* packet);

    /**
     * Returns a free Transaction ID,
     * requires mOperationLock to be lock to current thread
     * to prevent race conditions
     */
    uint16_t
    getNextAvailableTransactionID();

    //--------------------------------------------------------------------------
    hal::SpaceWireMultiProtocolHandlerInterface& mSpW;
    RmapTargetsList* mTargetNodes;
    outpost::rtos::Mutex mOperationLock;
    const uint8_t mInitiatorLogicalAddress;
    volatile bool mStopped;
    uint16_t mTransactionId;
    TransactionsList mTransactionsList;

    ErrorCounters mCounters;

    const outpost::support::parameter::HeartbeatSource mHeartbeatSource;

    std::array<uint8_t, maxCommandLength> mSendBuffer;

    outpost::hal::SpWChannel<rmap::numberOfReceiveBuffers> mChannel;
};

}  // namespace comm
}  // namespace outpost

#endif /* OUTPOST_COMM_RMAP_INITIATOR_H_ */
