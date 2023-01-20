/*
 * Copyright (c) 2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Adrian Roeser (DLR RY-AVS)
 */

#include <outpost/utils/container/reference_queue.h>

#include <unittest/harness.h>

#include <stdint.h>

using namespace outpost::utils;

TEST(reference_queue, status)
{
    ReferenceQueue<int, 3> refq1;

    EXPECT_TRUE(refq1.isEmpty());
    EXPECT_FALSE(refq1.isFull());

    int a = 1;
    int b = 2;
    int c = 3;

    EXPECT_TRUE(refq1.send(a));
    EXPECT_TRUE(refq1.send(b));
    EXPECT_TRUE(refq1.send(c));

    EXPECT_FALSE(refq1.isEmpty());
    EXPECT_TRUE(refq1.isFull());
}

TEST(reference_queue, sendingAndReceiving)
{
    ReferenceQueue<int, 3> refq1;

    EXPECT_TRUE(refq1.isEmpty());

    int sData1 = 1;
    int rData1;

    EXPECT_TRUE(refq1.send(sData1));

    EXPECT_EQ(1, refq1.getNumberOfItems());

    EXPECT_TRUE(refq1.receive(rData1, outpost::time::Seconds(1)));

    EXPECT_EQ(1, rData1);

    int sData2 = 2;
    int rData2;
    int sData3 = 3;
    int rData3;
    int sData4 = 4;
    int rData4;

    EXPECT_TRUE(refq1.send(sData2));
    // 2
    EXPECT_EQ(1, refq1.getNumberOfItems());

    EXPECT_TRUE(refq1.send(sData3));
    // 2,3
    EXPECT_EQ(2, refq1.getNumberOfItems());

    EXPECT_TRUE(refq1.receive(rData2));
    EXPECT_EQ(2, rData2);
    // 3
    EXPECT_EQ(1, refq1.getNumberOfItems());

    EXPECT_TRUE(refq1.send(sData4));
    // 3,4
    EXPECT_EQ(2, refq1.getNumberOfItems());

    EXPECT_TRUE(refq1.receive(rData3));  //==3
    EXPECT_EQ(3, rData3);
    // 4
    EXPECT_EQ(1, refq1.getNumberOfItems());

    EXPECT_TRUE(refq1.receive(rData4));
    EXPECT_EQ(4, rData4);
    EXPECT_EQ(0, refq1.getNumberOfItems());
}

TEST(reference_queue, errors)
{
    ReferenceQueue<int, 3> refq1;

    int a = 1;
    int b = 2;
    int c = 3;
    int x;

    EXPECT_FALSE(refq1.receive(x, outpost::time::Milliseconds(200)));

    EXPECT_TRUE(refq1.send(a, outpost::time::Milliseconds(200)));
    EXPECT_TRUE(refq1.send(b, outpost::time::Milliseconds(200)));
    EXPECT_TRUE(refq1.send(c, outpost::time::Milliseconds(200)));
    EXPECT_FALSE(refq1.send(a, outpost::time::Milliseconds(200)));

    EXPECT_FALSE(refq1.isEmpty());
    EXPECT_TRUE(refq1.isFull());
}
