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
    accessTimes(size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            GetParam()->access(mClock.now());
        }
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
    accessTimes(quotaLimit);

    EXPECT_FALSE(GetParam()->access(mClock.now()));
}

TEST_P(QuotaTest, shouldAllowAccessAgainAfterTimeLimit)
{
    accessTimes(quotaLimit);

    mClock.setTime(mClock.now() + defaultInterval);

    EXPECT_TRUE(GetParam()->access(mClock.now()));
}

INSTANTIATE_TEST_CASE_P(
        QuotaTests,
        QuotaTest,
        ::testing::Values(
                new ContinuousIntervalQuota<QuotaTest::quotaLimit>(QuotaTest::defaultInterval),
                new NonDeterministicIntervalQuota(QuotaTest::defaultInterval,
                                                  QuotaTest::quotaLimit)));
