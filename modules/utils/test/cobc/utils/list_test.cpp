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

#include <unittest/harness.h>

#include <cobc/utils/list.h>

using namespace cobc;

struct ListNode
{
    uint8_t mValue;

    // needed for the list handling
    ListNode* mNext;
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
        Condition(uint8_t value) :
            mValue(value)
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

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

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

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    class Condition
    {
    public:
        Condition(uint8_t value) :
            mValue(value)
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

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };

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

    ListNode node1 = { 1, 0 };
    ListNode node2 = { 3, 0 };
    ListNode node3 = { 5, 0 };
    ListNode node4 = { 3, 0 };

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

TEST(ListTest, shouldIterateOverElements)
{
	List<ListNode> list;

	ListNode node1 = { 1, 0 };
	ListNode node2 = { 3, 0 };
	ListNode node3 = { 5, 0 };

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
