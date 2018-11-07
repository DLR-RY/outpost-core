/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

/**
 * \file    harness.h
 * \brief    Test harness
 *
 * \author    Fabian Greif
 */

#ifndef OUTPOST_HARNESS_H
#define OUTPOST_HARNESS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

/**
 * Compare two arrays.
 *
 * Example:
 *   uint8_t expect[16];
 *   uint8_t result[16];
 *   ...
 *   EXPECT_ARRAY_EQ(uint8_t, expect, result, 16);
 */
#define EXPECT_ARRAY_EQ(type, reference, actual, element_count)       \
    do                                                                \
    {                                                                 \
        const type* reference_ = static_cast<const type*>(reference); \
        const type* actual_ = static_cast<const type*>(actual);       \
        for (size_t i = 0; i < element_count; ++i)                    \
        {                                                             \
            EXPECT_EQ(reference_[i], actual_[i]);                     \
        }                                                             \
    } while (0)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif
