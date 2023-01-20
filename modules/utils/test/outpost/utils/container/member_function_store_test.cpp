/*
 * Copyright (c) 2016-2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 * - 2022, Felix Passenberg (DLR RY-AVS)
 * - 2022, Tobias Pfeffer (DLR RY-AVS)
 */

#include <outpost/base/slice.h>
#include <outpost/utils/container/member_function_store.h>

#include <unittest/harness.h>

using namespace outpost;

struct TestStruct
{
    int value{};
};

class TestClass
{
public:
    explicit TestClass(int v) : mValue(v)
    {
    }

    int mValue{};
};

struct TestCallable
{
    TestCallable() = default;

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
    function5(bool b, int i)
    {
        called5++;
        return b ? i : 42;
    }

    int
    function6(void)
    {
        called6++;
        return 42;
    }

    int
    function7(bool b, int i, int j)
    {
        called7++;
        return b ? i : j;
    }

    int
    function8(bool b, int i, double j)
    {
        called8++;
        return b ? i : j;
    }

    int
    function9(bool b, int i, double j, int a)
    {
        called9++;
        return b ? i + a : j;
    }

    int
    function10(TestStruct tstruct, TestClass tclass, outpost::Slice<const int> slice)
    {
        called10++;
        return tstruct.value + tclass.mValue + slice[0] + slice[1];
    }

    bool
    function11(bool value)
    {
        called11++;
        return value;
    }

    int called1{};
    int called2{};
    int called3{};
    int called4{};
    int called5{};
    int called6{};
    int called7{};
    int called8{};
    int called9{};
    int called10{};
    int called11{};
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

TEST(MemberFunctionStoreTest, shouldRegisterFunction)
{
    TestCallable test;

    MemberFunctionStore<3, void(void)> store;

    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function1));

    EXPECT_EQ(0, test.called1);
    store.callFunction(0);
    EXPECT_EQ(1, test.called1);
}

TEST(MemberFunctionStoreTest, shouldReplaceCorrectly)
{
    TestCallable test;

    MemberFunctionStore<3, void(void)> store;

    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function1));
    // default: replace == true
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function2));

    EXPECT_EQ(0, test.called1);
    EXPECT_EQ(0, test.called2);
    store.callFunction(0);
    EXPECT_EQ(0, test.called1);
    EXPECT_EQ(1, test.called2);

    EXPECT_EQ(outpost::utils::OperationResult::inUse,
              store.registerFunction(0, test, &TestCallable::function1, false));

    EXPECT_EQ(0, test.called1);
    EXPECT_EQ(1, test.called2);
    store.callFunction(0);
    EXPECT_EQ(0, test.called1);
    EXPECT_EQ(2, test.called2);
}

TEST(MemberFunctionStoreTest, shouldRegisterMultipleFunctions)
{
    TestCallable test;

    MemberFunctionStore<3, void(void)> store;

    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function1));
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(1, test, &TestCallable::function2));
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(2, test, &TestCallable::function1));

    store.callFunction(0);
    store.callFunction(1);
    store.callFunction(2);

    EXPECT_EQ(2, test.called1);
    EXPECT_EQ(1, test.called2);
    EXPECT_EQ(0, test.called3);
}

TEST(MemberFunctionStoreTest, shouldAllowFunctionsWithArguments)
{
    TestCallable test;

    MemberFunctionStore<3, void(int)> store;

    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function3));

    EXPECT_EQ(0, test.called3);
    store.callFunction(0, 2);
    EXPECT_EQ(1, test.called3);
}

TEST(MemberFunctionStoreTest, shouldAllowReturnValues)
{
    TestCallable test;

    MemberFunctionStore<3, int(void)> store;

    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(2, test, &TestCallable::function4));

    EXPECT_EQ(0, test.called4);
    EXPECT_EQ(17, store.callFunction(2));
    EXPECT_EQ(1, test.called4);
}

// disable implicit conversion warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-conversion"
TEST(MemberFunctionStoreTest, shouldAllowMultipleArguments)
{
    TestCallable test;

    MemberFunctionStore<3, int(bool, int)> store;

    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(1, test, &TestCallable::function5));
    // store.registerFunction(2, test, &TestClass::function4); // must fail, wrong signature

    EXPECT_EQ(0, test.called5);
    EXPECT_EQ(100, store.callFunction(1, true, 100));
    EXPECT_EQ(1, test.called5);
    EXPECT_EQ(42, store.callFunction(1, false, 100));
    EXPECT_EQ(2, test.called5);
    EXPECT_EQ(4, store.callFunction(1, true, 4.2));  // implicit conversion
    EXPECT_EQ(3, test.called5);
    // store.callFunction(1, true, "notgood"); // fails, does not convert implicitly

    // non-const bool test
    bool condition = false;
    EXPECT_EQ(42, store.callFunction(1, condition, 100));
    EXPECT_FALSE(condition);
}
#pragma GCC diagnostic pop  // -Wliteral-conversion

