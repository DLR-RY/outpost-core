/*
 * Copyright (c) 2015-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015, 2017-2018, Fabian Greif (DLR RY-AVS)
 * - 2016, Jan Sommer (DLR SC-SRV)
 * - 2018, Annika Ofenloch (DLR RY-AVS)
 */

#include "heartbeat.h"

outpost::smpc::Topic<const outpost::support::Heartbeat> outpost::support::watchdogHeartbeat;
