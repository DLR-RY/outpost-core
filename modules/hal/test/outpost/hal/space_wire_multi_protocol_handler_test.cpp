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
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/hal/space_wire_multi_protocol_handler.h>
#include <outpost/rtos.h>
#include <outpost/support/heartbeat.h>

#include <unittest/hal/spacewire_stub.h>
#include <unittest/harness.h>

TEST(SpaceWireMultiProtocolHandlerTest, construct)
{
    char name[] = "test";
    outpost::rtos::SystemClock clock;
    unittest::hal::SpaceWireStub spw(4500);
    outpost::hal::SpaceWireMultiProtocolHandler<2> spwmp(
            spw, 1, 1024, name, outpost::support::parameter::HeartbeatSource::default0, clock);
    outpost::hal::SpaceWireMultiProtocolHandlerInterface& ref = spwmp;
    (void) ref;
}
