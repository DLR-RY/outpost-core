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
 * \file
 * \brief   Unittests for \c outpost::ErrorCode.
 *
 * \author  Fabian Greif
 */
#include <outpost/utils/error_code.h>

#include <unittest/harness.h>

TEST(ErrorCode, success)
{
    outpost::ErrorCode result = outpost::ErrorCode::success();

    EXPECT_EQ(0, result.getCode());
    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ErrorCode, successCode)
{
    outpost::ErrorCode result = outpost::ErrorCode::success(2);

    EXPECT_EQ(2, result.getCode());
    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ErrorCode, error)
{
    outpost::ErrorCode result = outpost::ErrorCode::error();

    EXPECT_EQ(-1, result.getCode());
    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.isError());
}

TEST(ErrorCode, errorCode)
{
    outpost::ErrorCode result = outpost::ErrorCode::error(-3);

    EXPECT_EQ(-3, result.getCode());
    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.isError());
}

TEST(ErrorCode, equality)
{
    outpost::ErrorCode result = outpost::ErrorCode::error(-3);
    outpost::ErrorCode result2 = outpost::ErrorCode::error(-2);

    EXPECT_TRUE(result != result2);
    EXPECT_FALSE(result == result2);
    EXPECT_FALSE(result);
    EXPECT_FALSE(result2);
}

/**
 * Test that the error code can be copied.
 */
TEST(ErrorCode, copy)
{
    outpost::ErrorCode result = outpost::ErrorCode::error(-3);

    outpost::ErrorCode result2(result);

    EXPECT_EQ(-3, result2.getCode());
    EXPECT_FALSE(static_cast<bool>(result2));
    EXPECT_FALSE(result2.isSuccess());
    EXPECT_TRUE(result2.isError());

    EXPECT_TRUE(result2 == result);
    EXPECT_FALSE(result2 != result);
}

class SomeErrorCode : public outpost::ErrorCode
{
public:
    static SomeErrorCode
    error1()
    {
        return SomeErrorCode(-2);
    }

    static SomeErrorCode
    error2()
    {
        return SomeErrorCode(-3);
    }

protected:
    explicit SomeErrorCode(int16_t code) : outpost::ErrorCode(code)
    {
    }
};

/**
 * Test shows how to extended the error code class by sub-classing it and
 * providing additional static constructor methods.
 */
TEST(ErrorCode, inheritance)
{
    outpost::ErrorCode result = SomeErrorCode::error1();

    EXPECT_EQ(-2, result.getCode());
    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.isError());

    EXPECT_TRUE(result == SomeErrorCode::error1());
    EXPECT_FALSE(result == SomeErrorCode::error2());
    EXPECT_TRUE(result != SomeErrorCode::error2());
    EXPECT_FALSE(result != SomeErrorCode::error1());
}

/**
 * Test that is possible to cast a sub-class to the generic error code class.
 */
TEST(ErrorCode, inheritanceUpCasting)
{
    SomeErrorCode result = SomeErrorCode::error1();

    // cast to the generic error class
    outpost::ErrorCode result2 = result;

    EXPECT_TRUE(result == result2);

    outpost::ErrorCode result3 = SomeErrorCode::error1();

    EXPECT_TRUE(result == result3);
}

/**
 * Test that subclasses are comparable.
 */
TEST(ErrorCode, inheritanceComparision)
{
    SomeErrorCode a = SomeErrorCode::error1();
    SomeErrorCode b = SomeErrorCode::error1();
    SomeErrorCode c = SomeErrorCode::error2();

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a == c);
}
