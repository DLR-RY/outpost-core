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
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#include "rmap_initiator.h"

#include "rmap_common.h"

#include <cinttypes>

using namespace outpost::comm;

constexpr outpost::time::Duration RmapInitiator::receiveTimeout;
constexpr outpost::time::Duration RmapInitiator::startUpWaitInterval;

uint8_t
RmapInitiator::TransactionsList::getNumberOfActiveTransactions()
{
    uint8_t active = 0;

    for (uint8_t i = 0; i < rmap::maxConcurrentTransactions; i++)
    {
        if (mTransactions[i].getState() != RmapTransaction::State::notInitiated)
        {
            active++;
        }
    }
    return active;
}

void
RmapInitiator::TransactionsList::removeTransaction(uint16_t tid)
{
    for (uint8_t i = 0; i < rmap::maxConcurrentTransactions; i++)
    {
        if (mTransactions[i].getTransactionID() == tid)
        {
            mTransactions[i].reset();
            break;
        }
    }
}

RmapTransaction*
RmapInitiator::TransactionsList::getTransaction(uint16_t tid)
{
    bool found = false;
    uint8_t i;

    for (i = 0; i < rmap::maxConcurrentTransactions; i++)
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
        return nullptr;
    }
}

bool
RmapInitiator::TransactionsList::isTransactionIdUsed(uint16_t tid)
{
    bool used = false;

    for (uint8_t i = 0; i < rmap::maxConcurrentTransactions; i++)
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
RmapInitiator::TransactionsList::getFreeTransaction()
{
    for (uint8_t i = 0; i < rmap::maxConcurrentTransactions; i++)
    {
        if (mTransactions[i].getState() == RmapTransaction::State::notInitiated)
        {
            mTransactions[i].setState(RmapTransaction::State::reserved);
            return &mTransactions[i];
        }
    }
    return nullptr;
}

//-----------------------------------------------------------------------------
RmapInitiator::RmapInitiator(hal::SpaceWireMultiProtocolHandlerInterface& spw,
                             RmapTargetsList* list,
                             uint8_t priority,
                             size_t stackSize,
                             outpost::support::parameter::HeartbeatSource heartbeatSource,
                             uint8_t initiatorLogicalAddress) :
    outpost::rtos::Thread(priority, stackSize, "RMEN"),
    mSpW(spw),
    mTargetNodes(list),
    mOperationLock(),
    mInitiatorLogicalAddress(initiatorLogicalAddress),
    mStopped(true),
    mTransactionId(0),
    mTransactionsList(),
    mCounters(),
    mHeartbeatSource(heartbeatSource),
    mChannel()
{
    mChannel.getFilter().setProtocol(rmap::protocolIdentifier);
    mChannel.getFilter().setAllowPartial(false);
    mChannel.getFilter().setMaxSize(maxReplyLength);
}

RmapInitiator::~RmapInitiator()
{
}

/**
 * Starts the internal thread and waits till the listeners are set up
 */
void
RmapInitiator::init(void)
{
    // setup the receive part
    mSpW.registerChannel(mChannel);

    start();
}

RmapResult
RmapInitiator::write(const char* targetNodeName,
                     const RMapOptions& options,
                     uint32_t memoryAddress,
                     uint8_t extendedMemoryAdress,
                     outpost::Slice<const uint8_t> const& data,
                     const outpost::time::Duration& timeout)
{
    RmapResult result;
    result.mResult = RmapResult::Code::invalidParameters;
    if (nullptr != mTargetNodes)
    {
        RmapTargetNode* targetNode = mTargetNodes->getTargetNode(targetNodeName);
        if (targetNode)
        {
            // Exit if trying to send zero length
            if (data.getNumberOfElements() != 0)
            {
                result = write(
                        *targetNode, options, memoryAddress, extendedMemoryAdress, data, timeout);
            }
        }
    }
    return result;
}

RmapResult
RmapInitiator::write(RmapTargetNode& targetNode,
                     const RMapOptions& options,
                     uint32_t memoryAddress,
                     uint8_t extendedMemoryAdress,
                     outpost::Slice<const uint8_t> const& data,
                     const outpost::time::Duration& timeout)
{
    RmapResult result;
    if (data.getNumberOfElements() == 0)
    {
        // the second write function also return without a message in this case
        result.mResult = RmapResult::Code::invalidParameters;
        return result;
    }

    RmapTransaction* transaction = nullptr;

    bool sendSuccesful = false;

    // Scope for the MutexGuard, protecting the transaction setup
    {
        // Guard operation against concurrent accesses
        if (!mOperationLock.acquire(timeout))
        {
            console_out("RMAP-Initiator: Transaction timeout\n");
            result.mResult = RmapResult::Code::lockingError;
            return result;
        }

        transaction = mTransactionsList.getFreeTransaction();

        if (nullptr == transaction)
        {
            console_out("RMAP-Initiator: All transactions are in use\n");
            result.mResult = RmapResult::Code::noFreeTransactions;
            mOperationLock.release();
            return result;
        }
        // id must also be under mutex
        uint16_t transactionID = getNextAvailableTransactionID();
        transaction->setTransactionID(transactionID);
        mOperationLock.release();
    }  // MutexGuard scope

    RmapPacket* cmd = transaction->getCommandPacket();

    // Packet configuration
    cmd->setInitiatorLogicalAddress(mInitiatorLogicalAddress);
    cmd->setWrite();
    cmd->setCommand();

    cmd->setIncrementFlag(options.mIncrementMode);
    cmd->setVerifyFlag(options.mVerifyMode);
    cmd->setReplyFlag(options.mReplyMode);

    if (options.mReplyMode)
    {
        // Sets the transaction mode
        transaction->setBlockingMode(true);

        // Extra block call with zero timeout for acquiring already released lock
        transaction->blockTransaction(outpost::time::Duration::zero());
    }
    else
    {
        // UnSets the transaction mode
        transaction->setBlockingMode(false);
    }
    cmd->setExtendedAddress(extendedMemoryAdress);
    cmd->setAddress(memoryAddress);
    cmd->setData(data);  // also set data length and crc
    cmd->setTargetInformation(targetNode);
    transaction->setTimeoutDuration(timeout);

    // this fixes a bug, where on the OM2 this function can hang indenfinitly.
    // could also be a bug in the SpWLite driver,
    outpost::rtos::Thread::sleep(outpost::time::Milliseconds(10));
    // Transaction will be initiated and sent through the SpW interface
    sendSuccesful = sendPacket(transaction);

    if (sendSuccesful)
    {
        // If reply is expected
        if (options.mReplyMode)
        {
            // Wait for the RMAP reply
            transaction->blockTransaction(timeout);

            RmapTransaction::State state = transaction->getState();

            // Command sent but no reply
            if (state == RmapTransaction::State::reserved)
            {
                console_out("RMAP-Initiator: command sent but no reply received for the "
                            "transaction %u\n",
                            transaction->getTransactionID());

                result.mResult = RmapResult::Code::timeout;
            }
            // Command sent and reply received
            else if (state == RmapTransaction::State::replyReceived)
            {
                RmapPacket* rply = transaction->getReplyPacket();

                result.mErrorCode =
                        static_cast<RmapReplyStatus::ErrorStatusCodes>(rply->getStatus());
                if (rply->getStatus() == RmapReplyStatus::commandExecutedSuccessfully)
                {
                    console_out("RMAP-Initiator: reply received with success\n");

                    result.mResult = RmapResult::Code::success;
                }
                else
                {
                    console_out("RMAP-Initiator: reply received with failure\n");

                    RmapReplyStatus::replyStatus(
                            static_cast<RmapReplyStatus::ErrorStatusCodes>(rply->getStatus()));

                    result.mResult = RmapResult::Code::executionFailed;
                    mCounters.mPackageCrcError++;
                }
            }
        }
        else
        {
            // Command was sent successfully, in non reply mode there is nothing more
            result.mResult = RmapResult::Code::success;
        }
    }
    else
    {
        // Command was not sent successfully
        console_out("RMAP-Initiator: transaction could not be initiated, failed to send command\n");
        result.mResult = RmapResult::Code::sendFailed;
        mCounters.mSpacewireFailure++;
    }

    // mutex guarded scope to guard the transaction tear down
    {
        // Guard operation against concurrent accesses
        outpost::rtos::MutexGuard lock(mOperationLock);
        // Delete the transaction from the list
        mTransactionsList.removeTransaction(transaction->getTransactionID());
    }

    return result;
}

RmapResult
RmapInitiator::read(const char* targetNodeName,
                    const RMapOptions& options,
                    uint32_t memoryAddress,
                    uint8_t extendedMemoryAdress,
                    outpost::Slice<uint8_t> const& buffer,
                    const outpost::time::Duration& timeout)
{
    RmapResult result;
    result.mResult = RmapResult::Code::invalidParameters;
    if (nullptr != mTargetNodes)
    {
        RmapTargetNode* targetNode = mTargetNodes->getTargetNode(targetNodeName);
        if (targetNode)
        {
            // Exit if trying to read zero length
            if (buffer.getNumberOfElements() != 0)
            {
                result = read(
                        *targetNode, options, memoryAddress, extendedMemoryAdress, buffer, timeout);
            }
        }
    }
    return result;
}

RmapResult
RmapInitiator::read(RmapTargetNode& rmapTargetNode,
                    const RMapOptions& options,
                    uint32_t memoryAddress,
                    uint8_t extendedMemoryAdress,
                    outpost::Slice<uint8_t> const& buffer,
                    const outpost::time::Duration& timeout)
{
    RmapResult result;
    if (buffer.getNumberOfElements() > rmap::bufferSize)
    {
        console_out("RMAP-Initiator: Requested size for read %lu, maximal allowed size %u\n",
                    buffer.getNumberOfElements(),
                    rmap::bufferSize);
        result.mResult = RmapResult::Code::invalidParameters;
        return result;
    }

    if (buffer.getNumberOfElements() == 0)
    {
        // the second read function also return without a message in this case
        result.mResult = RmapResult::Code::invalidParameters;
        return result;
    }

    RmapTransaction* transaction = nullptr;
    bool sendSuccesful = false;

    // Scope for the MutexGuard, protecting the transaction setup
    {
        // Guard operation against concurrent accesses
        if (!mOperationLock.acquire(timeout))
        {
            console_out("RMAP-Initiator: Transaction timeout\n");
            result.mResult = RmapResult::Code::lockingError;
            return result;
        }

        transaction = mTransactionsList.getFreeTransaction();

        if (nullptr == transaction)
        {
            console_out("RMAP-Initiator: All transactions are in use\n");
            result.mResult = RmapResult::Code::noFreeTransactions;
            mOperationLock.release();
            return result;
        }
        // id must also be under mutex
        uint16_t transactionID = getNextAvailableTransactionID();
        transaction->setTransactionID(transactionID);
        mOperationLock.release();
    }  // MutexGuard scope

    RmapPacket* cmd = transaction->getCommandPacket();

    // Read transaction will always be blocking
    transaction->setBlockingMode(true);

    // Extra block call with zero timeout for acquiring already released lock
    transaction->blockTransaction(outpost::time::Duration::zero());

    // Sets the command packet
    cmd->setInitiatorLogicalAddress(mInitiatorLogicalAddress);
    cmd->setRead();
    cmd->setCommand();

    cmd->setIncrementFlag(options.mIncrementMode);
    cmd->setVerifyFlag(options.mVerifyMode);
    cmd->setReplyFlag(true);

    cmd->setExtendedAddress(extendedMemoryAdress);
    cmd->setAddress(memoryAddress);
    cmd->setDataLength(buffer.getNumberOfElements());

    // InitiatorLogicalAddress might be updated in below
    cmd->setTargetInformation(rmapTargetNode);
    transaction->setInitiatorLogicalAddress(cmd->getInitiatorLogicalAddress());
    transaction->setTimeoutDuration(timeout);

    // this fixes a bug, where on the OM2 this function can hang indenfinitly.
    // could also be a bug in the SpWLite driver,
    outpost::rtos::Thread::sleep(outpost::time::Milliseconds(10));
    sendSuccesful = sendPacket(transaction);

    if (sendSuccesful)
    {
        console_out("RMAP-Initiator: Command sent %u, waiting for reply\n",
                    transaction->getState());

        // Wait for the RMAP reply
        transaction->blockTransaction(timeout);

        console_out("RMAP-Initiator: Notified with state: %u\n", transaction->getState());

        if (transaction->getState() == RmapTransaction::State::replyReceived)
        {
            RmapPacket* rply = transaction->getReplyPacket();
            uint8_t replyStatus = rply->getStatus();
            result.mErrorCode = static_cast<RmapReplyStatus::ErrorStatusCodes>(replyStatus);

            if (replyStatus != RmapReplyStatus::commandExecutedSuccessfully)
            {
                console_out("RMAP-Initiator: Command not executed successfully: %u\n", replyStatus);
                result.mResult = RmapResult::Code::executionFailed;
                mCounters.mOperationFailed++;
            }
            else
            {
                result.mReadbytes = rply->getDataLength();
                if (buffer.getNumberOfElements() < rply->getDataLength())
                {
                    console_out("RMAP-Initiator: Read reply with more data then requested\n");
                    result.mResult = RmapResult::Code::invalidReply;
                    mCounters.mIncorrectOperation++;
                }
                else if (buffer.getNumberOfElements() > rply->getDataLength())
                {
                    console_out("RMAP-Initiator: Read reply with insufficient data\n");

                    // Copy received data to the external buffer
                    memcpy(&buffer[0], &rply->getData()[0], rply->getDataLength());

                    result.mResult = RmapResult::Code::replyTooShort;
                }
                else
                {
                    // Copy received data to the external buffer
                    // at this point rply->getDataLength() == buffer.getNumberOfElements()
                    memcpy(&buffer[0], &rply->getData()[0], rply->getDataLength());

                    // Release the SpW buffer

                    result.mResult = RmapResult::Code::success;
                }
            }
        }
        else
        {
            console_out("RMAP-Initiator: Timeout\n");
            result.mResult = RmapResult::Code::timeout;
        }
    }
    else
    {
        console_out("RMAP-Initiator: Transaction could not be initiated\n");
        result.mResult = RmapResult::Code::sendFailed;
        mCounters.mSpacewireFailure++;
    }

    // mutex guarded scope to guard the transaction tear down
    {
        // Guard operation against concurrent accesses
        outpost::rtos::MutexGuard lock(mOperationLock);
        // Delete the transaction from the list
        // Will also release the SharedBuffer allocated for the received data
        mTransactionsList.removeTransaction(transaction->getTransactionID());
    }

    return result;
}

//=============================================================================

void
RmapInitiator::run()
{
    mStopped = false;
    while (!mStopped)
    {
        outpost::support::Heartbeat::send(mHeartbeatSource, receiveTimeout * 2);
        doSingleStep();
    }
    outpost::support::Heartbeat::suspend(mHeartbeatSource);
    mStopped = true;
}

/**
 * does a single step of the receive loop, called continuously by receiver thread,
 * for testing needed as own function
 */
void
RmapInitiator::doSingleStep()
{
    RmapPacket packet;

    outpost::hal::SpWMessage rxBuffer;

    if (receivePacket(&packet, rxBuffer))
    {
        // Only handling reply packet, no command packets
        if (packet.isReplyPacket())
        {
            handleReplyPacket(&packet, rxBuffer);
        }
    }
}

bool
RmapInitiator::sendPacket(RmapTransaction* transaction)
{
    RmapPacket* cmd = transaction->getCommandPacket();
    bool result = false;

    cmd->setTransactionID(transaction->getTransactionID());

    // It is assumed that the transaction is already in the list and in case of
    // required reply corresponding transaction will found and freed accordingly
    // therefore transmit can directly begin

    // Request TX buffer
    outpost::Slice<uint8_t> txBuffer = outpost::asSlice(mSendBuffer);

    // Serialize the packet content to the SpW buffer
    if (cmd->constructPacket(txBuffer))
    {
#ifdef DEBUG_EN
        console_out("TX-Data length: %zu\n", txBuffer.getNumberOfElements());
        for (uint16_t i = 0; i < txBuffer.getNumberOfElements(); i++)
        {
            console_out("%02X ", txBuffer[i]);
            if (i % 30 == 29)
            {
                console_out("\n");
            }
        }
        console_out("\n");
#endif
        if (mSpW.send(txBuffer, transaction->getTimeoutDuration()))
        {
            result = true;
        }
    }
    return result;
}

bool
RmapInitiator::receivePacket(RmapPacket* rxedPacket, outpost::hal::SpWMessage& rx)
{
    bool result = false;

    // Receive response
    if (mChannel.receiveMessage(rx, receiveTimeout) == outpost::swb::OperationResult::success)
    {
        outpost::Slice<const uint8_t> rxData = rx.buffer.asSlice();

#ifdef DEBUG_EN
        console_out("RX-Data length: %zu\n", rxData.getNumberOfElements());
        for (uint16_t i = 0; i < rxData.getNumberOfElements(); i++)
        {
            console_out("%02X ", rxData[i]);
            if (i % 30 == 29)
            {
                console_out("\n");
            }
        }
        console_out("\n");
#endif

        rxedPacket->reset();

        auto extractionResult = rxedPacket->extractReplyPacket(rxData, mInitiatorLogicalAddress);
        switch (extractionResult)
        {
            case RmapPacket::ExtractionResult::success: result = true; break;
            case RmapPacket::ExtractionResult::incorrectAddress: result = false; break;
            case RmapPacket::ExtractionResult::crcError:
                result = false;
                mCounters.mPackageCrcError++;
                break;
            case RmapPacket::ExtractionResult::invalid:
                result = false;
                mCounters.mInvalidSize++;
                break;
        }
    }
    return result;
}

void
RmapInitiator::handleReplyPacket(RmapPacket* packet, const outpost::hal::SpWMessage& rx)
{
    // Prevents that this function and removeTransaction both change the transaction
    // Which could lead to unusable transaction objects
    outpost::rtos::MutexGuard lock(mOperationLock);

    // Find a corresponding command packet
    RmapTransaction* transaction = resolveTransaction(packet);

    if (nullptr == transaction)
    {
        // If not found, increment error counter
        mCounters.mUnknownTransactionID++;
        console_out("RMAP Reply packet (dataLength %" PRIu32 " bytes) was received but "
                    "no corresponding transaction was found.\n",
                    packet->getDataLength());
    }
    else
    {
        // check correct answer
        if ((transaction->getCommandPacket()->getInstruction() & 0x3c)
            != (packet->getInstruction() & 0x3c))
        {
            console_out("RMAP-Initiator: Received reply does not fit request \n");
            mCounters.mIncorrectOperation++;
            return;
        }

        // Register reply packet to the resolved transaction
        transaction->setReplyPacket(packet);

        // give the transaction the control over the shared buffer, so it lives as long as the
        // transaction does
        transaction->setBuffer(rx);

        // Update transaction state
        transaction->setState(RmapTransaction::State::replyReceived);

        console_out("RMAP-Initiator: Reply received, thus notifying blocking thread\n");

        if (transaction->isBlockingMode())
        {
            console_out("RMAP-Initiator: Transaction with TID %u is blocking, thus "
                        "releasing lock...\n",
                        transaction->getTransactionID());
            transaction->releaseTransaction();
        }
    }
}

RmapTransaction*
RmapInitiator::resolveTransaction(RmapPacket* packet)
{
    uint16_t transactionID = packet->getTransactionID();
    RmapTransaction* transaction = mTransactionsList.getTransaction(transactionID);

    if (nullptr == transaction)
    {
        // TID is not in use
        console_out("RMAP-Initiator: Unexpected RMAP Reply Packet Was Received\n");
        return nullptr;
    }
    return transaction;
}

uint16_t
RmapInitiator::getNextAvailableTransactionID()
{
    // Check in the current transaction list that the TID is not in use
    uint16_t i = (mTransactionId < rmap::maxTransactionId) ? (mTransactionId + 1) : 0;
    while (i != mTransactionId)
    {
        if (!mTransactionsList.isTransactionIdUsed(i))
        {
            mTransactionId = i;
            break;
        }

        if (i >= rmap::maxTransactionId)
        {
            i = 0;
        }
        else
        {
            i++;
        }
    }
    return mTransactionId;
}
