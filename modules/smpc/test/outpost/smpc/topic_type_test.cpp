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

#include <outpost/smpc/topic.h>

#include <unittest/harness.h>

/**
 * Check that it is possible to compile topic with selected types.
 */
TEST(TopicTypeTest, testInteger)
{
    outpost::smpc::Topic<const uint16_t> keepAlive;

    keepAlive.publish(123);

    uint16_t apid = 234;
    keepAlive.publish(apid);
}

TEST(TopicTypeTest, testArray)
{
    static const size_t numberOfSunSensors = 10;

    outpost::smpc::Topic<const uint16_t[numberOfSunSensors]> sunSensors;

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

    outpost::smpc::Topic<const PowerState> powerState;

    PowerState p;
    p.device1 = true;
    p.device2 = false;

    powerState.publish(p);
}
