/*
 * Copyright (c) 2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Tobias Pfeffer (DLR RY-AVS)
 */

#include <outpost/utils/functor.h>

#include <unittest/harness.h>

#include <stdint.h>

using namespace outpost;

struct Testclass
{
    bool
    trueFunc()
    {
        ++called;
        return true;
    }

    int
    intPtrFunc(const int* var)
    {
        ++called;
        return *var + 2;
    }

    int
    sumFunc(int a, int b, int c)
    {
        ++called;
        return a + b + c;
    }

    void
    voidOutParamFunc(int* a)
    {
        ++called;
        *a = 42;
    }

    bool
    boolParamFunc(bool someBool)
    {
        ++called;
        return someBool;
    }

    int called = 0;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

TEST(Functor, testLegacy)
{
    int called = 0;

    Testclass testObject;
    EXPECT_EQ(testObject.called, called);

    outpost::Functor<bool()> functor(testObject, &Testclass::trueFunc);
    EXPECT_TRUE(functor.execute());
    EXPECT_EQ(testObject.called, ++called);

    EXPECT_TRUE(functor());
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<int(const int*)> second(testObject, &Testclass::intPtrFunc);
    int a = 8;
    EXPECT_EQ(second.execute(&a), 10);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<int(int, int, int)> sum(testObject, &Testclass::sumFunc);
    EXPECT_EQ(sum(1, 2, 3), 6);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<int(int, int, int)> invalFunc;
    EXPECT_TRUE(invalFunc.isEmpty());
    EXPECT_EQ(testObject.called, called);

    outpost::Functor<void(int*)> voidFunc(testObject, &Testclass::voidOutParamFunc);
    voidFunc(&a);
    EXPECT_EQ(a, 42);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<bool(bool)> boolFunc(testObject, &Testclass::boolParamFunc);
    EXPECT_TRUE(boolFunc(true));
    EXPECT_EQ(testObject.called, ++called);
    bool testBool = false;
    EXPECT_FALSE(boolFunc(testBool));
    EXPECT_EQ(testObject.called, ++called);
}

#pragma GCC diagnostic pop  // -Wdeprecated-declarations

TEST(Functor, testChecked)
{
    int called = 0;

    Testclass testObject;
    EXPECT_EQ(testObject.called, called);

    outpost::utils::OperationResult valid = outpost::utils::OperationResult::invalid;
    outpost::Functor<bool()> functor(testObject, &Testclass::trueFunc);
    EXPECT_TRUE(functor.execute(valid));
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(testObject.called, ++called);

    // operator dispatched by additional arg compared to signature
    valid = outpost::utils::OperationResult::invalid;
    EXPECT_TRUE(functor(valid));
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<int(const int*)> second(testObject, &Testclass::intPtrFunc);
    valid = outpost::utils::OperationResult::invalid;
    int a = 8;
    a = second.execute(valid, &a);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(a, 10);
    EXPECT_EQ(testObject.called, ++called);

    valid = outpost::utils::OperationResult::invalid;
    a = second(valid, &a);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(a, 12);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<int(int, int, int)> sum(testObject, &Testclass::sumFunc);
    valid = outpost::utils::OperationResult::invalid;
    int b = sum(valid, 1, 2, 3);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(b, 6);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<int(int, int, int)> invalFunc;
    valid = outpost::utils::OperationResult::invalid;
    invalFunc(valid, 1, 2, 3);
    EXPECT_EQ(valid, outpost::utils::OperationResult::invalid);
    EXPECT_EQ(testObject.called, called);

    outpost::Functor<void(int*)> voidFunc(testObject, &Testclass::voidOutParamFunc);
    valid = outpost::utils::OperationResult::invalid;
    voidFunc(valid, &a);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(a, 42);
    EXPECT_EQ(testObject.called, ++called);

    outpost::Functor<bool(bool)> boolFunc(testObject, &Testclass::boolParamFunc);
    valid = outpost::utils::OperationResult::invalid;
    EXPECT_TRUE(boolFunc(valid, true));
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(testObject.called, ++called);
    valid = outpost::utils::OperationResult::invalid;
    bool testBool = false;
    EXPECT_FALSE(boolFunc(valid, testBool));
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(testObject.called, ++called);
}
