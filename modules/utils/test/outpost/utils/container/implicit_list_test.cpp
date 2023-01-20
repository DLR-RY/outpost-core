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

#include <outpost/utils/container/implicit_list.h>

#include <unittest/harness.h>

#include <inttypes.h>
#include <stdio.h>

using namespace outpost;

class IntElement;

struct IntList
{
    friend class IntElement;

    IntElement* anchor = nullptr;
};

class IntElement : public outpost::ImplicitList<IntElement>
{
public:
    int value;

    explicit IntElement(IntList& list) : ImplicitList(list.anchor, this), value(0)
    {
    }
};

TEST(ImplicitListTest, getNext)
{
    IntList list1;

    IntElement e1(list1);
    IntElement e2(list1);
    IntElement e3(list1);

    EXPECT_EQ(&e2, e3.getNext());
    EXPECT_EQ(&e1, e2.getNext());
    EXPECT_EQ(nullptr, e1.getNext());
}

TEST(ImplicitListTest, removeFromList)
{
    IntList list1;

    IntElement e1(list1);
    IntElement e2(list1);
    IntElement e3(list1);

    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, &e2);

    EXPECT_EQ(&e1, e3.getNext());

    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, &e1);

    IntElement e4(list1);
    EXPECT_EQ(&e3, e4.getNext());

    IntElement e5(list1);
    IntElement e6(list1);

    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, &e6);
    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, &e3);

    EXPECT_EQ(nullptr, e4.getNext());
    EXPECT_EQ(&e4, e5.getNext());

    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, &e4);
    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, &e5);

    // removing from empty list is ok
    outpost::ImplicitList<IntElement>::removeFromList(&list1.anchor, nullptr);
}
