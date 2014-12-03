/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.3-pre.
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

#include <cobc/utils/list.h>

using namespace cobc;

struct ListNode
{
    uint8_t value;

    // needed for the list handling
    ListNode* next;
};

TEST(ListTest, createAndAdd)
{
    List<ListNode> list;

    EXPECT_TRUE(list.isEmpty());

    ListNode node;

    list.add(&node);

    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(&node, list.first());
}

TEST(ListTest, get)
{
    class Condition
    {
    public:
        Condition(uint8_t value) :
            value(value)
        {
        }

        inline bool
        operator()(const ListNode& node)
        {
            return (node.value == value);
        }

        uint8_t value;
    };

    List<ListNode> list;

    EXPECT_EQ(0, list.get(Condition(2)));

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };

    list.add(&node1);
    list.add(&node2);
    list.add(&node3);

    EXPECT_EQ(&node1, list.get(Condition(1)));
    EXPECT_EQ(&node2, list.get(Condition(3)));
    EXPECT_EQ(&node3, list.get(Condition(5)));

    EXPECT_EQ(0, list.get(Condition(2)));
}

TEST(ListTest, remove)
{
    List<ListNode> list;

    EXPECT_FALSE(list.removeNode(0));

    ListNode node;
    ListNode node2;
    ListNode node3;

    list.add(&node);
    list.add(&node2);
    list.add(&node3);

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

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };

    list.add(&node1);
    list.add(&node2);
    list.add(&node3);

    class Condition
    {
    public:
        Condition(uint8_t value) :
            value(value)
        {
        }

        inline bool
        operator()(const ListNode& node)
        {
            return (node.value == value);
        }

        uint8_t value;
    };

    EXPECT_EQ(&node2, list.remove(Condition(3)));

    // node is already removed
    EXPECT_EQ(0, list.remove(Condition(3)));

    EXPECT_EQ(&node3, list.remove(Condition(5)));
    EXPECT_EQ(&node1, list.remove(Condition(1)));

    EXPECT_TRUE(list.isEmpty());
}

static bool
check(const ListNode& node)
{
    return (node.value == 3);
}

TEST(ListTest, removeAll)
{
    List<ListNode> list;

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };
    ListNode node4 = { 3, 0 };

    list.add(&node1);
    list.add(&node2);
    list.add(&node3);
    list.add(&node4);

    list.removeAll(check);

    // node is already removed
    EXPECT_FALSE(list.removeNode(&node2));
    EXPECT_FALSE(list.removeNode(&node4));

    EXPECT_TRUE(list.removeNode(&node1));
    EXPECT_TRUE(list.removeNode(&node3));
}
