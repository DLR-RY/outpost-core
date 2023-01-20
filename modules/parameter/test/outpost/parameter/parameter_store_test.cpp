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

#include <outpost/parameter/parameter_store.h>
#include <outpost/time.h>

#include <unittest/harness.h>

#include <stdint.h>

#include <set>

using namespace outpost::parameter;

TEST(ParameterStoreTest, defaultConstructorAndList)
{
    ParameterList list;
    outpost::time::SpacecraftElapsedTime time;
    // test invalid id results in an uninitialized one.

    IDType id = ParameterBase::invalidID + 1;
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par(id, data, time, list);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(false, store.isInitialized());
    EXPECT_EQ(OperationResult::success, store.initialize(list));
    EXPECT_EQ(true, store.isInitialized());

    // with correct id
    const ParameterBase* pointer = nullptr;
    EXPECT_EQ(OperationResult::success, store.getParameter(id, pointer));
    EXPECT_EQ(&par, pointer);

    const Parameter<uint32_t>* typedPointer = nullptr;
    EXPECT_EQ(OperationResult::success, store.getParameter(id, typedPointer));
    EXPECT_EQ(&par, typedPointer);

    uint32_t readData = 0;
    outpost::time::SpacecraftElapsedTime readtime;
    EXPECT_EQ(OperationResult::success, store.getValue(id, readData, &readtime));
    EXPECT_EQ(data, readData);
    EXPECT_EQ(time, readtime);

    // invalid ID
    EXPECT_EQ(OperationResult::invalidParameter,
              store.getParameter(ParameterBase::invalidID, pointer));
    EXPECT_EQ(OperationResult::invalidParameter,
              store.getValue(ParameterBase::invalidID, readData, &readtime));

    // unknown ID
    EXPECT_EQ(OperationResult::noSuchID, store.getParameter(id + 1, pointer));
    EXPECT_EQ(OperationResult::noSuchID, store.getValue(id + 1, readData, &readtime));
}

TEST(ParameterStoreTest, uninitializedParameter)
{
    ParameterList list;
    outpost::time::SpacecraftElapsedTime time;

    IDType id = ParameterBase::invalidID + 1;
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id, data, time, list);
    Parameter<uint32_t> par2(list);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(OperationResult::uninitializedParameter, store.initialize(list));
    EXPECT_EQ(false, store.isInitialized());
}

TEST(ParameterStoreTest, invalidParameter)
{
    ParameterList list;
    outpost::time::SpacecraftElapsedTime time;

    IDType id = ParameterBase::invalidID + 1;
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id, data, time, list);
    Parameter<uint32_t> par2(ParameterBase::invalidID, data, time, list);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(OperationResult::invalidParameter, store.initialize(list));
    EXPECT_EQ(false, store.isInitialized());
}

TEST(ParameterStoreTest, dublicatedID)
{
    ParameterList list;
    outpost::time::SpacecraftElapsedTime time;

    IDType id = ParameterBase::invalidID + 10;
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id, data, time, list);
    Parameter<uint32_t> par2(id - 1, data, time, list);
    Parameter<uint32_t> par3(id, data, time, list);
    Parameter<uint32_t> par4(id + 1, data, time, list);
    Parameter<uint32_t> par5(id + 2, data, time, list);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(OperationResult::dublicatedID, store.initialize(list));
    EXPECT_EQ(false, store.isInitialized());
}

TEST(ParameterStoreTest, dublicatedIDMultiList)
{
    ParameterList list1;
    ParameterList list2;
    outpost::time::SpacecraftElapsedTime time;

    IDType id = ParameterBase::invalidID + 10;
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id, data, time, list1);
    Parameter<uint32_t> par2(id - 1, data, time, list1);
    Parameter<uint32_t> par3(id, data, time, list2);
    Parameter<uint32_t> par4(id + 1, data, time, list2);
    Parameter<uint32_t> par5(id + 2, data, time, list1);

    ParameterStoreWithMemory<5> store;

    ParameterList* array[] = {&list1, &list2};
    EXPECT_EQ(OperationResult::dublicatedID, store.initialize(outpost::asSlice(array)));
    EXPECT_EQ(false, store.isInitialized());
}

