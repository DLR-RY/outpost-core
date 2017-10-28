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
// ----------------------------------------------------------------------------
#include <unittest/harness.h>

#include <outpost/comm/rmap/rmap_initiator.h>
#include <unittest/hal/spacewire_stub.h>
#include <outpost/container/bounded_array.h>
#include <outpost/storage/bit_access.h>
#include <outpost/coding/crc.h>
#include <outpost/smpc/subscription.h>

namespace outpost
{
namespace comm
{
class TestingRmap
{
public:
    uint8_t
    getActiveTransactions(RmapInitiator &init)
    {
        return init.mTransactionsList.getActiveTransactions();
    }

    void
    addTransaction(RmapInitiator &init, RmapTransaction *trans)
    {
        init.mTransactionsList.addTransaction(trans);
    }

    void
    removeTransaction(RmapInitiator &init, uint16_t tid)
    {
        init.mTransactionsList.removeTransaction(tid);
    }

    RmapTransaction*
    getTransaction(RmapInitiator &init, uint16_t tid)
    {
        return init.mTransactionsList.getTransaction(tid);
    }

    bool
    isUsedTransaction(RmapInitiator &init, uint16_t tid)
    {
        return init.mTransactionsList.isTransactionIdUsed(tid);
    }

    bool
    sendPacket(RmapInitiator &init,
               RmapTransaction *trans,
               outpost::BoundedArray<const uint8_t> data)
    {
        return init.sendPacket(trans, data);
    }

    bool
    receivePacket(RmapInitiator &init, RmapPacket *pkt)
    {
        return init.receivePacket(pkt);
    }

    void
    getRxData(RmapInitiator &init, uint8_t *buffer)
    {
        init.mRxData.getData(buffer);
    }

    void
    constructPacketHeader(RmapPacket& pckt, uint8_t* buffer)
    {
        outpost::Serialize stream(buffer);
        pckt.constructHeader(stream);
    }

};
}
}

class NonRmapReceiverTest : public outpost::smpc::Subscriber
{
public:
    NonRmapReceiverTest() :
            mLocalDataBuffer(), mDataReceived(false)
    {
    }

    ~NonRmapReceiverTest()
    {
    }

    void
    receiveData(outpost::comm::NonRmapDataType *data)
    {
        memcpy(mLocalDataBuffer, data->begin(), data->getNumberOfElements());
        mDataReceived = true;
    }

    uint8_t mLocalDataBuffer[32];
    bool mDataReceived;
};

using outpost::hal::SpaceWire;

using namespace outpost::comm;

class RmapTest : public testing::Test
{
public:
    static const uint8_t numberOfTargetSpwAddresses = 1;
    static const uint8_t replyAddressLength = 4;
    static uint8_t targetSpwAddress[numberOfTargetSpwAddresses];
    static uint8_t replyAddress[replyAddressLength];
    static const uint8_t targetLogicalAddress = 0xFE;
    static const uint8_t key = 0xFF;
    static const char *targetName;

    RmapTest() :
            mSpaceWire(100),
            mRmapTarget(targetName, 1, targetLogicalAddress, key),
            mTargetNodes(), mRmapInitiator(mSpaceWire, &mTargetNodes, 100, 4096),
            mTestingRmap(), mNonRmapReceiver()
    {
    }

    virtual void
    SetUp() override
    {
        mSpaceWire.open();
        mSpaceWire.up(outpost::time::Duration::zero());
        mRmapTarget.setTargetSpaceWireAddress(
                            outpost::BoundedArray<uint8_t>(targetSpwAddress,
                                    numberOfTargetSpwAddresses));
        mRmapTarget.setReplyAddress(
                            outpost::BoundedArray<uint8_t>(replyAddress,
                                    replyAddressLength));
    }

    virtual void
    TearDown() override
    {
    }

    unittest::hal::SpaceWireStub mSpaceWire;
    RmapTargetNode mRmapTarget;
    RmapTargetsList mTargetNodes;
    RmapInitiator mRmapInitiator;
    TestingRmap mTestingRmap;
    NonRmapReceiverTest mNonRmapReceiver;
};

uint8_t RmapTest::targetSpwAddress[numberOfTargetSpwAddresses] = { 0 };
uint8_t RmapTest::replyAddress[replyAddressLength] = { 0, 0, 0, 2 };
const char *RmapTest::targetName = "SpWR";

// ----------------------------------------------------------------------------
TEST_F(RmapTest, shouldGetEmptyRmapTargetList)
{
    EXPECT_EQ(0, mTargetNodes.getSize());
    EXPECT_EQ(nullptr, mTargetNodes.getTargetNode(targetName));
}

