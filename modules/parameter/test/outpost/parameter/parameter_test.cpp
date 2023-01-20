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
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/parameter/parameter.h>
#include <outpost/time.h>

#include <unittest/harness.h>

#include <stdint.h>

#include <set>

using namespace outpost::parameter;

TEST(ParameterTest, defaultconstructor)
{
    outpost::time::SpacecraftElapsedTime time;
    Parameter<uint32_t> par;

    EXPECT_EQ(false, par.isInitialized());
    EXPECT_EQ(false, par.hasInvalidIdAssigned());

    // after constructor all functions beside initialize and compare operator should result in
    // invalid state
    uint32_t id = 0;
    EXPECT_EQ(ParameterBase::invalidID, par.getID());
    uint32_t data;
    EXPECT_EQ(OperationResult::notInitialized, par.getValue(data, nullptr));
    data = 10;
    EXPECT_EQ(OperationResult::notInitialized, par.setValue(data, time));

    id = ParameterBase::invalidID;
    // a invalid ID should fail init
    EXPECT_EQ(OperationResult::invalidParameter, par.initialize(id, data, time));
    EXPECT_EQ(true, par.hasInvalidIdAssigned());

    id = ParameterBase::invalidID + 1;
    // valid ID should work
    time = time + outpost::time::Seconds(1);
    EXPECT_EQ(OperationResult::success, par.initialize(id, data, time));

    EXPECT_EQ(true, par.isInitialized());
    EXPECT_EQ(false, par.hasInvalidIdAssigned());

    // should be the correct type
    EXPECT_EQ(Type::getType<uint32_t>(), par.getType());

    // the initial value should be correct
    uint32_t returnedData;
    outpost::time::SpacecraftElapsedTime returnedTime;
    EXPECT_EQ(OperationResult::success, par.getValue(returnedData, &returnedTime));
    EXPECT_EQ(returnedData, data);
    EXPECT_EQ(returnedTime, time);

    // write new data
    data = data + 1;
    time = time + outpost::time::Seconds(1);
    EXPECT_EQ(OperationResult::success, par.setValue(data, time));

    // test result, also check that nullptr works
    EXPECT_EQ(OperationResult::success, par.getValue(returnedData, nullptr));
    EXPECT_EQ(returnedData, data);

    // a few more writes
    for (int i = 0; i < 10; i++)
    {
        // write new data
        data = data + 1;
        time = time + outpost::time::Seconds(1);
        EXPECT_EQ(OperationResult::success, par.setValue(data, time));

        // test result
        EXPECT_EQ(OperationResult::success, par.getValue(returnedData, &returnedTime));
        EXPECT_EQ(returnedData, data);
        EXPECT_EQ(returnedTime, time);
    }
}

TEST(ParameterTest, initializeConstructor)
{
    outpost::time::SpacecraftElapsedTime time;
    // test invalid id results in an uninitialized one.
    Parameter<uint32_t> par(ParameterBase::invalidID, 1, time);

    EXPECT_EQ(false, par.isInitialized());
    EXPECT_EQ(true, par.hasInvalidIdAssigned());

    IDType id = 0;
    EXPECT_EQ(ParameterBase::invalidID, par.getID());
    uint32_t data;
    EXPECT_EQ(OperationResult::notInitialized, par.getValue(data, nullptr));
    data = 10;
    EXPECT_EQ(OperationResult::notInitialized, par.setValue(data, time));

    // check correct constructor
    id = ParameterBase::invalidID + 1;
    data = 11;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par2(id, data, time);

    EXPECT_EQ(true, par2.isInitialized());

    // should be the correct type
    EXPECT_EQ(Type::getType<uint32_t>(), par2.getType());

    // the initial value should be correct
    uint32_t returnedData;
    outpost::time::SpacecraftElapsedTime returnedTime;
    EXPECT_EQ(OperationResult::success, par2.getValue(returnedData, &returnedTime));
    EXPECT_EQ(returnedData, data);
    EXPECT_EQ(returnedTime, time);

    // the ID should be correct
    EXPECT_EQ(id, par2.getID());
}

namespace outpost
{
namespace parameter
{
ParameterBase*
getAnchor(const ParameterList& a)
{
    return a.anchor;
}

std::atomic_flag&
getFlag(ParameterBase& a)
{
    return a.mWriteInProgress;
}

}  // namespace parameter
}  // namespace outpost

