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

#include <cobc/smpc/topic.h>

#include <unittest/harness.h>

/**
 * Check that it is possible to compile topic with selected types.
 */
TEST(TopicTypeTest, testInteger)
{
    cobc::smpc::Topic<const uint16_t> keepAlive;

    keepAlive.publish(123);

    uint16_t apid = 234;
    keepAlive.publish(apid);
}

TEST(TopicTypeTest, testArray)
{
    static const size_t numberOfSunSensors = 10;

    cobc::smpc::Topic<const uint16_t[numberOfSunSensors]> sunSensors;

    uint16_t values[10] = {};

    sunSensors.publish(values);
}

TEST(TopicTypeTest, testPowerStruct)
{
    struct PowerState
    {
        bool device1;
        bool device2;
    };

    cobc::smpc::Topic<const PowerState> powerState;

    PowerState p;
    p.device1 = true;
    p.device2 = false;

    powerState.publish(p);
}