TEST_F(RmapTest, shouldRegisterRmapTargetInList)
{
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));
}

TEST_F(RmapTest, shouldGetEmptyTransactionsList)
{
    EXPECT_EQ(0, mTestingRmap.getActiveTransactions(mRmapInitiator));
}

TEST_F(RmapTest, shouldAddAndRemoveEmptyTransactionInList)
{
    RmapTransaction transaction;
    transaction.setTransactionID(50);
    mTestingRmap.addTransaction(mRmapInitiator, &transaction);
    EXPECT_EQ(1, mTestingRmap.getActiveTransactions(mRmapInitiator));
    mTestingRmap.removeTransaction(mRmapInitiator, 50);
    EXPECT_EQ(0, mTestingRmap.getActiveTransactions(mRmapInitiator));
}

TEST_F(RmapTest, shouldGetAddedTransactionFromList)
{
    RmapTransaction transaction;
    transaction.setTransactionID(60);
    mTestingRmap.addTransaction(mRmapInitiator, &transaction);
    EXPECT_EQ(1, mTestingRmap.getActiveTransactions(mRmapInitiator));
    EXPECT_EQ(60,
            mTestingRmap.getTransaction(mRmapInitiator, 60)->getTransactionID());
}

TEST_F(RmapTest, shouldGetUsedTransactionFromList)
{
    RmapTransaction transaction;
    transaction.setTransactionID(80);
    mTestingRmap.addTransaction(mRmapInitiator, &transaction);
    EXPECT_EQ(1, mTestingRmap.getActiveTransactions(mRmapInitiator));
    EXPECT_TRUE(mTestingRmap.isUsedTransaction(mRmapInitiator, 80));
}

TEST_F(RmapTest, shouldSetReplyPacketType)
{
    RmapPacket::InstructionField instruction;
    instruction.setPacketType(RmapPacket::InstructionField::replyPacket);
    EXPECT_EQ(RmapPacket::InstructionField::replyPacket,
            instruction.getPacketType());
}

TEST_F(RmapTest, shouldSetCommandPacketType)
{
    RmapPacket::InstructionField instruction;
    instruction.setPacketType(RmapPacket::InstructionField::commandPacket);
    EXPECT_EQ(RmapPacket::InstructionField::commandPacket,
            instruction.getPacketType());
}

TEST_F(RmapTest, shouldSetWriteOperationType)
{
    RmapPacket::InstructionField instruction;
    instruction.setOperation(RmapPacket::InstructionField::write);
    EXPECT_EQ(RmapPacket::InstructionField::write, instruction.getOperation());
}

TEST_F(RmapTest, shouldSetReadOperationType)
{
    RmapPacket::InstructionField instruction;
    instruction.setOperation(RmapPacket::InstructionField::read);
    EXPECT_EQ(RmapPacket::InstructionField::read, instruction.getOperation());
}

TEST_F(RmapTest, shouldSetZeroReplyAddressLength)
{
    RmapPacket::InstructionField instruction;

    instruction.setReplyAddressLength(RmapPacket::InstructionField::zeroBytes);
    EXPECT_EQ(RmapPacket::InstructionField::zeroBytes,
            instruction.getReplyAddressLength());

    instruction.setReplyAddressLength(RmapPacket::InstructionField::fourBytes);
    EXPECT_EQ(RmapPacket::InstructionField::fourBytes,
            instruction.getReplyAddressLength());

    instruction.setReplyAddressLength(RmapPacket::InstructionField::eigthBytes);
    EXPECT_EQ(RmapPacket::InstructionField::eigthBytes,
            instruction.getReplyAddressLength());

    instruction.setReplyAddressLength(
            RmapPacket::InstructionField::twelveBytes);
    EXPECT_EQ(RmapPacket::InstructionField::twelveBytes,
            instruction.getReplyAddressLength());
}

TEST_F(RmapTest, shouldBuildVerifyPacketHeaderCRC)
{
    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // *** Constructing command ***
    uint8_t sendBuffer[50] = {0};
    RmapPacket send;
    send.setInitiatorLogicalAddress(rmap::defaultLogicalAddress);
    send.setWrite();
    send.setCommand();
    send.unsetIncrementFlag();
    send.unsetVerifyFlag();
    send.unsetReplyFlag();
    send.setExtendedAddress(rmap::defaultExtendedAddress);
    send.setAddress(0x100);
    send.setDataLength(4);
    send.setTargetInformation(mRmapTarget);
    send.setInitiatorLogicalAddress(0xFE);

    mTestingRmap.constructPacketHeader(send, sendBuffer);

    // Acc to RMAP protocol definition, all the SpW target address will be truncated by the
    // router upon receiving the packet at the target node. Thus final packet will be considered
    // without SpW target fields
    uint8_t numberOfTargets = send.getTargetSpaceWireAddress().getNumberOfElements();
    uint8_t calculatedCrc = outpost::Crc8CcittReversed::calculate(
        outpost::BoundedArray<uint8_t>(sendBuffer + numberOfTargets, send.getHeaderLength() - numberOfTargets));

    EXPECT_EQ(calculatedCrc, send.getHeaderCRC());
}

