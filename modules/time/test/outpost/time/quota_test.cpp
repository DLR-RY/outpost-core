/*
 * Copyright (c) 2015, 2017-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015, 2017-2018, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/time/quota.h>

#include <unittest/harness.h>
#include <unittest/time/testing_clock.h>

using namespace outpost::time;

class QuotaTest : public ::testing::TestWithParam<Quota*>
{
public:
    static constexpr size_t quotaLimit = 5;
    static constexpr Duration defaultInterval = Seconds(1);

    QuotaTest() : mClock()
    {
    }

    void
    SetUp() override
    {
        GetParam()->reset();
    }

    bool
    accessTimes(size_t n)
    {
        bool allPassed = true;
        for (size_t i = 0; i < n; ++i)
        {
            allPassed &= GetParam()->access(mClock.now());
        }
        return allPassed;
    }

    unittest::time::TestingClock mClock;
};

constexpr Duration QuotaTest::defaultInterval;

TEST_P(QuotaTest, shouldAllowAccess)
{
    EXPECT_TRUE(GetParam()->access(mClock.now()));
}

TEST_P(QuotaTest, shouldBlockAfterReachingTheLimit)
{
    EXPECT_TRUE(accessTimes(quotaLimit));

    EXPECT_FALSE(GetParam()->access(mClock.now()));
}

TEST_P(QuotaTest, shouldAllowAccessAgainAfterTimeLimit)
{
    EXPECT_TRUE(accessTimes(quotaLimit));

    EXPECT_FALSE(GetParam()->access(mClock.now()));

    mClock.incrementBy(defaultInterval);

    EXPECT_TRUE(GetParam()->access(mClock.now()));
}

TEST_P(QuotaTest, shouldStillBlockAfterHalfTimeLimit)
{
    EXPECT_TRUE(accessTimes(quotaLimit));

    EXPECT_FALSE(GetParam()->access(mClock.now()));

    mClock.incrementBy(defaultInterval / 2);

    EXPECT_FALSE(GetParam()->access(mClock.now()));
}

TEST_P(QuotaTest, setInterval)
{
    GetParam()->setTimeInterval(2 * defaultInterval);

    EXPECT_TRUE(accessTimes(quotaLimit));

    EXPECT_FALSE(GetParam()->access(mClock.now()));

    mClock.incrementBy(defaultInterval);

    EXPECT_FALSE(GetParam()->access(mClock.now()));

    mClock.incrementBy(defaultInterval);

    EXPECT_TRUE(GetParam()->access(mClock.now()));
}

TEST_P(QuotaTest, resetting)
{
    EXPECT_TRUE(accessTimes(quotaLimit));

    EXPECT_FALSE(GetParam()->access(mClock.now()));

    GetParam()->reset();

    EXPECT_TRUE(accessTimes(quotaLimit));

    EXPECT_FALSE(GetParam()->access(mClock.now()));
}

static outpost::time::ContinuousIntervalQuota<QuotaTest::quotaLimit>
        CIQ(QuotaTest::defaultInterval);
static outpost::time::NonDeterministicIntervalQuota
        NDIQ(QuotaTest::defaultInterval, QuotaTest::quotaLimit);

INSTANTIATE_TEST_SUITE_P(QuotaTests, QuotaTest, ::testing::Values(&CIQ, &NDIQ));

TEST(UnlimitedQuotaTest, UnlimitedQuotaTest)
{
    unittest::time::TestingClock clock;
    UnlimitedQuota quota;

    // always allowed
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));

    // set interval has no effect
    quota.setTimeInterval(outpost::time::Seconds(1000));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));

    // reset has no effect
    quota.reset();
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
    EXPECT_TRUE(quota.access(clock.now()));
}
