/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/time/timeout.h>

#include <unittest/harness.h>
#include <unittest/time/testing_clock.h>

using namespace outpost::time;

class TimeoutTest : public testing::Test
{
public:
    unittest::time::TestingClock mClock;
};

TEST_F(TimeoutTest, shouldBeStoppedAfterDefaultConstruction)
{
    Timeout timeout;

    EXPECT_TRUE(timeout.isStopped());
    EXPECT_EQ(Timeout::stopped, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldBeArmedAfterConstructionWithTime)
{
    Timeout timeout(mClock, outpost::time::Milliseconds(100));

    EXPECT_TRUE(timeout.isArmed(mClock));
    EXPECT_EQ(Timeout::armed, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldExpiredAfterGivenTime)
{
    Timeout timeout(mClock, outpost::time::Milliseconds(100));

    mClock.setTime(SpacecraftElapsedTime::afterEpoch(outpost::time::Milliseconds(100)));

    EXPECT_TRUE(timeout.isExpired(mClock));
    EXPECT_EQ(Timeout::expired, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldRestartFromStopped)
{
    Timeout timeout;

    timeout.restart(mClock, outpost::time::Milliseconds(100));

    mClock.setTime(SpacecraftElapsedTime::afterEpoch(outpost::time::Milliseconds(100)));

    EXPECT_TRUE(timeout.isExpired(mClock));
    EXPECT_EQ(Timeout::expired, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldRestartFromExpired)
{
    Timeout timeout(mClock, outpost::time::Milliseconds(100));
    mClock.setTime(SpacecraftElapsedTime::afterEpoch(outpost::time::Milliseconds(100)));
    timeout.isExpired(mClock);

    timeout.restart(mClock, outpost::time::Milliseconds(200));
    EXPECT_FALSE(timeout.isExpired(mClock));
    EXPECT_EQ(Timeout::armed, timeout.getState(mClock));
}
