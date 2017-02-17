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
// ----------------------------------------------------------------------------

#include <unittest/harness.h>

#include <outpost/utils/fixed_ordered_map.h>

using namespace outpost;

struct Entry
{
    uint16_t mKey;
    uint32_t mValue;
};

TEST(FixedOrderedMapTest, createList)
{
    Entry entries[] = {
        Entry( { 1235,  0 } ),
        Entry( { 1236, 10 } ),
    };

    FixedOrderedMap<Entry, uint16_t> list(entries, 2);

    EXPECT_EQ(2U, list.getNumberOfElements());
}

TEST(FixedOrderedMapTest, createMapFromArrayWithoutExplicitSize)
{
    Entry entries[] = {
        Entry( {  1, 0 } ),
        Entry( {  3, 1 } ),
        Entry( {  5, 2 } ),
        Entry( { 17, 3 } ),
    };

    FixedOrderedMap<Entry, uint16_t> list(entries);

    EXPECT_EQ(4U, list.getNumberOfElements());
}


TEST(FixedOrderedMapTest, availableEntriesShouldBeReturnedByPointer)
{
    Entry entries[] = {
        Entry( {  1, 0 } ),
        Entry( {  3, 1 } ),
        Entry( {  5, 2 } ),
        Entry( { 17, 3 } ),
    };

    FixedOrderedMap<Entry, uint16_t> list(entries);

    EXPECT_EQ(0U, list.getEntry(1)->mValue);
    EXPECT_EQ(1U, list.getEntry(3)->mValue);
    EXPECT_EQ(2U, list.getEntry(5)->mValue);
    EXPECT_EQ(3U, list.getEntry(17)->mValue);
}

TEST(FixedOrderedMapTest, missingEntriesShouldReturnANullPointer)
{
    Entry entries[] = {
        Entry( {  1, 0 } ),
        Entry( {  3, 1 } ),
        Entry( {  5, 2 } ),
        Entry( { 17, 3 } ),
    };

    FixedOrderedMap<Entry, uint16_t> list(entries);

    EXPECT_EQ(0, list.getEntry(0));
    EXPECT_EQ(0, list.getEntry(2));
    EXPECT_EQ(0, list.getEntry(4));
    EXPECT_EQ(0, list.getEntry(35000));
}
