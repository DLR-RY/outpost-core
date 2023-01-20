/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/utils/container/circular_singly_linked_list.h>

#include <unittest/harness.h>

using namespace outpost;

struct CircularSinglyLinkedListNode
{
    uint8_t mValue;

    // needed for the list handling
    CircularSinglyLinkedListNode* mNext;

    bool
    operator<(const CircularSinglyLinkedListNode& other) const
    {
        return (mValue < other.mValue);
    }
};

TEST(CircularSinglyLinkedListTest, createAndAdd)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    EXPECT_TRUE(list.isEmpty());

    CircularSinglyLinkedListNode node;

    list.prepend(&node);

    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(&node, list.first());
    EXPECT_EQ(&node, list.last());

    list.reset();
}

TEST(CircularSinglyLinkedListTest, constfirstlast)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node;

    list.prepend(&node);

    const CircularSinglyLinkedList<CircularSinglyLinkedListNode>* constListPointer = &list;

    EXPECT_EQ(&node, constListPointer->first());
    EXPECT_EQ(&node, constListPointer->last());
}

TEST(CircularSinglyLinkedListTest, shouldAppendNodesToTheEnd)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    EXPECT_TRUE(list.isEmpty());

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.append(&node1);
    list.append(&node2);
    list.append(&node3);

    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(&node1, list.first());
}

TEST(CircularSinglyLinkedListTest, get)
{
    class Condition
    {
    public:
        explicit Condition(uint8_t value) : mValue(value)
        {
        }

        inline bool
        operator()(const CircularSinglyLinkedListNode& node) const
        {
            return (node.mValue == mValue);
        }

        uint8_t mValue;
    };

    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    EXPECT_EQ(0, list.get(Condition(2)));

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    EXPECT_EQ(&node1, list.get(Condition(1)));
    EXPECT_EQ(&node2, list.get(Condition(3)));
    EXPECT_EQ(&node3, list.get(Condition(5)));

    EXPECT_EQ(0, list.get(Condition(2)));

    const CircularSinglyLinkedList<CircularSinglyLinkedListNode>* constListPointer = &list;
    EXPECT_EQ(&node1, constListPointer->get(Condition(1)));
    EXPECT_EQ(&node2, constListPointer->get(Condition(3)));
    EXPECT_EQ(&node3, constListPointer->get(Condition(5)));

    EXPECT_EQ(0, constListPointer->get(Condition(4)));
}

TEST(CircularSinglyLinkedListTest, getN)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    EXPECT_EQ(0, list.getN(0));
    EXPECT_EQ(0, list.getN(10));

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    EXPECT_EQ(&node1, list.getN(2));
    EXPECT_EQ(&node2, list.getN(1));
    EXPECT_EQ(&node3, list.getN(0));

    EXPECT_EQ(0, list.getN(3));

    const CircularSinglyLinkedList<CircularSinglyLinkedListNode>* constListPointer = &list;
    EXPECT_EQ(&node1, constListPointer->getN(2));
    EXPECT_EQ(&node2, constListPointer->getN(1));
    EXPECT_EQ(&node3, constListPointer->getN(0));

    EXPECT_EQ(0, constListPointer->getN(3));
}

