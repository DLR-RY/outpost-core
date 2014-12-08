/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
// ----------------------------------------------------------------------------

#include <unittest/harness.h>

#include <cobc/utils/bitorder.h>

using cobc::BitorderConverter;

TEST(BitorderTest, convert16Bit)
{
    size_t start = BitorderConverter<uint16_t, 0, 15>::start;
    size_t end   = BitorderConverter<uint16_t, 0, 15>::end;
    EXPECT_EQ(15U, start);
    EXPECT_EQ(0U, end);

    // 0..15, offset 0 -> 15..0
    // 8..23, offset 8 -> 15..0
    // 2..4,  offset 0 -> 13..11

    //      MSB
    // MSB0   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    // LSB0  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0


}