TEST(ParameterStoreTest, initializeSuccessAfterFixedProblem)
{
    ParameterList list1;
    ParameterList list2;
    outpost::time::SpacecraftElapsedTime time;

    IDType id = ParameterBase::invalidID + 10;
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id, data, time, list1);
    Parameter<uint32_t> par2(id - 1, data, time, list1);
    Parameter<uint32_t> par3(id, data, time, list2);
    Parameter<uint32_t> par4(id + 1, data, time, list1);
    Parameter<uint32_t> par5(id + 2, data, time, list1);

    ParameterStoreWithMemory<5> store;

    ParameterList* array[] = {&list1, &list2};
    EXPECT_EQ(OperationResult::dublicatedID, store.initialize(outpost::asSlice(array)));
    EXPECT_EQ(false, store.isInitialized());
    EXPECT_EQ(OperationResult::success, store.initialize(list1));
}

TEST(ParameterStoreTest, maxParameter)
{
    ParameterList plist;
    outpost::time::SpacecraftElapsedTime time;

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 11,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 8};
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id[0], data, time, plist);
    Parameter<uint32_t> par2(id[1], data, time, plist);
    Parameter<uint32_t> par3(id[2], data, time, plist);
    Parameter<uint32_t> par4(id[3], data, time, plist);
    Parameter<uint32_t> par5(id[4], data, time, plist);
    const ParameterBase* list[5] = {&par1, &par2, &par3, &par4, &par5};

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(OperationResult::success, store.initialize(plist));

    for (int i = 0; i < 5; i++)
    {
        const ParameterBase* pointer = nullptr;
        EXPECT_EQ(OperationResult::success, store.getParameter(id[i], pointer));
        EXPECT_EQ(list[i], pointer);
    }

    // not included id just for final sanity check
    const ParameterBase* pointer = nullptr;
    EXPECT_EQ(OperationResult::noSuchID, store.getParameter(ParameterBase::invalidID + 1, pointer));
}

TEST(ParameterStoreTest, maxParameterInAListMultiList)
{
    ParameterList plist;
    ParameterList plist2;
    outpost::time::SpacecraftElapsedTime time;

    // add them unsorted for additional testing
    IDType id[6] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 11,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 8,
                    ParameterBase::invalidID + 7};
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id[0], data, time, plist);
    Parameter<uint32_t> par2(id[1], data, time, plist);
    Parameter<uint32_t> par3(id[2], data, time, plist);
    Parameter<uint32_t> par4(id[3], data, time, plist);
    Parameter<uint32_t> par5(id[4], data, time, plist);
    Parameter<uint32_t> par6(id[5], data, time, plist2);

    ParameterStoreWithMemory<5> store;

    ParameterList* array[] = {&plist, &plist2};
    EXPECT_EQ(OperationResult::tooManyElements, store.initialize(outpost::asSlice(array)));
    EXPECT_EQ(false, store.isInitialized());
}

TEST(ParameterStoreTest, invalidType)
{
    ParameterList plist;
    outpost::time::SpacecraftElapsedTime time;

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 11,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 8};
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id[0], data, time, plist);
    Parameter<uint32_t> par2(id[1], data, time, plist);
    Parameter<uint32_t> par3(id[2], data, time, plist);
    Parameter<uint32_t> par4(id[3], data, time, plist);
    Parameter<uint32_t> par5(id[4], data, time, plist);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(OperationResult::success, store.initialize(plist));

    uint16_t readData;
    outpost::time::SpacecraftElapsedTime readTime;
    EXPECT_EQ(OperationResult::incorrectType, store.getValue(id[0], readData, &readTime));

    const Parameter<uint16_t>* readParameter = nullptr;
    EXPECT_EQ(OperationResult::incorrectType, store.getValue(id[0], readParameter));
}