TEST(CircularSinglyLinkedListTest, remove)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    EXPECT_FALSE(list.removeNode(0));

    CircularSinglyLinkedListNode node1;
    CircularSinglyLinkedListNode node2;
    CircularSinglyLinkedListNode node3;

    list.append(&node1);
    list.append(&node2);
    list.append(&node3);

    EXPECT_TRUE(list.removeNode(&node3));
    EXPECT_TRUE(list.removeNode(&node2));

    // node is already removed
    EXPECT_FALSE(list.removeNode(&node2));

    EXPECT_TRUE(list.removeNode(&node1));

    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularSinglyLinkedListTest, shouldRemoveFirstEntry)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1;
    CircularSinglyLinkedListNode node2;
    CircularSinglyLinkedListNode node3;

    list.append(&node1);
    list.append(&node2);
    list.append(&node3);

    EXPECT_EQ(&node1, list.first());
    list.removeFirst();
    EXPECT_EQ(&node2, list.first());
    list.removeFirst();
    EXPECT_EQ(&node3, list.first());
    list.removeFirst();
    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularSinglyLinkedListTest, removeWithFunctor)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.prepend(&node1);
    list.prepend(&node2);
    list.prepend(&node3);

    class Condition
    {
    public:
        explicit Condition(uint8_t value) : mValue(value)
        {
        }

        inline bool
        operator()(const CircularSinglyLinkedListNode& node) const
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
condition(const CircularSinglyLinkedListNode& node)
{
    return (node.mValue == 5);
}

TEST(CircularSinglyLinkedListTest, removeWithConditionFunction)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.append(&node1);
    list.append(&node2);
    list.append(&node3);

    EXPECT_EQ(3U, list.size());

    EXPECT_EQ(&node3, list.remove(condition));

    EXPECT_EQ(2U, list.size());
}

static bool
check(const CircularSinglyLinkedListNode& node)
{
    return (node.mValue == 3);
}

TEST(CircularSinglyLinkedListTest, removeAllWithConditionFunction)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};
    CircularSinglyLinkedListNode node4 = {3, 0};

    list.append(&node1);
    list.append(&node2);
    list.append(&node3);
    list.append(&node4);

    list.removeAll(check);

    // node is already removed
    EXPECT_FALSE(list.removeNode(&node2));
    EXPECT_FALSE(list.removeNode(&node4));

    EXPECT_TRUE(list.removeNode(&node1));
    EXPECT_TRUE(list.removeNode(&node3));
}

static bool
checkAlwaysTrue(const CircularSinglyLinkedListNode& /*node*/)
{
    return true;
}

TEST(CircularSinglyLinkedListTest, shouldRemoveAll)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};
    CircularSinglyLinkedListNode node4 = {3, 0};

    list.append(&node1);
    list.append(&node2);
    list.append(&node3);
    list.append(&node4);

    list.removeAll(checkAlwaysTrue);

    EXPECT_TRUE(list.isEmpty());
}

TEST(CircularSinglyLinkedListTest, shouldIterateOverPrependedElements)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.prepend(&node3);
    list.prepend(&node2);
    list.prepend(&node1);

    CircularSinglyLinkedList<CircularSinglyLinkedListNode>::Iterator it = list.begin();

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

TEST(CircularSinglyLinkedListTest, shouldIterateOverAppendedElements)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {3, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};

    list.append(&node3);
    list.prepend(&node2);
    list.append(&node1);

    CircularSinglyLinkedList<CircularSinglyLinkedListNode>::Iterator it = list.begin();

    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 3);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 5);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 1);
    ++it;

    ASSERT_FALSE(it != list.end());
}

TEST(CircularSinglyLinkedListTest, shouldIterateOverInsertedElements)
{
    CircularSinglyLinkedList<CircularSinglyLinkedListNode> list;

    CircularSinglyLinkedListNode node1 = {1, 0};
    CircularSinglyLinkedListNode node2 = {7, 0};
    CircularSinglyLinkedListNode node3 = {5, 0};
    CircularSinglyLinkedListNode node4 = {10, 0};
    CircularSinglyLinkedListNode node5 = {2, 0};

    list.insert(&node1);
    list.insert(&node2);
    list.insert(&node3);
    list.insert(&node4);
    list.insert(&node5);

    EXPECT_EQ(5U, list.size());

    CircularSinglyLinkedList<CircularSinglyLinkedListNode>::Iterator it = list.begin();

    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 1);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 2);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 5);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 7);
    ++it;
    ASSERT_TRUE(it != list.end());
    EXPECT_EQ(it->mValue, 10);
    ++it;

    ASSERT_FALSE(it != list.end());
}
