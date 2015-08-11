/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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
#include <cobc/utils/deque.h>

TEST(Deque, forward)
{
    int16_t buffer[3];
    cobc::Deque<int16_t> deque(buffer, 3);
    
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

TEST(Deque, backward)
{
    int16_t buffer[3];
    cobc::Deque<int16_t> deque(buffer, 3);
    
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

TEST(Deque, both)
{
    int16_t buffer[3];
    cobc::Deque<int16_t> deque(buffer, 3);
    
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

TEST(Deque, clear)
{
    int16_t buffer[3];
    cobc::Deque<int16_t> deque(buffer, 3);
    
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