TEST(ParameterStoreTest, maxParameterMultiList)
{
    ParameterList plist1;
    ParameterList plist2;
    outpost::time::SpacecraftElapsedTime time;

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 11,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 8};
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id[0], data, time, plist1);
    Parameter<uint32_t> par2(id[1], data, time, plist1);
    Parameter<uint32_t> par3(id[2], data, time, plist1);
    Parameter<uint32_t> par4(id[3], data, time, plist2);
    Parameter<uint32_t> par5(id[4], data, time, plist2);
    const ParameterBase* list[5] = {&par1, &par2, &par3, &par4, &par5};

    ParameterStoreWithMemory<5> store;

    ParameterList* array[] = {&plist1, &plist2};
    EXPECT_EQ(OperationResult::success, store.initialize(outpost::asSlice(array)));

    for (int i = 0; i < 5; i++)
    {
        const ParameterBase* pointer = nullptr;
        EXPECT_EQ(OperationResult::success, store.getParameter(id[i], pointer));
        EXPECT_EQ(list[i], pointer);
    }

    // not included id just for final sanity check
    const ParameterBase* pointer = nullptr;
    EXPECT_EQ(OperationResult::noSuchID, store.getParameter(ParameterBase::invalidID + 1, pointer));
}

TEST(ParameterStoreTest, tooManyParameter)
{
    ParameterList plist;
    outpost::time::SpacecraftElapsedTime time;

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 11,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 8};
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id[0], data, time, plist);
    Parameter<uint32_t> par2(id[1], data, time, plist);
    Parameter<uint32_t> par3(id[2], data, time, plist);
    Parameter<uint32_t> par4(id[3], data, time, plist);
    Parameter<uint32_t> par5(id[4], data, time, plist);

    ParameterStoreWithMemory<4> store;

    EXPECT_EQ(OperationResult::tooManyElements, store.initialize(plist));
}

TEST(ParameterStoreTest, tooManyParameterMultiList)
{
    ParameterList plist1;
    ParameterList plist2;
    outpost::time::SpacecraftElapsedTime time;

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 11,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 8};
    uint32_t data = 10;
    time = time + outpost::time::Minutes(1);
    Parameter<uint32_t> par1(id[0], data, time, plist1);
    Parameter<uint32_t> par2(id[1], data, time, plist1);
    Parameter<uint32_t> par3(id[2], data, time, plist2);
    Parameter<uint32_t> par4(id[3], data, time, plist2);
    Parameter<uint32_t> par5(id[4], data, time, plist2);

    ParameterStoreWithMemory<4> store;

    ParameterList* array[] = {&plist1, &plist2};
    EXPECT_EQ(OperationResult::tooManyElements, store.initialize(outpost::asSlice(array)));
}

TEST(ParameterStoreTest, getValue)
{
    ParameterList plist;
    outpost::time::SpacecraftElapsedTime time[5];

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 13,
                    ParameterBase::invalidID + 8};
    uint32_t data[5] = {10, 14, 17, 19, 20};
    for (int i = 0; i < 5; i++)
    {
        time[i] = time[i] + outpost::time::Minutes(i);
    }
    Parameter<uint32_t> par1(id[0], data[0], time[0], plist);
    Parameter<uint32_t> par2(id[1], data[1], time[1], plist);
    Parameter<uint32_t> par3(id[2], data[2], time[2], plist);
    Parameter<uint32_t> par4(id[3], data[3], time[3], plist);
    Parameter<uint32_t> par5(id[4], data[4], time[4], plist);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(OperationResult::success, store.initialize(plist));

    for (int i = 0; i < 5; i++)
    {
        uint32_t readData;
        outpost::time::SpacecraftElapsedTime readTime;
        EXPECT_EQ(OperationResult::success, store.getValue(id[i], readData, &readTime));
        EXPECT_EQ(data[i], readData);
        EXPECT_EQ(time[i], readTime);
    }

    // not included id just for final sanity check
    uint32_t readData;

    // smaller
    outpost::time::SpacecraftElapsedTime readTime;
    EXPECT_EQ(OperationResult::noSuchID,
              store.getValue(ParameterBase::invalidID + 1, readData, &readTime));

    // larger
    EXPECT_EQ(OperationResult::noSuchID,
              store.getValue(ParameterBase::invalidID + 20, readData, &readTime));

    // between valid ids
    EXPECT_EQ(OperationResult::noSuchID,
              store.getValue(ParameterBase::invalidID + 11, readData, &readTime));
}

