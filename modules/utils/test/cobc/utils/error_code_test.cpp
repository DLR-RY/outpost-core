/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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
 * \file
 * \brief   Unittests for \c cobc::ErrorCode.
 *
 * \author  Fabian Greif
 */
#include <unittest/harness.h>
#include <cobc/utils/error_code.h>

TEST(ErrorCode, success)
{
    cobc::ErrorCode result = cobc::ErrorCode::success();

    EXPECT_EQ(0, result.getCode());
    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ErrorCode, successCode)
{
    cobc::ErrorCode result = cobc::ErrorCode::success(2);

    EXPECT_EQ(2, result.getCode());
    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ErrorCode, error)
{
    cobc::ErrorCode result = cobc::ErrorCode::error();

    EXPECT_EQ(-1, result.getCode());
    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.isError());
}

TEST(ErrorCode, errorCode)
{
    cobc::ErrorCode result = cobc::ErrorCode::error(-3);

    EXPECT_EQ(-3, result.getCode());
    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_FALSE(result.isSuccess());
    EXPECT_TRUE(result.isError());
}

TEST(ErrorCode, equality)
{
    cobc::ErrorCode result  = cobc::ErrorCode::error(-3);
    cobc::ErrorCode result2 = cobc::ErrorCode::error(-2);

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
    cobc::ErrorCode result = cobc::ErrorCode::error(-3);

    cobc::ErrorCode result2(result);

    EXPECT_EQ(-3, result2.getCode());
    EXPECT_FALSE(static_cast<bool>(result2));
    EXPECT_FALSE(result2.isSuccess());
    EXPECT_TRUE(result2.isError());

    EXPECT_TRUE(result2 == result);
    EXPECT_FALSE(result2 != result);
}

class SomeErrorCode : public cobc::ErrorCode
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
    SomeErrorCode(int16_t code) :
        cobc::ErrorCode(code)
    {
    }
};

/**
 * Test shows how to extended the error code class by sub-classing it and
 * providing additional static constructor methods.
 */
TEST(ErrorCode, inheritance)
{
    cobc::ErrorCode result = SomeErrorCode::error1();

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
    cobc::ErrorCode result2 = result;

    EXPECT_TRUE(result == result2);

    cobc::ErrorCode result3 = SomeErrorCode::error1();

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
