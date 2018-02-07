/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include <outpost/utils/container/member_function_store.h>

#include <unittest/harness.h>

using namespace outpost;

struct TestClass
{
    TestClass() : called1(0), called2(0), called3(0), called4(0), called5(0)
    {
    }

    void
    function1()
    {
        called1++;
    }

    void
    function2()
    {
        called2++;
    }

    void
    function3(int)
    {
        called3++;
    }

    int
    function4(void)
    {
        called4++;
        return 17;
    }

    int
    function5(bool, int)
    {
        called5++;
        return 42;
    }

    int called1;
    int called2;
    int called3;
    int called4;
    int called5;
};

TEST(MemberFunctionStoreTest, shouldRegisterFunction)
{
    TestClass test;

    MemberFunctionStore<3, void(void)> store;

    store.registerFunction(0, &test, &TestClass::function1);

    EXPECT_EQ(0, test.called1);
    store.callFunction(0);
    EXPECT_EQ(1, test.called1);
}

TEST(MemberFunctionStoreTest, shouldRegisterMultipleFunctions)
{
    TestClass test;

    MemberFunctionStore<3, void(void)> store;

    store.registerFunction(0, &test, &TestClass::function1);
    store.registerFunction(1, &test, &TestClass::function2);
    store.registerFunction(2, &test, &TestClass::function1);

    store.callFunction(0);
    store.callFunction(1);
    store.callFunction(2);

    EXPECT_EQ(2, test.called1);
    EXPECT_EQ(1, test.called2);
    EXPECT_EQ(0, test.called3);
}

TEST(MemberFunctionStoreTest, shouldAllowFunctionsWithArguments)
{
    TestClass test;

    MemberFunctionStore<3, void(int)> store;

    store.registerFunction(0, &test, &TestClass::function3);

    EXPECT_EQ(0, test.called3);
    store.callFunction<void(int)>(0, 2);
    EXPECT_EQ(1, test.called3);
}

TEST(MemberFunctionStoreTest, shouldAllowReturnValues)
{
    TestClass test;

    MemberFunctionStore<3, int(void)> store;

    store.registerFunction(2, &test, &TestClass::function4);

    EXPECT_EQ(0, test.called4);
    EXPECT_EQ(17, store.callFunction(2));
    EXPECT_EQ(1, test.called4);
}

TEST(MemberFunctionStoreTest, shouldAllowMultipleArguments)
{
    TestClass test;

    MemberFunctionStore<3, int(bool, int)> store;

    store.registerFunction(1, &test, &TestClass::function5);

    EXPECT_EQ(0, test.called5);
    EXPECT_EQ(42, store.callFunction<int(bool, int)>(1, true, 100));
    EXPECT_EQ(1, test.called5);
}