TEST(ParameterStoreTest, IteratorTest)
{
    ParameterList plist;
    outpost::time::SpacecraftElapsedTime time[5];

    // add them unsorted for additional testing
    IDType id[5] = {ParameterBase::invalidID + 10,
                    ParameterBase::invalidID + 9,
                    ParameterBase::invalidID + 12,
                    ParameterBase::invalidID + 13,
                    ParameterBase::invalidID + 8};
    uint32_t data[5] = {10, 14, 17, 19, 20};
    for (int i = 0; i < 5; i++)
    {
        time[i] = time[i] + outpost::time::Minutes(i);
    }
    Parameter<uint32_t> par1(id[0], data[0], time[0], plist);
    Parameter<uint32_t> par2(id[1], data[1], time[1], plist);
    Parameter<uint32_t> par3(id[2], data[2], time[2], plist);
    Parameter<uint32_t> par4(id[3], data[3], time[3], plist);
    Parameter<uint32_t> par5(id[4], data[4], time[4], plist);

    std::set<const ParameterBase*> expected;
    expected.insert(&par1);
    expected.insert(&par2);
    expected.insert(&par3);
    expected.insert(&par4);
    expected.insert(&par5);

    ParameterStoreWithMemory<5> store;

    EXPECT_EQ(static_cast<size_t>(0), store.getCount());
    EXPECT_EQ(store.begin(), store.end());

    EXPECT_EQ(OperationResult::success, store.initialize(plist));

    EXPECT_EQ(static_cast<size_t>(5), store.getCount());
    EXPECT_NE(store.begin(), store.end());

    // + and -
    EXPECT_EQ(store.begin() + store.getCount(), store.end());
    EXPECT_EQ(store.begin(), store.end() - store.getCount());
    EXPECT_NE(store.begin(), store.begin() + store.getCount());
    EXPECT_NE(store.end(), store.end() - store.getCount());

    EXPECT_EQ(static_cast<ParameterStore::Iterator::DiffType>(store.getCount()),
              store.end() - store.begin());

    std::set<const ParameterBase*> actual;

    // postfix ++ iteration and operator*
    for (auto it = store.begin(); it != store.end(); ++it)
    {
        actual.insert(&(*it));
    }

    EXPECT_EQ(expected, actual);
    actual.clear();

    // prefix ++ iteration
    for (auto it = store.begin(); it != store.end(); ++it)
    {
        actual.insert(&(*it));
    }

    EXPECT_EQ(expected, actual);

    // prefix
    EXPECT_EQ(store.begin(), ++--store.begin());
    EXPECT_NE(store.end(), --store.end());
    EXPECT_NE(store.begin(), ++store.begin());

    // postfix
    auto tmp = store.end();
    EXPECT_EQ(store.end(), tmp--);
    EXPECT_NE(store.end(), tmp);
    EXPECT_NE(store.end(), tmp++);
    EXPECT_EQ(store.end(), tmp);

    // operator->
    std::set<uint32_t> actualID;
    std::set<uint32_t> expectedID(&(id[0]), &(id[5]));

    for (auto it = store.begin(); it != store.end(); ++it)
    {
        actualID.insert(it->getID());
        EXPECT_EQ(it->getType(), Type::getType<uint32_t>());
    }

    EXPECT_EQ(actualID, expectedID);
}
