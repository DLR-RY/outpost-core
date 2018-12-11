/*
 * Copyright (c) 2015-2018, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the Eu:CROPIS flight software.
 *
 * Authors:
 * - 2015, 2017-2018, Fabian Greif (DLR RY-AVS)
 * - 2016, Jan Sommer (DLR SC-SRV)
 * - 2018, Annika Ofenloch (DLR RY-AVS)
 */

#include "heartbeat.h"

outpost::smpc::Topic<const outpost::support::Heartbeat> outpost::support::watchdogHeartbeat;
