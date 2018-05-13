/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */

#include <unittest/harness.h>
#include <unittest/smpc/testing_subscription.h>
#include <unittest/smpc/topic_logger.h>

using namespace outpost::smpc;

struct Data
{
    int mValue;
};

static Topic<Data> topic;

class TopicLoggerTest : public testing::Test
{
public:
    TopicLoggerTest() : mLogger(topic)
    {
    }

    virtual void
    SetUp() override
    {
        unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();
    }

    virtual void
    TearDown() override
    {
        EXPECT_TRUE(mLogger.isEmpty());

        unittest::smpc::TestingSubscription::releaseAllSubscriptions();
    }

    unittest::smpc::TopicLogger<Data> mLogger;
};

TEST_F(TopicLoggerTest, shouldReceiveSingleItem)
{
    Data data = {1};
    topic.publish(data);

    ASSERT_FALSE(mLogger.isEmpty());

    auto& result = mLogger.getNext();

    EXPECT_EQ(data.mValue, result.mValue);
    mLogger.dropNext();
}

TEST_F(TopicLoggerTest, shouldReceiveMultiple)
{
    Data data1 = {1};
    Data data2 = {2};
    Data data3 = {3};
    Data data4 = {4};

    topic.publish(data1);
    topic.publish(data2);
    topic.publish(data3);
    topic.publish(data4);

    ASSERT_FALSE(mLogger.isEmpty());
    auto& result1 = mLogger.getNext();
    EXPECT_EQ(data1.mValue, result1.mValue);
    mLogger.dropNext();

    ASSERT_FALSE(mLogger.isEmpty());
    auto& result2 = mLogger.getNext();
    EXPECT_EQ(data2.mValue, result2.mValue);
    mLogger.dropNext();

    ASSERT_FALSE(mLogger.isEmpty());
    auto& result3 = mLogger.getNext();
    EXPECT_EQ(data3.mValue, result3.mValue);
    mLogger.dropNext();

    ASSERT_FALSE(mLogger.isEmpty());
    auto& result4 = mLogger.getNext();
    EXPECT_EQ(data4.mValue, result4.mValue);
    mLogger.dropNext();
}
