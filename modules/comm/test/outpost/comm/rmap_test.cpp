/*
 * rmap_test.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: user
 */

#include <unittest/harness.h>

#include <outpost/comm/rmap/rmap_initiator.h>
#include <unittest/hal/spacewire_stub.h>
#include <outpost/utils/bounded_array.h>
#include <outpost/utils/bit_access.h>
#include <outpost/utils/crc.h>
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
               uint8_t *buffer,
               uint16_t len)
    {
        return init.sendPacket(trans, buffer, len);
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
            mRmapTarget(targetName, 1,
                    outpost::BoundedArray<uint8_t>(targetSpwAddress,
                            numberOfTargetSpwAddresses),
                    outpost::BoundedArray<uint8_t>(replyAddress,
                            replyAddressLength), targetLogicalAddress, 0xFE,
                    key),
            mTargetNodes(), mRmapInitiator(mSpaceWire, &mTargetNodes),
            mTestingRmap(), mNonRmapReceiver()
    {
    }

    virtual void
    SetUp() override
    {
        mSpaceWire.open();
        mSpaceWire.up(outpost::time::Duration::zero());
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

TEST_F(RmapTest, shouldSendWritePacket)
{
    EXPECT_TRUE(mTargetNodes.addTargetNode(&mRmapTarget));
    EXPECT_EQ(1, mTargetNodes.getSize());
    EXPECT_EQ(&mRmapTarget, mTargetNodes.getTargetNode(targetName));

    uint8_t buffer[4] = { 0x01, 0x02, 0x03, 0x04 };
    RmapTransaction transaction;
    RmapPacket *cmd = transaction.getCommandPacket();
    cmd->setInitiatorLogicalAddress(0xFE);
    cmd->setWrite();
    cmd->setCommand();
    cmd->unsetIncrementFlag();
    cmd->unsetVerifyFlag();
    cmd->unsetReplyFlag();
    transaction.setBlockingMode(false);
    cmd->setExtendedAddress(0x00);
    cmd->setAddress(0x100);
    cmd->setDataLength(4);
    cmd->setTargetInformation(&mRmapTarget);
    transaction.setInitiatorLogicalAddress(cmd->getInitiatorLogicalAddress());
    transaction.setTimeoutDuration(outpost::time::Duration::zero());

    EXPECT_TRUE(
            mTestingRmap.sendPacket(mRmapInitiator, &transaction, buffer, 4));

    size_t expectedSize = 1;

    EXPECT_EQ(expectedSize, mSpaceWire.mSentPackets.size());
    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(RmapTest, shouldSendReadPacket)
{
    uint8_t buffer[4] = { 0x01, 0x02, 0x03, 0x04 };

    // Construct packet that should be received
    uint8_t instr = 0;
    std::vector<uint8_t> rply;

    outpost::BitAccess::set<uint8_t, 7, 6>(instr, 0);

    rply.push_back(0xFE);
    rply.push_back(0x01);
    rply.push_back(instr);
    rply.push_back(0);
    rply.push_back(0xFE);
    rply.push_back(0);
    rply.push_back(1);
    rply.push_back(0);
    rply.push_back(0);
    rply.push_back(0);
    rply.push_back(4);
    uint8_t crc = outpost::Crc8CcittReversed::calculate(
            outpost::BoundedArray<const uint8_t>(&rply.front(), rply.size()));
    rply.push_back(crc);

    auto data_start = &rply.back() + 1;
    rply.push_back(buffer[0]);
    rply.push_back(buffer[1]);
    rply.push_back(buffer[2]);
    rply.push_back(buffer[3]);
    crc = outpost::Crc8CcittReversed::calculate(
            outpost::BoundedArray<const uint8_t>(
                    reinterpret_cast<uint8_t*>(data_start), 4));
    rply.push_back(crc);

    mSpaceWire.mPacketsToReceive.emplace_back(
            unittest::hal::SpaceWireStub::Packet { rply, SpaceWire::eop });

    RmapPacket rxedPacket;
    EXPECT_TRUE(mTestingRmap.receivePacket(mRmapInitiator, &rxedPacket));
    EXPECT_TRUE(rxedPacket.isReplyPacket());

    uint8_t rxbuffer[10];
    mTestingRmap.getRxData(mRmapInitiator, rxbuffer);
    for (uint8_t i = 0; i < rxedPacket.getDataLength(); i++)
    {
        EXPECT_EQ(rxbuffer[i], buffer[i]);
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
