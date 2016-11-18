/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include <unittest/harness.h>
#include <unittest/time/testing_clock.h>

#include <cobc/time/timeout.h>

using namespace cobc::time;

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
    Timeout timeout(mClock, cobc::time::Milliseconds(100));

    EXPECT_TRUE(timeout.isArmed(mClock));
    EXPECT_EQ(Timeout::armed, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldExpiredAfterGivenTime)
{
    Timeout timeout(mClock, cobc::time::Milliseconds(100));

    mClock.setTime(SpacecraftElapsedTime::afterEpoch(cobc::time::Milliseconds(100)));

    EXPECT_TRUE(timeout.isExpired(mClock));
    EXPECT_EQ(Timeout::expired, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldRestartFromStopped)
{
    Timeout timeout;

    timeout.restart(mClock, cobc::time::Milliseconds(100));

    mClock.setTime(SpacecraftElapsedTime::afterEpoch(cobc::time::Milliseconds(100)));

    EXPECT_TRUE(timeout.isExpired(mClock));
    EXPECT_EQ(Timeout::expired, timeout.getState(mClock));
}

TEST_F(TimeoutTest, shouldRestartFromExpired)
{
    Timeout timeout(mClock, cobc::time::Milliseconds(100));
    mClock.setTime(SpacecraftElapsedTime::afterEpoch(cobc::time::Milliseconds(100)));
    timeout.isExpired(mClock);

    timeout.restart(mClock, cobc::time::Milliseconds(200));
    EXPECT_FALSE(timeout.isExpired(mClock));
    EXPECT_EQ(Timeout::armed, timeout.getState(mClock));
}
