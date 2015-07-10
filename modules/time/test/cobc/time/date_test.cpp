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

TEST(DateTest, shouldDetectValidDates)
{
	Date date;

	date = Date { 1980, 2, 28, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1980, 2, 29, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1980, 3, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1992, 1, 1, 12, 12, 12 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2000, 1, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2000, 6, 1, 23, 59, 58 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1989, 12, 31, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1990, 1, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2005, 12, 31, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2006, 1, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2015, 6, 30, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2015, 7, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());
}
