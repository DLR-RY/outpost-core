/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include <outpost/utils/storage/bitorder.h>

#include <unittest/harness.h>

using outpost::BitorderMsb0ToLsb0;

TEST(BitorderTest, convert16Bit)
{
    typedef BitorderMsb0ToLsb0<uint16_t, 0, 15> Converter;

    EXPECT_EQ(0U, static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(16U, static_cast<size_t>(Converter::width));
    EXPECT_EQ(15U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(0U, static_cast<size_t>(Converter::end));
}

TEST(BitorderTest, convertWithOffset)
{
    typedef BitorderMsb0ToLsb0<uint16_t, 13, 21> Converter;

    EXPECT_EQ(1U, static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(9U, static_cast<size_t>(Converter::width));
    EXPECT_EQ(10U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(2U, static_cast<size_t>(Converter::end));
}

TEST(BitorderTest, convertWithBigOffset)
{
    typedef BitorderMsb0ToLsb0<uint16_t, 150, 154> Converter;

    EXPECT_EQ(18U, static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(5U, static_cast<size_t>(Converter::width));
    EXPECT_EQ(9U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(5U, static_cast<size_t>(Converter::end));
}

TEST(BitorderTest, convertWithOffset32Bit)
{
    typedef BitorderMsb0ToLsb0<uint32_t, 13, 21> Converter;

    EXPECT_EQ(1U, static_cast<size_t>(Converter::byteIndex));
    EXPECT_EQ(9U, static_cast<size_t>(Converter::width));
    EXPECT_EQ(26U, static_cast<size_t>(Converter::start));
    EXPECT_EQ(18U, static_cast<size_t>(Converter::end));
}