TEST(MemberFunctionStoreTest, shouldNotSegfault)
{
    TestCallable test;

    MemberFunctionStore<3, int(void)> store;
    EXPECT_EQ(0, test.called4);
    EXPECT_EQ(0, test.called6);

    // register a function
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function4));

    EXPECT_EQ(17, store.callFunction(0));
    EXPECT_EQ(1, test.called4);
    EXPECT_EQ(0, test.called6);
    EXPECT_EQ(0, store.callFunction(1));   // must not segfault (not initialized storage)
    EXPECT_EQ(0, store.callFunction(42));  // must not segfault (out of range access)

    // out of range -> do nothing
    EXPECT_EQ(outpost::utils::OperationResult::outOfBounds,
              store.registerFunction(4, test, &TestCallable::function6));

    // overwrite location
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function6));

    EXPECT_EQ(42, store.callFunction(0));
    EXPECT_EQ(1, test.called4);
    EXPECT_EQ(1, test.called6);
}

TEST(MemberFunctionStoreTest, variableArguments)
{
    TestCallable test;

    MemberFunctionStore<1, int(bool, int, int)> store;

    // register a function
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function7));

    EXPECT_EQ(0, test.called7);
    EXPECT_EQ(5, store.callFunction(0, true, 5, 18));
    EXPECT_EQ(1, test.called7);
    EXPECT_EQ(18, store.callFunction(0, false, 5, 18));
    EXPECT_EQ(2, test.called7);

    // mismatched signatures, does not compile
    // store.registerFunction(0, test, &TestClass::function6);
    // store.registerFunction(0, test, &TestClass::function8);
    // store.registerFunction(0, test, &TestClass::function9);

    MemberFunctionStore<1, int(bool, int, double, int)> store2;
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store2.registerFunction(0, test, &TestCallable::function9));

    // mismatched signatures, does not compile
    // store2.registerFunction(0, test, &TestClass::function6);
    // store2.registerFunction(0, test, &TestClass::function7);
    // store2.registerFunction(0, test, &TestClass::function8);
}

TEST(MemberFunctionStoreTest, complexArguments)
{
    TestCallable test;

    MemberFunctionStore<1, int(TestStruct, TestClass, outpost::Slice<const int>)> store;

    // register a function
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function10));

    TestStruct tstruct{3};
    TestClass tclass(42);
    std::array<int, 2> arr{{1, 2}};
    outpost::Slice<int> slice(arr);

    EXPECT_EQ(0, test.called10);
    EXPECT_EQ(48, store.callFunction(0, tstruct, tclass, slice));
    EXPECT_EQ(1, test.called10);
}

TEST(MemberFunctionStoreTest, checkBoolFunctionDispatch)
{
    TestCallable test;

    MemberFunctionStore<1, bool(bool)> store;
    EXPECT_EQ(0, test.called11);
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function11));

    // dispatched to unchecked version, const param
    bool ret = store.callFunction(0, true);
    EXPECT_EQ(1, test.called11);
    EXPECT_TRUE(ret);

    // dispatched to unchecked version, non-const param
    bool input = false;
    ret = store.callFunction(0, input);
    EXPECT_EQ(2, test.called11);
    EXPECT_FALSE(ret);
    EXPECT_FALSE(input);

    // dispatched to checked version due to extra parameter
    outpost::utils::OperationResult valid;
    ret = store.callFunction(0, valid, true);
    EXPECT_EQ(3, test.called11);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_TRUE(ret);
}

#pragma GCC diagnostic pop  // -Wdeprecated-declarations

TEST(MemberFunctionStoreTest, checkParameter)
{
    TestCallable test;

    MemberFunctionStore<3, int(void)> store;
    EXPECT_EQ(0, test.called4);
    EXPECT_EQ(0, test.called6);

    // register a function
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function4));

    outpost::utils::OperationResult valid;
    int ret = store.callFunction(0, valid);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(17, ret);
    EXPECT_EQ(1, test.called4);
    EXPECT_EQ(0, test.called6);
    ret = store.callFunction(1, valid);
    EXPECT_EQ(valid, outpost::utils::OperationResult::invalid);
    EXPECT_EQ(0, ret);  // must not segfault (not initialized storage)
    ret = store.callFunction(42, valid);
    EXPECT_EQ(valid, outpost::utils::OperationResult::outOfBounds);
    EXPECT_EQ(0, ret);  // must not segfault (out of range access)

    // out of range -> do nothing
    EXPECT_EQ(outpost::utils::OperationResult::outOfBounds,
              store.registerFunction(4, test, &TestCallable::function6));

    // overwrite location
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function6));

    ret = store.callFunction(0, valid);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(42, ret);
    EXPECT_EQ(1, test.called4);
    EXPECT_EQ(1, test.called6);
}

TEST(MemberFunctionStoreTest, checkParameterDispatch)
{
    TestCallable test;

    MemberFunctionStore<1, int(bool, int)> store;
    EXPECT_EQ(0, test.called5);
    EXPECT_EQ(outpost::utils::OperationResult::success,
              store.registerFunction(0, test, &TestCallable::function5));

    // dispatched to checked version due to extra parameter
    outpost::utils::OperationResult valid;
    int ret = store.callFunction(0, valid, true, 10);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(ret, 10);

    ret = store.callFunction(0, valid, false, 10);
    EXPECT_EQ(valid, outpost::utils::OperationResult::success);
    EXPECT_EQ(ret, 42);

    ret = store.callFunction(1, valid, true, 10);
    EXPECT_EQ(valid, outpost::utils::OperationResult::outOfBounds);
    EXPECT_EQ(ret, 0);
}
