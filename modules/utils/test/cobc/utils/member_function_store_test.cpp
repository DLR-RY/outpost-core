/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include <cobc/utils/member_function_store.h>

using namespace cobc;

struct TestClass
{
    TestClass() :
        called1(0),
        called2(0),
        called3(0)
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
    function3()
    {
        called3++;
    }

    int called1;
    int called2;
    int called3;
};

TEST(MemberFunctionStoreTest, shouldRegisterFunction)
{
    TestClass test;

    MemberFunctionStore<3> store;

    store.registerFunction(0, &test, &TestClass::function1);

    EXPECT_EQ(0, test.called1);
    store.callFunction(0);
    EXPECT_EQ(1, test.called1);
}

TEST(MemberFunctionStoreTest, shouldRegisterMultipleFunctions)
{
    TestClass test;

    MemberFunctionStore<3> store;

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
