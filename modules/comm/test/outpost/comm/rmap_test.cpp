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
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 * - 2017-2018, Fabian Greif (DLR RY-AVS)
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/base/slice.h>
#include <outpost/comm/rmap/rmap_initiator.h>
#include <outpost/smpc/subscription.h>
#include <outpost/utils/coding/crc.h>
#include <outpost/utils/storage/bit_access.h>

#include <unittest/hal/spacewire_stub.h>
#include <unittest/harness.h>

#include <cstdlib>
#include <future>

namespace outpost
{
namespace comm
{
class TestingRmap
{
public:
    uint8_t
    getActiveTransactions(RmapInitiator& init)
    {
        return init.mTransactionsList.getNumberOfActiveTransactions();
    }

    void
    addTransaction(RmapInitiator& init, RmapTransaction* trans)
    {
        init.mTransactionsList.addTransaction(trans);
    }

    void
    removeTransaction(RmapInitiator& init, uint16_t tid)
    {
        init.mTransactionsList.removeTransaction(tid);
    }

    RmapTransaction*
    getTransaction(RmapInitiator& init, uint16_t tid)
    {
        return init.mTransactionsList.getTransaction(tid);
    }

    bool
    isUsedTransaction(RmapInitiator& init, uint16_t tid)
    {
        return init.mTransactionsList.isTransactionIdUsed(tid);
    }

    bool
    sendPacket(RmapInitiator& init, RmapTransaction* trans, outpost::Slice<const uint8_t> data)
    {
        return init.sendPacket(trans, data);
    }

    bool
    receivePacket(RmapInitiator& init,
                  RmapPacket* pkt,
                  outpost::utils::SharedBufferPointer& rxBuffer)
    {
        return init.receivePacket(pkt, rxBuffer);
    }

    void
    constructPacketHeader(RmapPacket& pckt, uint8_t* buffer)
    {
        outpost::Serialize stream(buffer);
        pckt.constructHeader(stream);
    }

    void
    initalize(RmapInitiator& init)
    {
        init.mSpW.addQueue(rmap::protocolIdentifier, &init.mPool, &init.mQueue, true);
    }

    void
    step(RmapInitiator& init)
    {
        init.doSingleStep();
    }

    void
    clear(RmapInitiator& init)
    {
        outpost::utils::SharedBufferPointer p;
        while (!init.mQueue.isEmpty())
        {
            init.mQueue.receive(p);
        }
    }
};
}  // namespace comm
}  // namespace outpost

using outpost::hal::SpaceWire;

using namespace outpost::comm;

static char name[] = "Test";
static outpost::rtos::SystemClock Clock;

class RmapTest : public testing::Test
{
public:
    static const uint8_t numberOfTargetSpwAddresses = 1;
    static const uint8_t replyAddressLength = 4;
    static uint8_t targetSpwAddress[numberOfTargetSpwAddresses];
    static uint8_t replyAddress[replyAddressLength];
    static const uint8_t targetLogicalAddress = 0xFE;
    static const uint8_t key = 0xFF;
    static const char* targetName;

    RmapTest() :
        mSpaceWire(100),
        mHandler(mSpaceWire,
                 1,
                 1,
                 name,
                 outpost::support::parameter::HeartbeatSource::default0,
                 Clock),
        mRmapTarget(targetName, 1, targetLogicalAddress, key),
        mTargetNodes(),
        mRmapInitiator(mHandler,
                       &mTargetNodes,
                       100,
                       4096,
                       outpost::support::parameter::HeartbeatSource::default0),
        mTestingRmap()
    {
        mTestingRmap.initalize(mRmapInitiator);
    }

    virtual void
    SetUp() override
    {
        mSpaceWire.open();
        mSpaceWire.up(outpost::time::Duration::zero());
        mRmapTarget.setTargetSpaceWireAddress(outpost::asSlice(targetSpwAddress));
        mRmapTarget.setReplyAddress(outpost::asSlice(replyAddress));
        mTestingRmap.clear(mRmapInitiator);
    }

    virtual void
    TearDown() override
    {
    }

    unittest::hal::SpaceWireStub mSpaceWire;
    outpost::hal::SpaceWireMultiProtocolHandler<1> mHandler;
    RmapTargetNode mRmapTarget;
    RmapTargetsList mTargetNodes;
    RmapInitiator mRmapInitiator;
    TestingRmap mTestingRmap;
};

