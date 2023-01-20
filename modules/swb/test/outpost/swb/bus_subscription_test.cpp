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
 * - 2020, Jan-Gerd Mess (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/swb/bus_subscription.h>

#include <unittest/harness.h>

class BusSubscriptionTest : public ::testing::Test
{
public:
    BusSubscriptionTest()
    {
    }

    virtual void
    SetUp() override
    {
    }

    virtual void
    TearDown() override
    {
    }
};
using MessageId = uint16_t;

TEST_F(BusSubscriptionTest, constructorTest)
{
    MessageId id = 0x0100;

    {
        outpost::swb::BusSubscription<MessageId> sub1(id);
        EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 0U);
    }

    outpost::swb::BusSubscription<MessageId> sub2(id, 0xFF00);
    EXPECT_EQ(sub2.getNumberOfMatchedMessages(), 0U);
}

TEST_F(BusSubscriptionTest, matchTest)
{
    MessageId id = 0x1234;
    outpost::swb::BusSubscription<MessageId> sub1(id);

    EXPECT_FALSE(sub1.matches(0U));
    EXPECT_FALSE(sub1.matches(0x1235));
    EXPECT_FALSE(sub1.matches(0x1233));
    EXPECT_FALSE(sub1.matches(0xFFFF));
    EXPECT_FALSE(sub1.matches(0x0234));
    EXPECT_FALSE(sub1.matches(0x1200));

    EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 0U);

    EXPECT_TRUE(sub1.matches(0x1234));

    EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 1U);
}

TEST_F(BusSubscriptionTest, matchTestWithMask)
{
    MessageId id = 0x1234;
    outpost::swb::BusSubscription<MessageId> sub1(id, 0xFF00);

    EXPECT_FALSE(sub1.matches(0U));
    EXPECT_FALSE(sub1.matches(0xFFFF));
    EXPECT_FALSE(sub1.matches(0x0233));

    EXPECT_TRUE(sub1.matches(0x1200));
    EXPECT_TRUE(sub1.matches(0x1235));
    EXPECT_TRUE(sub1.matches(0x1233));
    EXPECT_TRUE(sub1.matches(0x12FF));
    EXPECT_TRUE(sub1.matches(0x1234));

    EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 5U);

    outpost::swb::BusSubscription<MessageId> sub2(id, 0x00FF);

    EXPECT_FALSE(sub2.matches(0U));
    EXPECT_FALSE(sub2.matches(0xFFFF));
    EXPECT_FALSE(sub2.matches(0x1200));
    EXPECT_FALSE(sub2.matches(0x1235));
    EXPECT_FALSE(sub2.matches(0x1233));
    EXPECT_FALSE(sub2.matches(0x12FF));

    EXPECT_TRUE(sub2.matches(0x0234));
    EXPECT_TRUE(sub2.matches(0x1034));
    EXPECT_TRUE(sub2.matches(0x0034));
    EXPECT_TRUE(sub2.matches(0x1234));

    EXPECT_EQ(sub2.getNumberOfMatchedMessages(), 4U);
}
