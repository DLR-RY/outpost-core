/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#include <outpost/utils/bitorder.h>

using outpost::BitorderMsb0ToLsb0;

TEST(BitorderTest, convert16Bit)
{
    typedef BitorderMsb0ToLsb0<uint16_t, 0, 15> Converter;

    EXPECT_EQ(0U,  static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(16U, static_cast<size_t>(Converter::width));
    EXPECT_EQ(15U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(0U,  static_cast<size_t>(Converter::end));
}

TEST(BitorderTest, convertWithOffset)
{
    typedef BitorderMsb0ToLsb0<uint16_t, 13, 21> Converter;

    EXPECT_EQ(1U,  static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(9U,  static_cast<size_t>(Converter::width));
    EXPECT_EQ(10U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(2U,  static_cast<size_t>(Converter::end));
}

TEST(BitorderTest, convertWithBigOffset)
{
    typedef BitorderMsb0ToLsb0<uint16_t, 150, 154> Converter;

    EXPECT_EQ(18U,  static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(5U,  static_cast<size_t>(Converter::width));
    EXPECT_EQ(9U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(5U,  static_cast<size_t>(Converter::end));
}

TEST(BitorderTest, convertWithOffset32Bit)
{
    typedef BitorderMsb0ToLsb0<uint32_t, 13, 21> Converter;

    EXPECT_EQ(1U,  static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(9U,  static_cast<size_t>(Converter::width));
    EXPECT_EQ(26U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(18U,  static_cast<size_t>(Converter::end));
}
