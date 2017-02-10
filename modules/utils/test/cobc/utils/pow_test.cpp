/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#include <stdint.h>

#include <unittest/harness.h>

#include <cobc/utils/pow.h>

using namespace cobc;

TEST(PowerOfTwoTest, test)
{
    EXPECT_EQ(8, cobc::PowerOfTwo<3>::value);
    EXPECT_EQ(256*256*256, cobc::PowerOfTwo<24>::value);
}

TEST(PowTest, test)
{
    int32_t value;

    value = cobc::Pow<2, 3>::value;
    EXPECT_EQ(8, value);

    value = cobc::Pow<2, 0>::value;
    EXPECT_EQ(1, value);

    value = cobc::Pow<2, -1>::value;
    EXPECT_EQ(0, value);

    value = cobc::Pow<3, 4>::value;
    EXPECT_EQ(81, value);
}
