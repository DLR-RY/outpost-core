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
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/utils/container/deque.h>

#include <unittest/harness.h>

TEST(DequeTest, forward)
{
    int16_t buffer[3];
    outpost::Deque<int16_t> deque(buffer, 3);

    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(deque.getMaxSize(), 3U);

    EXPECT_EQ(deque.getSize(), 0U);

    EXPECT_TRUE(deque.append(1));
    EXPECT_EQ(deque.getSize(), 1U);
    EXPECT_TRUE(deque.append(2));
    EXPECT_EQ(deque.getSize(), 2U);
    EXPECT_TRUE(deque.append(3));
    EXPECT_EQ(deque.getSize(), 3U);

    EXPECT_FALSE(deque.append(4));
    EXPECT_EQ(deque.getSize(), 3U);
    EXPECT_TRUE(deque.isFull());

    EXPECT_EQ(deque.getFront(), 1);
    deque.removeFront();
    EXPECT_EQ(deque.getSize(), 2U);

    EXPECT_EQ(deque.getFront(), 2);
    deque.removeFront();
    EXPECT_EQ(deque.getSize(), 1U);

    EXPECT_TRUE(deque.append(4));
    EXPECT_EQ(deque.getSize(), 2U);
    EXPECT_TRUE(deque.append(5));
    EXPECT_EQ(deque.getSize(), 3U);
    EXPECT_TRUE(deque.isFull());

    EXPECT_EQ(deque.getFront(), 3);
    deque.removeFront();
    EXPECT_EQ(deque.getSize(), 2U);

    EXPECT_EQ(deque.getFront(), 4);
    deque.removeFront();
    EXPECT_EQ(deque.getSize(), 1U);

    EXPECT_EQ(deque.getFront(), 5);
    deque.removeFront();

    EXPECT_EQ(deque.getSize(), 0U);
    EXPECT_TRUE(deque.isEmpty());
}

TEST(DequeTest, backward)
{
    int16_t buffer[3];
    outpost::Deque<int16_t> deque(buffer, 3);

    EXPECT_TRUE(deque.prepend(1));
    EXPECT_TRUE(deque.prepend(2));
    EXPECT_TRUE(deque.prepend(3));

    EXPECT_FALSE(deque.prepend(4));
    EXPECT_TRUE(deque.isFull());

    EXPECT_EQ(deque.getBack(), 1);
    deque.removeBack();

    EXPECT_EQ(deque.getBack(), 2);
    deque.removeBack();

    EXPECT_TRUE(deque.prepend(4));
    EXPECT_TRUE(deque.prepend(5));
    EXPECT_TRUE(deque.isFull());

    EXPECT_EQ(deque.getBack(), 3);
    deque.removeBack();

    EXPECT_EQ(deque.getBack(), 4);
    deque.removeBack();

    EXPECT_EQ(deque.getBack(), 5);
    deque.removeBack();

    EXPECT_TRUE(deque.isEmpty());
}

TEST(DequeTest, both)
{
    int16_t buffer[3];
    outpost::Deque<int16_t> deque(buffer, 3);

    EXPECT_EQ(deque.getSize(), 0U);

    EXPECT_TRUE(deque.prepend(12));
    EXPECT_EQ(deque.getSize(), 1U);

    EXPECT_TRUE(deque.prepend(11));
    EXPECT_EQ(deque.getSize(), 2U);

    EXPECT_TRUE(deque.prepend(10));
    EXPECT_EQ(deque.getSize(), 3U);

    EXPECT_EQ(deque.getFront(), 10);
    deque.removeFront();

    EXPECT_EQ(deque.getFront(), 11);
    deque.removeFront();

    EXPECT_TRUE(deque.append(13));
    EXPECT_TRUE(deque.append(14));
    EXPECT_TRUE(deque.isFull());

    EXPECT_EQ(deque.getBack(), 14);
    deque.removeBack();

    EXPECT_EQ(deque.getFront(), 12);
    deque.removeFront();

    EXPECT_EQ(deque.getBack(), 13);
    deque.removeBack();

    EXPECT_TRUE(deque.isEmpty());
}

TEST(DequeTest, clear)
{
    int16_t buffer[3];
    outpost::Deque<int16_t> deque(buffer, 3);

    deque.prepend(12);
    deque.prepend(11);
    deque.prepend(10);

    EXPECT_EQ(deque.getSize(), 3U);

    deque.clear();

    EXPECT_EQ(deque.getSize(), 0U);
    EXPECT_TRUE(deque.isEmpty());

    deque.prepend(12);
    deque.prepend(11);
    deque.prepend(10);
    EXPECT_EQ(deque.getSize(), 3U);

    EXPECT_EQ(deque.getFront(), 10);
    deque.removeFront();

    EXPECT_EQ(deque.getFront(), 11);
    deque.removeFront();

    deque.append(13);
    deque.append(14);
    EXPECT_TRUE(deque.isFull());

    EXPECT_EQ(deque.getBack(), 14);
    deque.removeBack();

    EXPECT_EQ(deque.getFront(), 12);
    deque.removeFront();

    EXPECT_EQ(deque.getBack(), 13);
    deque.removeBack();

    EXPECT_TRUE(deque.isEmpty());
}

TEST(DequeTest, appendSlice) {
    // invalid = -, head = h, tail = t
    int16_t buffer[5];
    outpost::Deque<int16_t> deque(buffer, 5); // {-h -t - - -}

    int16_t elements[3];
    outpost::Slice<int16_t> slice(elements);
    slice[0] = 20;
    slice[1] = 31;
    slice[2] = 42;

    EXPECT_TRUE(deque.append(slice)); // {- 20t 31 42h -}
    EXPECT_EQ(2u, deque.getAvailableSpace());
    EXPECT_EQ(3u, deque.getSize());
    EXPECT_EQ(20, deque.getFront());
    EXPECT_EQ(42, deque.getBack());

    deque.removeFront(); // {- - 31t 42h -}

    EXPECT_TRUE(deque.append(slice)); // {31 42h 31t 42 20}
    EXPECT_EQ(0u, deque.getAvailableSpace());
    EXPECT_EQ(5u, deque.getSize());
    EXPECT_EQ(31, deque.getFront());
    EXPECT_EQ(42, deque.getBack());

    deque.removeFront(); // {31 42h - 42t 20}
    deque.removeBack(); // {31h - - 42t 20}

    slice[1] = 35;
    EXPECT_FALSE(deque.append(slice)); // {31 20 35h 42t 20}
    EXPECT_EQ(0u, deque.getAvailableSpace());
    EXPECT_EQ(5u, deque.getSize());
    EXPECT_EQ(42, deque.getFront());
    EXPECT_EQ(35, deque.getBack());
}