TEST(ParameterTest, anchoredImplicitList)
{
    // don't use 0 as thats the default and we may got some from the previous test

    Parameter<uint32_t> par20(IndexedParameterList<2>::getList());
    Parameter<uint32_t> par21(IndexedParameterList<2>::getList());
    Parameter<uint32_t> par22(IndexedParameterList<2>::getList());

    Parameter<uint32_t> par50(IndexedParameterList<5>::getList());

    Parameter<uint32_t> par80(IndexedParameterList<8>::getList());
    Parameter<uint32_t> par81(IndexedParameterList<8>::getList());

    // not used anchor should be nullptr
    EXPECT_EQ(getAnchor(IndexedParameterList<1>::getList()), nullptr);

    // 5 should only include 50
    EXPECT_EQ(getAnchor(IndexedParameterList<5>::getList()), &par50);
    EXPECT_EQ(getAnchor(IndexedParameterList<5>::getList())->getNext(), nullptr);

    std::set<ParameterBase*> expected;
    expected.insert(&par20);
    expected.insert(&par21);
    expected.insert(&par22);
    expected.insert(nullptr);

    std::set<ParameterBase*> actual;

    for (auto it = getAnchor(IndexedParameterList<2>::getList()); it != nullptr; it = it->getNext())
    {
        actual.insert(it);
    }

    actual.insert(nullptr);
    EXPECT_EQ(actual, expected);

    expected.clear();
    expected.insert(&par80);
    expected.insert(&par81);
    expected.insert(nullptr);

    actual.clear();
    for (auto it = getAnchor(IndexedParameterList<8>::getList()); it != nullptr; it = it->getNext())
    {
        actual.insert(it);
    }

    actual.insert(nullptr);
    EXPECT_EQ(actual, expected);
}

TEST(ParameterTest, testConcurrentWrite)
{
    Parameter<uint32_t> par;
    outpost::time::SpacecraftElapsedTime time;
    IDType id = ParameterBase::invalidID + 1;
    uint32_t data = 10;

    EXPECT_EQ(OperationResult::success, par.initialize(id, data, time, true));

    // write without concurrent access should work
    data = data + 1;
    time = time + outpost::time::Minutes(1);
    EXPECT_EQ(OperationResult::success, par.setValue(data, time));

    uint32_t returnedData;
    outpost::time::SpacecraftElapsedTime returnedTime;

    // test result
    EXPECT_EQ(OperationResult::success, par.getValue(returnedData, &returnedTime));
    EXPECT_EQ(returnedData, data);
    EXPECT_EQ(returnedTime, time);

    uint32_t oldData = returnedData;
    outpost::time::SpacecraftElapsedTime oldtime = time;

    data = data + 1;
    time = time + outpost::time::Minutes(1);
    // set another write
    getFlag(par).test_and_set();
    EXPECT_EQ(OperationResult::concurrentWrite, par.setValue(data, time));

    // test result
    EXPECT_EQ(OperationResult::success, par.getValue(returnedData, &returnedTime));
    EXPECT_EQ(returnedData, oldData);
    EXPECT_EQ(returnedTime, oldtime);

    // once it is reset it should work again
    getFlag(par).clear();
    EXPECT_EQ(OperationResult::success, par.setValue(data, time));

    // test result
    EXPECT_EQ(OperationResult::success, par.getValue(returnedData, &returnedTime));
    EXPECT_EQ(returnedData, data);
    EXPECT_EQ(returnedTime, time);
}

TEST(ParameterTest, sortAndFind)
{
    outpost::time::SpacecraftElapsedTime time;
    // test invalid id results in an uninitialized one.
    Parameter<uint32_t> par1(1, 1, time);
    Parameter<uint32_t> par2(2, 1, time);
    Parameter<uint32_t> par3(3, 1, time);
    Parameter<uint32_t> par4(4, 1, time);
    Parameter<uint32_t> par5(5, 1, time);
    Parameter<uint32_t> par6(6, 1, time);
    Parameter<uint32_t> par7(7, 1, time);
    Parameter<uint32_t> par8(8, 1, time);
    Parameter<uint32_t> par10(10, 1, time);
    Parameter<uint32_t> par11(11, 1, time);

    // put 5 in double
    ParameterBase* array[11] = {
            &par3, &par1, &par5, &par8, &par7, &par2, &par5, &par4, &par11, &par6, &par10};
    auto slice = outpost::asSlice(array);
    ParameterBase::sort(slice);

    EXPECT_EQ(array[0], &par1);
    EXPECT_EQ(array[1], &par2);
    EXPECT_EQ(array[2], &par3);
    EXPECT_EQ(array[3], &par4);
    EXPECT_EQ(array[4], &par5);
    EXPECT_EQ(array[5], &par5);
    EXPECT_EQ(array[6], &par6);
    EXPECT_EQ(array[7], &par7);
    EXPECT_EQ(array[8], &par8);
    EXPECT_EQ(array[9], &par10);
    EXPECT_EQ(array[10], &par11);

    // last
    EXPECT_EQ(&par11, ParameterBase::findInSorted(slice, 11));
    // the double
    EXPECT_EQ(&par5, ParameterBase::findInSorted(slice, 5));
    // some in the middle
    EXPECT_EQ(&par7, ParameterBase::findInSorted(slice, 7));
    // first
    EXPECT_EQ(&par1, ParameterBase::findInSorted(slice, 1));
    // before
    EXPECT_EQ(nullptr, ParameterBase::findInSorted(slice, 0));
    // after
    EXPECT_EQ(nullptr, ParameterBase::findInSorted(slice, 12));
    // hole in middle
    EXPECT_EQ(nullptr, ParameterBase::findInSorted(slice, 9));
}
