/*
 * Copyright (c) 2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2018, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/parameter/support.h>
#include <outpost/support/heartbeat_limiter.h>

#include <unittest/harness.h>
#include <unittest/smpc/topic_logger.h>
#include <unittest/time/testing_clock.h>

using namespace outpost::support;

static constexpr auto source = outpost::support::parameter::HeartbeatSource::default0;

class HeartbeatLimiterTest : public testing::Test
{
public:
    static constexpr outpost::time::Duration heartbeatInterval = outpost::time::Seconds(1);
    static constexpr outpost::time::Duration executionTimeoutLong = outpost::time::Seconds(10);
    static constexpr outpost::time::Duration executionTimeoutShort = outpost::time::Seconds(2);

    HeartbeatLimiterTest() :
        mClock(),
        mHeartbeat(mClock, heartbeatInterval, source),
        mLogger(watchdogHeartbeat)
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
        unittest::smpc::TestingSubscription::releaseAllSubscriptions();
    }

    unittest::time::TestingClock mClock;
    outpost::support::HeartbeatLimiter mHeartbeat;
    unittest::smpc::TopicLogger<const Heartbeat> mLogger;
};

constexpr outpost::time::Duration HeartbeatLimiterTest::heartbeatInterval;
constexpr outpost::time::Duration HeartbeatLimiterTest::executionTimeoutLong;
constexpr outpost::time::Duration HeartbeatLimiterTest::executionTimeoutShort;

TEST_F(HeartbeatLimiterTest, sendHeartbeatOnFirstInvocation)
{
    mHeartbeat.send(executionTimeoutShort);

    ASSERT_FALSE(mLogger.isEmpty());
    auto& entry = mLogger.getNext();

    EXPECT_EQ(source, entry.mSource);
    EXPECT_EQ(outpost::time::SpacecraftElapsedTime::afterEpoch(
                      heartbeatInterval + executionTimeoutShort + parameter::heartbeatTolerance),
              entry.mTimeout);

    mLogger.dropNext();
    EXPECT_TRUE(mLogger.isEmpty());
}

TEST_F(HeartbeatLimiterTest, shouldNotResendHeartbeatBeforeInterval)
{
    // Initial heartbeat
    mHeartbeat.send(executionTimeoutShort);
    mLogger.clear();

    // No heartbeat directly afterwards
    mHeartbeat.send(executionTimeoutShort);
    mHeartbeat.send(executionTimeoutShort);
    EXPECT_TRUE(mLogger.isEmpty());

    mClock.incrementBy(heartbeatInterval);

    // After the interval has elapsed, a new heartbeat should be generated
    mHeartbeat.send(executionTimeoutShort);
    ASSERT_FALSE(mLogger.isEmpty());

    auto& entry = mLogger.getNext();
    EXPECT_EQ(source, entry.mSource);
    EXPECT_EQ(outpost::time::SpacecraftElapsedTime::afterEpoch(
                      heartbeatInterval + executionTimeoutShort + parameter::heartbeatTolerance),
              entry.mTimeout);

    mLogger.dropNext();
    EXPECT_TRUE(mLogger.isEmpty());
}

TEST_F(HeartbeatLimiterTest, shouldResendOnShorterExecutionTimeout)
{
    // Initial heartbeat
    mHeartbeat.send(executionTimeoutLong);
    mLogger.clear();

    mHeartbeat.send(executionTimeoutShort);
    ASSERT_FALSE(mLogger.isEmpty());

    auto& entry = mLogger.getNext();
    EXPECT_EQ(source, entry.mSource);
    EXPECT_EQ(outpost::time::SpacecraftElapsedTime::afterEpoch(
                      heartbeatInterval + executionTimeoutShort + parameter::heartbeatTolerance),
              entry.mTimeout);

    mLogger.dropNext();
    EXPECT_TRUE(mLogger.isEmpty());
}
