/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

/**
 * \file    harness.h
 * \brief    Test harness
 *
 * \author    Fabian Greif
 */

#ifndef COBC_HARNESS_H
#define COBC_HARNESS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cobc/utils.h>

/**
 * Compare two arrays.
 *
 * Example:
 *   uint8_t expect[16];
 *   uint8_t result[16];
 *   ...
 *   EXPECT_ARRAY_EQ(uint8_t, expect, result, 16);
 */
#define EXPECT_ARRAY_EQ(type, reference, actual, element_count)             \
    do {                                                                    \
        const type* reference_ = static_cast<const type *> (reference);        \
        const type* actual_ = static_cast<const type *> (actual);            \
        for (size_t i = 0; i < element_count; ++i ) {                    \
            EXPECT_EQ(reference_[i], actual_[i]);                            \
        }                                                                    \
    } while (0)

#define ARRAY_SIZE(a)    (sizeof(a)/sizeof(a[0]))

#endif // COBC_HARNESS_H