std::vector<uint8_t>
getReadReplyPacket(std::vector<uint8_t>& command, uint8_t readValue, uint16_t countRead);

/**
 * Note: Only working for command with logical addressing!!!
 */
std::vector<uint8_t>
getReadReplyPacket(std::vector<uint8_t>& command, uint8_t readValue, uint16_t countRead)
{
    std::vector<uint8_t> ret;
    ret.resize(countRead + 13);

    // data
    for (unsigned int i = 0; i < countRead; i++)
    {
        ret[i + 12] = readValue;
    }
    ret[countRead + 12] = outpost::Crc8CcittReversed::calculate(
            outpost::Slice<uint8_t>::unsafe(&ret[12], countRead));

    ret[8] = 0;  // this function does max read replys o of uint16_t size
    ret[9] = (countRead & 0xff00) >> 8;
    ret[10] = countRead & 0x00ff;

    // address
    ret[0] = command[4];
    ret[4] = command[0];

    // transaction id
    ret[5] = command[5];
    ret[6] = command[6];

    ret[7] = 0;  // reserved

    ret[1] = rmap::protocolIdentifier;
    ret[2] = command[2] & 0xbf;  // remove command flag
    ret[3] = RmapReplyStatus::commandExecutedSuccessfully;
    ret[11] = outpost::Crc8CcittReversed::calculate(outpost::Slice<uint8_t>::unsafe(&ret[0], 11));

    return ret;
}

uint8_t RmapTest::targetSpwAddress[numberOfTargetSpwAddresses] = {0};
uint8_t RmapTest::replyAddress[replyAddressLength] = {0, 0, 0, 2};
const char* RmapTest::targetName = "SpWR";

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
    EXPECT_EQ(60, mTestingRmap.getTransaction(mRmapInitiator, 60)->getTransactionID());
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
    EXPECT_EQ(RmapPacket::InstructionField::replyPacket, instruction.getPacketType());
}

TEST_F(RmapTest, shouldSetCommandPacketType)
{
    RmapPacket::InstructionField instruction;
    instruction.setPacketType(RmapPacket::InstructionField::commandPacket);
    EXPECT_EQ(RmapPacket::InstructionField::commandPacket, instruction.getPacketType());
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
    EXPECT_EQ(RmapPacket::InstructionField::zeroBytes, instruction.getReplyAddressLength());

    instruction.setReplyAddressLength(RmapPacket::InstructionField::fourBytes);
    EXPECT_EQ(RmapPacket::InstructionField::fourBytes, instruction.getReplyAddressLength());

    instruction.setReplyAddressLength(RmapPacket::InstructionField::eigthBytes);
    EXPECT_EQ(RmapPacket::InstructionField::eigthBytes, instruction.getReplyAddressLength());

    instruction.setReplyAddressLength(RmapPacket::InstructionField::twelveBytes);
    EXPECT_EQ(RmapPacket::InstructionField::twelveBytes, instruction.getReplyAddressLength());
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

    // Acc to RMAP protocol definition, all the SpW target address will be truncated by
    // the router upon receiving the packet at the target node. Thus final packet will be
    // considered without SpW target fields
    uint8_t numberOfTargets = send.getTargetSpaceWireAddress().getNumberOfElements();
    uint8_t calculatedCrc = outpost::Crc8CcittReversed::calculate(outpost::Slice<uint8_t>::unsafe(
            sendBuffer + numberOfTargets, send.getHeaderLength() - numberOfTargets));

    EXPECT_EQ(calculatedCrc, send.getHeaderCRC());
}

