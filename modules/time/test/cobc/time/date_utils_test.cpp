/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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
#include <cobc/time/date.h>

using namespace cobc::time;

TEST(DateUtilsTest, shouldHaveAStartingEpoch)
{
    Date date = DateUtils::getDate(0);

    EXPECT_EQ(0, date.year);
    EXPECT_EQ(3, date.month);
    EXPECT_EQ(1, date.day);
}

TEST(DateUtilsTest, shouldCalculateTheUnixEpochOffset)
{
	const Date unixEpochStart = { 1970, 1, 1, 0, 0, 0 };
    EXPECT_EQ(719468, DateUtils::getDay(unixEpochStart));
}
