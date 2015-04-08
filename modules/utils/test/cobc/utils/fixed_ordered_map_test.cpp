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

#include <cobc/utils/fixed_ordered_map.h>

using namespace cobc;

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

    EXPECT_EQ(2, list.getNumberOfEntries());
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

    EXPECT_EQ(4, list.getNumberOfEntries());
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