TEST_F(RmapTest, shouldSendWriteCommandPacket)
{
    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // *** Constructing and sending write command ***
    uint8_t buffer[4] = {0x01, 0x02, 0x03, 0x04};
    RmapTransaction transaction;
    RmapPacket* cmd = transaction.getCommandPacket();
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

    EXPECT_TRUE(mTestingRmap.sendPacket(mRmapInitiator, &transaction, outpost::asSlice(buffer)));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(RmapTest, shouldReceiveReplyOfWriteCommandPacket)
{
    outpost::utils::SharedBufferPointer rxBuffer;
    // *** Constructing and sending write reply packet acc. to RMAP standard ***
    uint8_t reply[20];
    outpost::Serialize stream{outpost::asSlice(reply)};

    // Construct packet that should be received
    RmapPacket::InstructionField instr;

    instr.setPacketType(RmapPacket::InstructionField::replyPacket);
    instr.setOperation(RmapPacket::InstructionField::write);

    stream.store<uint8_t>(rmap::defaultLogicalAddress);  // Initiator logical address field
    stream.store<uint8_t>(rmap::protocolIdentifier);     // RMAP protocol ID field
    stream.store<uint8_t>(instr.getRaw());               // Instruction field
    stream.store<uint8_t>(0);                            // Status field
    stream.store<uint8_t>(rmap::defaultLogicalAddress);  // Target logical address field
    stream.store<uint16_t>(1);                           // Transaction ID

    uint8_t crc = outpost::Crc8CcittReversed::calculate(
            outpost::Slice<uint8_t>::unsafe(stream.getPointer(), stream.getPosition()));
    stream.store<uint8_t>(crc);  // Header CRC

    mHandler.handlePackage(outpost::asSlice(reply).first(stream.getPosition()),
                           stream.getPosition());

    RmapPacket receivedPacket;
    EXPECT_TRUE(mTestingRmap.receivePacket(mRmapInitiator, &receivedPacket, rxBuffer));
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
    RmapPacket* cmd = transaction.getCommandPacket();
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

    EXPECT_TRUE(mTestingRmap.sendPacket(
            mRmapInitiator, &transaction, outpost::Slice<uint8_t>::empty()));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(RmapTest, shouldReceiveReplyOfReadCommandPacket)
{
    outpost::utils::SharedBufferPointer rxBuffer;
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t reply[20];
    outpost::Serialize stream{outpost::Slice<uint8_t>(reply)};

    // Construct packet that should be received
    RmapPacket::InstructionField instr;

    instr.setPacketType(RmapPacket::InstructionField::replyPacket);
    instr.setOperation(RmapPacket::InstructionField::read);

    stream.store<uint8_t>(rmap::defaultLogicalAddress);  // Initiator logical address field
    stream.store<uint8_t>(rmap::protocolIdentifier);     // RMAP protocol ID field
    stream.store<uint8_t>(instr.getRaw());               // Instruction field
    stream.store<uint8_t>(0);                            // Status field
    stream.store<uint8_t>(rmap::defaultLogicalAddress);  // Target logical address field
    stream.store<uint16_t>(1);                           // Transaction ID
    stream.store<uint8_t>(0);                            // Reserved byte
    stream.store24(sizeof(data));                        // Transaction ID

    uint8_t crc = outpost::Crc8CcittReversed::calculate(
            outpost::Slice<uint8_t>::unsafe(stream.getPointer(), stream.getPosition()));
    stream.store<uint8_t>(crc);  // Header CRC

    uint8_t* dataStart = stream.getPointerToCurrentPosition();
    stream.store(outpost::asSlice(data));  // Data bytes to be read

    crc = outpost::Crc8CcittReversed::calculate(
            outpost::Slice<uint8_t>::unsafe(dataStart, sizeof(data)));

    stream.store<uint8_t>(crc);  // Data CRC

    mHandler.handlePackage(outpost::asSlice(reply).first(stream.getPosition()),
                           stream.getPosition());

    RmapPacket rxedPacket;
    EXPECT_TRUE(mTestingRmap.receivePacket(mRmapInitiator, &rxedPacket, rxBuffer));
    EXPECT_TRUE(rxedPacket.isReplyPacket());

    for (uint8_t i = 0; i < rxedPacket.getDataLength(); i++)
    {
        EXPECT_EQ(data[i], rxedPacket.getData()[i]);
    }

    EXPECT_TRUE(mSpaceWire.mPacketsToReceive.empty());
    EXPECT_TRUE(mSpaceWire.noUsedReceiveBuffers());
}

TEST_F(RmapTest, shouldSendHigherLevelWriteCommandPacket)
{
    uint8_t dataToSend[4] = {0x01, 0x02, 0x03, 0x04};

    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // Send RMAP command to read from the target
    RMapOptions options;
    options.mIncrementMode = false;
    options.mReplyMode = false;
    options.mVerifyMode = false;

    static const uint8_t extaddress = 0x7e;
    static const uint32_t address = 0x1000;

    EXPECT_TRUE(mRmapInitiator.write(
            targetName, options, address, extaddress, outpost::asSlice(dataToSend)));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());

    // check send package
    auto& packet = *mSpaceWire.mSentPackets.begin();

    // we start with the compare at the extended memory address as we can find it nicely
    uint32_t start = 0;
    while ((packet.data[start] != extaddress) && start < packet.data.size())
    {
        start++;
    }

    EXPECT_EQ(start + 10 + sizeof(dataToSend), packet.data.size());
    EXPECT_EQ(packet.data[start + 1], (address >> 24) & 0xff);
    EXPECT_EQ(packet.data[start + 2], (address >> 16) & 0xff);
    EXPECT_EQ(packet.data[start + 3], (address >> 8) & 0xff);
    EXPECT_EQ(packet.data[start + 4], (address) &0xff);

    uint32_t size = sizeof(dataToSend);

    EXPECT_EQ(packet.data[start + 5], (size >> 16) & 0xff);
    EXPECT_EQ(packet.data[start + 6], (size >> 8) & 0xff);
    EXPECT_EQ(packet.data[start + 7], (size) &0xff);
    // start + 8 is crc

    EXPECT_EQ(packet.data[start + 9 + 0], dataToSend[0]);
    EXPECT_EQ(packet.data[start + 9 + 1], dataToSend[1]);
    EXPECT_EQ(packet.data[start + 9 + 2], dataToSend[2]);
    EXPECT_EQ(packet.data[start + 9 + 3], dataToSend[3]);
}

TEST_F(RmapTest, testRead)
{
    uint8_t readBuffer[4] = {0x00, 0x00, 0x00, 0x00};

    uint8_t readValue = 0xf0;

    // for easier parsing of send command
    mRmapTarget.setReplyAddress(outpost::Slice<uint8_t>::empty());
    mRmapTarget.setTargetSpaceWireAddress(outpost::Slice<uint8_t>::empty());

    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // Send RMAP command to read from the target
    RMapOptions options;
    options.mIncrementMode = true;
    options.mReplyMode = true;
    options.mVerifyMode = true;

    static const uint8_t extaddress = 0x7e;
    static const uint32_t address = 0x1000;

    auto read1 = std::async(std::launch::async, [&]() {
        return mRmapInitiator.read(
                targetName, options, address, extaddress, outpost::asSlice(readBuffer));
    });
    read1.wait_for(std::chrono::seconds(1));  // give it time to send

    // check send package
    auto& packet = *mSpaceWire.mSentPackets.begin();

    // parsing result requires a logical addressing command
    EXPECT_EQ(packet.data.size(), 16u);

    auto answer = getReadReplyPacket(packet.data, readValue, sizeof(readBuffer));

    mHandler.handlePackage(outpost::asSlice(answer), answer.size());
    mTestingRmap.step(mRmapInitiator);

    auto status = read1.wait_for(std::chrono::seconds(1));  // give it time to send

    if (status == std::future_status::ready)
    {
        EXPECT_EQ(RmapResultType::success, read1.get().getResult());
    }
    else
    {
        EXPECT_TRUE(false);
        exit(-1);  // no other way to stop the threads, unit tests will still fail.
    }

    for (unsigned int i = 0; i < sizeof(readBuffer); i++)
    {
        EXPECT_EQ(readBuffer[i], readValue);
    }
}

TEST_F(RmapTest, testReadAnswerTooShort)
{
    uint8_t readBuffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t readValue = 0xf0;

    // for easier parsing of send command
    mRmapTarget.setReplyAddress(outpost::Slice<uint8_t>::empty());
    mRmapTarget.setTargetSpaceWireAddress(outpost::Slice<uint8_t>::empty());

    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // Send RMAP command to read from the target
    RMapOptions options;
    options.mIncrementMode = true;
    options.mReplyMode = true;
    options.mVerifyMode = true;

    static const uint8_t extaddress = 0x7e;
    static const uint32_t address = 0x1000;

    auto read1 = std::async(std::launch::async, [&]() {
        return mRmapInitiator.read(
                targetName, options, address, extaddress, outpost::asSlice(readBuffer));
    });
    read1.wait_for(std::chrono::seconds(1));  // give it time to send

    // check send package
    auto& packet = *mSpaceWire.mSentPackets.begin();

    // parsing result requires a logical addressing command
    EXPECT_EQ(packet.data.size(), 16u);

    const uint32_t answerSize = sizeof(readBuffer) / 2;

    auto answer = getReadReplyPacket(packet.data, readValue, answerSize);

    mHandler.handlePackage(outpost::asSlice(answer), answer.size());
    mTestingRmap.step(mRmapInitiator);

    auto status = read1.wait_for(std::chrono::seconds(1));  // give it time to send

    if (status == std::future_status::ready)
    {
        RmapResult result = read1.get();
        EXPECT_EQ(RmapResultType::replyTooShort, result.getResult());
        EXPECT_EQ(answerSize, result.getReadBytes());
    }
    else
    {
        EXPECT_TRUE(false);
        exit(-1);  // no other way to stop the threads, unit tests will still fail.
    }

    // the data we got is still correct
    for (unsigned int i = 0; i < answerSize; i++)
    {
        EXPECT_EQ(readBuffer[i], readValue);
    }
}

TEST_F(RmapTest, testReadSwitchReply)
{
	static_assert(rmap::numberOfReceiveBuffer >= 2,"Two package switch test requires a receive buffer of at least 2");

    uint8_t readBuffer1[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t readBuffer2[4] = {0x00, 0x00, 0x00, 0x00};

    uint8_t readValue1 = 0xf0;
    uint8_t readValue2 = 0xf0;

    // for easier parsing of send command
    mRmapTarget.setReplyAddress(outpost::Slice<uint8_t>::empty());
    mRmapTarget.setTargetSpaceWireAddress(outpost::Slice<uint8_t>::empty());

    // Register RMAP target
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    // Send RMAP command to read from the target
    RMapOptions options;
    options.mIncrementMode = true;
    options.mReplyMode = true;
    options.mVerifyMode = true;

    static const uint8_t extaddress = 0x7e;
    static const uint32_t address = 0x1000;

    auto read1 = std::async(std::launch::async, [&]() {
        return mRmapInitiator.read(
                targetName, options, address, extaddress, outpost::asSlice(readBuffer1));
    });
    read1.wait_for(std::chrono::seconds(1));  // give it time to send

    auto read2 = std::async(std::launch::async, [&]() {
        return mRmapInitiator.read(
                targetName, options, address+4, extaddress, outpost::asSlice(readBuffer2));
    });
    read2.wait_for(std::chrono::seconds(1));  // give it time to send

    EXPECT_EQ(2u, mSpaceWire.mSentPackets.size());

    // check send package
    auto it = mSpaceWire.mSentPackets.begin();
    auto& packet1 = *it;
    auto& packet2 = *(++it);

    // parsing result requires a logical addressing command
    EXPECT_EQ(packet1.data.size(), 16u);
    EXPECT_EQ(packet2.data.size(), 16u);

    auto answer2 = getReadReplyPacket(packet2.data, readValue2, sizeof(readBuffer2));

    mHandler.handlePackage(outpost::asSlice(answer2), answer2.size());
    mTestingRmap.step(mRmapInitiator);

    auto status = read2.wait_for(std::chrono::seconds(1));  // give it time to send

    if (status == std::future_status::ready)
    {
        EXPECT_EQ(RmapResultType::success, read2.get().getResult());
    }
    else
    {
        EXPECT_TRUE(false);
        exit(-1);  // no other way to stop the threads, unit tests will still fail.
    }

    auto answer1 = getReadReplyPacket(packet1.data, readValue1, sizeof(readBuffer1));

    mHandler.handlePackage(outpost::asSlice(answer1), answer1.size());
    mTestingRmap.step(mRmapInitiator);

    status = read1.wait_for(std::chrono::seconds(1));  // give it time to send

    if (status == std::future_status::ready)
    {
        EXPECT_EQ(RmapResultType::success, read1.get().getResult());
    }
    else
    {
        EXPECT_TRUE(false);
        exit(-1);  // no other way to stop the threads, unit tests will still fail.
    }

    for (unsigned int i = 0; i < sizeof(readBuffer1); i++)
    {
        EXPECT_EQ(readBuffer1[i], readValue1);
    }

    for (unsigned int i = 0; i < sizeof(readBuffer2); i++)
    {
        EXPECT_EQ(readBuffer2[i], readValue2);
    }
}