TEST_F(RmapTest, shouldSendWriteCommandPacket)
{
    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // *** Constructing and sending write command ***
    uint8_t buffer[4] = { 0x01, 0x02, 0x03, 0x04 };
    RmapTransaction transaction;
    RmapPacket *cmd = transaction.getCommandPacket();
    cmd->setInitiatorLogicalAddress(rmap::defaultLogicalAddress);
    cmd->setWrite();
    cmd->setCommand();
    cmd->unsetIncrementFlag();
    cmd->unsetVerifyFlag();
    cmd->unsetReplyFlag();
    transaction.setBlockingMode(false);
    cmd->setExtendedAddress(rmap::defaultExtendedAddress);
    cmd->setAddress(0x100);
    cmd->setDataLength(4);
    cmd->setTargetInformation(mRmapTarget);
    transaction.setInitiatorLogicalAddress(cmd->getInitiatorLogicalAddress());
    transaction.setTimeoutDuration(outpost::time::Duration::zero());

    EXPECT_TRUE(
            mTestingRmap.sendPacket(mRmapInitiator, &transaction,
                    outpost::BoundedArray<uint8_t>(buffer, 4)));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(RmapTest, shouldReceiveReplyOfWriteCommandPacket)
{
    // *** Constructing and sending write reply packet acc. to RMAP standard ***
    uint8_t reply[20];
    outpost::Serialize stream { outpost::BoundedArray<uint8_t>(reply) };

    // Construct packet that should be received
    RmapPacket::InstructionField instr;

    instr.setPacketType(RmapPacket::InstructionField::replyPacket);
    instr.setOperation(RmapPacket::InstructionField::write);

    stream.store<uint8_t>(rmap::defaultLogicalAddress);  // Initiator logical address field
    stream.store<uint8_t>(rmap::protocolIdentifier);   // RMAP protocol ID field
    stream.store<uint8_t>(instr.getRaw());              // Instruction field
    stream.store<uint8_t>(0);                           // Status field
    stream.store<uint8_t>(rmap::defaultLogicalAddress);  // Target logical address field
    stream.store<uint16_t>(1);                          // Transaction ID

    uint8_t crc = outpost::Crc8CcittReversed::calculate(
            outpost::BoundedArray<uint8_t>(stream.getPointer(),
                    stream.getPosition()));
    stream.store<uint8_t>(crc);                         // Header CRC

    mSpaceWire.mPacketsToReceive.emplace_back(
            unittest::hal::SpaceWireStub::Packet { std::vector<uint8_t>(reply,
                    reply + stream.getPosition()), SpaceWire::eop });

    RmapPacket receivedPacket;
    EXPECT_TRUE(mTestingRmap.receivePacket(mRmapInitiator, &receivedPacket));
    EXPECT_TRUE(receivedPacket.isReplyPacket());
    EXPECT_TRUE(receivedPacket.isWrite());

    EXPECT_TRUE(mSpaceWire.mPacketsToReceive.empty());
    EXPECT_TRUE(mSpaceWire.noUsedReceiveBuffers());

}

TEST_F(RmapTest, shouldSendReadCommandPacket)
{
    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // *** Constructing and sending write command ***
    RmapTransaction transaction;
    RmapPacket *cmd = transaction.getCommandPacket();
    cmd->setInitiatorLogicalAddress(rmap::defaultLogicalAddress);
    cmd->setRead();
    cmd->setCommand();
    cmd->unsetIncrementFlag();
    cmd->unsetVerifyFlag();
    cmd->setReplyFlag();
    transaction.setBlockingMode(false);
    cmd->setExtendedAddress(rmap::defaultExtendedAddress);
    cmd->setAddress(0x100);
    cmd->setDataLength(4);
    cmd->setTargetInformation(mRmapTarget);
    transaction.setInitiatorLogicalAddress(cmd->getInitiatorLogicalAddress());
    transaction.setTimeoutDuration(outpost::time::Duration::zero());

    EXPECT_TRUE(
            mTestingRmap.sendPacket(mRmapInitiator, &transaction,
                    outpost::BoundedArray<uint8_t>::empty()));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(RmapTest, shouldReceiveReplyOfReadCommandPacket)
{
    uint8_t data[4] = { 0x01, 0x02, 0x03, 0x04 };
    uint8_t reply[20];
    outpost::Serialize stream { outpost::BoundedArray<uint8_t>(reply) };

    // Construct packet that should be received
    RmapPacket::InstructionField instr;

    instr.setPacketType(RmapPacket::InstructionField::replyPacket);
    instr.setOperation(RmapPacket::InstructionField::read);

    stream.store<uint8_t>(rmap::defaultLogicalAddress); // Initiator logical address field
    stream.store<uint8_t>(rmap::protocolIdentifier);    // RMAP protocol ID field
    stream.store<uint8_t>(instr.getRaw());              // Instruction field
    stream.store<uint8_t>(0);                           // Status field
    stream.store<uint8_t>(rmap::defaultLogicalAddress); // Target logical address field
    stream.store<uint16_t>(1);                          // Transaction ID
    stream.store<uint8_t>(0);                           // Reserved byte
    stream.store24(sizeof(data));                       // Transaction ID

    uint8_t crc = outpost::Crc8CcittReversed::calculate(
            outpost::BoundedArray<uint8_t>(stream.getPointer(),
                    stream.getPosition()));
    stream.store<uint8_t>(crc);                         // Header CRC

    uint8_t *dataStart = stream.getPointerToCurrentPosition();
    stream.storeBuffer(data, sizeof(data));             // Data bytes to be read

    crc = outpost::Crc8CcittReversed::calculate(
            outpost::BoundedArray<uint8_t>(dataStart, sizeof(data)));

    stream.store<uint8_t>(crc);                         // Data CRC

    mSpaceWire.mPacketsToReceive.emplace_back(
            unittest::hal::SpaceWireStub::Packet { std::vector<uint8_t>(reply,
                    reply + stream.getPosition()), SpaceWire::eop });

    RmapPacket rxedPacket;
    EXPECT_TRUE(mTestingRmap.receivePacket(mRmapInitiator, &rxedPacket));
    EXPECT_TRUE(rxedPacket.isReplyPacket());

    uint8_t rxbuffer[10];
    mTestingRmap.getRxData(mRmapInitiator, rxbuffer);
    for (uint8_t i = 0; i < rxedPacket.getDataLength(); i++)
    {
        EXPECT_EQ(rxbuffer[i], data[i]);
    }

    EXPECT_TRUE(mSpaceWire.mPacketsToReceive.empty());
    EXPECT_TRUE(mSpaceWire.noUsedReceiveBuffers());
}

TEST_F(RmapTest, shouldReceiveAndPublishNonRmapPacket)
{
    outpost::smpc::Subscription *subscription0 =
            new outpost::smpc::Subscription(
                    outpost::comm::nonRmapPacketReceived, &mNonRmapReceiver,
                    &NonRmapReceiverTest::receiveData);

    outpost::smpc::Subscription::connectSubscriptionsToTopics();

    // Packet to receive
    std::vector<uint8_t> rply = { 0xFE, 0x02, 0x03, 4, 5 };

    mSpaceWire.mPacketsToReceive.emplace_back(
            unittest::hal::SpaceWireStub::Packet { rply, SpaceWire::eop });

    RmapPacket rxedPacket;
    EXPECT_FALSE(mTestingRmap.receivePacket(mRmapInitiator, &rxedPacket));
    EXPECT_TRUE(mNonRmapReceiver.mDataReceived);

    for (uint8_t i = 0; i < 4; i++)
    {
        EXPECT_EQ(mNonRmapReceiver.mLocalDataBuffer[i], rply[i]);
    }

    EXPECT_TRUE(mSpaceWire.mPacketsToReceive.empty());
    EXPECT_TRUE(mSpaceWire.noUsedReceiveBuffers());

    delete subscription0;
}

TEST_F(RmapTest, shouldSendHigherLevelWriteCommandPacket)
{
    uint8_t dataToSend[4] = { 0x01, 0x02, 0x03, 0x04 };

    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // Send RMAP command to read from the target
    mRmapInitiator.unsetIncrementMode();
    mRmapInitiator.unsetVerifyMode();
    mRmapInitiator.unsetReplyMode();

    EXPECT_TRUE(
            mRmapInitiator.write(targetName, 0x1000,
                    outpost::BoundedArray<uint8_t>(dataToSend,
                            sizeof(dataToSend))));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}
