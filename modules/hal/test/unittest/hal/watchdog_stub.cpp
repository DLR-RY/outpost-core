/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#include "watchdog_stub.h"

#include <unittest/time/testing_clock.h>

// Just to have somewhere where that one is instantiated as otherwise
// syntax error might be missed
static unittest::time::TestingClock clk;
static unittest::hal::WatchdogStub<10> watchdog(clk);
