/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/utils/container/list.h>

#include <unittest/harness.h>

#include <vector>

using namespace outpost;

struct ListNode : public outpost::ListElement
{
    ListNode() : mValue(0){};
    ListNode(uint8_t v) : mValue(v){};
    uint8_t mValue;
};

struct RelationalListNode : public outpost::ListElement
{
    RelationalListNode() : mValue(0){};
    RelationalListNode(uint8_t v) : mValue(v){};
    uint8_t mValue;

    bool
    operator<(const RelationalListNode& other)
    {
        return mValue < other.mValue;
    }
};

TEST(ListTest, createAndAdd)
{
    List<ListNode> list;

    EXPECT_TRUE(list.isEmpty());

    ListNode node;

    list.prepend(&node);

    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(&node, list.first());
}

TEST(ListTest, get)
{
    class Condition
    {
    public:
        Condition(uint8_t value) : mValue(value)
        {
        }

        inline bool
        operator()(const ListNode& node)
        {
            return (node.mValue == mValue);
        }

        uint8_t mValue;
    };

    List<ListNode> list;

    EXPECT_EQ(0, list.get(Condition(2)));

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    EXPECT_EQ(&node1, list.get(Condition(1)));
    EXPECT_EQ(&node2, list.get(Condition(3)));
    EXPECT_EQ(&node3, list.get(Condition(5)));

    EXPECT_EQ(0, list.get(Condition(2)));
}

TEST(ListTest, getN)
{
    List<ListNode> list;

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    EXPECT_EQ(&node1, list.getN(2));
    EXPECT_EQ(&node2, list.getN(1));
    EXPECT_EQ(&node3, list.getN(0));
}

TEST(ListTest, insert)
{
    List<RelationalListNode> list;

    RelationalListNode node1(1);
    RelationalListNode node2(2);
    RelationalListNode node3(3);
    RelationalListNode node4(4);
    RelationalListNode node5(5);

    list.insert(&node4);
    list.insert(&node3);
    list.insert(&node2);
    list.insert(&node1);
    list.insert(&node5);

    EXPECT_EQ(&node1, list.getN(0));
    EXPECT_EQ(&node2, list.getN(1));
    EXPECT_EQ(&node3, list.getN(2));
    EXPECT_EQ(&node4, list.getN(3));
    EXPECT_EQ(&node5, list.getN(4));
}

TEST(ListTest, remove)
{
    List<ListNode> list;

    EXPECT_FALSE(list.removeNode(0));

    ListNode node;
    ListNode node2;
    ListNode node3;

    list.prepend(&node);
    list.prepend(&node2);
    list.prepend(&node3);

    EXPECT_TRUE(list.removeNode(&node2));

    // node is already removed
    EXPECT_FALSE(list.removeNode(&node2));

    EXPECT_TRUE(list.removeNode(&node3));
    EXPECT_TRUE(list.removeNode(&node));

    EXPECT_TRUE(list.isEmpty());
}

TEST(ListTest, removeWithFunctor)
{
    List<ListNode> list;

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    class Condition
    {
    public:
        Condition(uint8_t value) : mValue(value)
        {
        }

        inline bool
        operator()(const ListNode& node)
        {
            return (node.mValue == mValue);
        }

        uint8_t mValue;
    };

    EXPECT_EQ(&node2, list.remove(Condition(3)));

    // node is already removed
    EXPECT_EQ(0, list.remove(Condition(3)));

    EXPECT_EQ(&node3, list.remove(Condition(5)));
    EXPECT_EQ(&node1, list.remove(Condition(1)));

    EXPECT_TRUE(list.isEmpty());
}

static inline bool
condition(const ListNode& node)
{
    return (node.mValue == 5);
}

TEST(ListTest, removeWithFunction)
{
    List<ListNode> list;

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    EXPECT_EQ(3U, list.size());

    EXPECT_EQ(&node3, list.remove(condition));

    EXPECT_EQ(2U, list.size());
}

static bool
check(const ListNode& node)
{
    return (node.mValue == 3);
}

TEST(ListTest, removeAll)
{
    List<ListNode> list;

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);
    ListNode node4(3);

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);
    list.prepend(&node4);

    list.removeAll(check);

    // node is already removed
    EXPECT_FALSE(list.removeNode(&node2));
    EXPECT_FALSE(list.removeNode(&node4));

    EXPECT_TRUE(list.removeNode(&node1));
    EXPECT_TRUE(list.removeNode(&node3));
}

TEST(ListTest, removeAllWithPostCondition)
{
    List<ListNode> list;

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);
    ListNode node4(3);
    ListNode node5(5);
    ListNode node6(4);

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);
    list.prepend(&node4);
    list.prepend(&node5);
    list.prepend(&node6);

    std::vector<ListNode*> v;

    list.removeAll([](ListNode& l) { return l.mValue > 3; },
                   [&v](ListNode& l) { v.push_back(&l); });

    EXPECT_EQ(3u, list.size());

    EXPECT_EQ(&node4, list.getN(0));
    EXPECT_EQ(&node2, list.getN(1));
    EXPECT_EQ(&node1, list.getN(2));

    EXPECT_EQ(3u, v.size());

    EXPECT_EQ(&node6, v[0]);
    EXPECT_EQ(&node5, v[1]);
    EXPECT_EQ(&node3, v[2]);
}

TEST(ListTest, shouldIterateOverElements)
{
    List<ListNode> list;

    ListNode node1(1);
    ListNode node2(3);
    ListNode node3(5);

    list.prepend(&node3);
    list.prepend(&node2);
    list.prepend(&node1);

    List<ListNode>::Iterator it = list.begin();

    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 1);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 3);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 5);
    ++it;

    ASSERT_FALSE(it != list.end());
}
