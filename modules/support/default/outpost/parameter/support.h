/*
 * Copyright (c) 2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2018, Annika Ofenloch (DLR RY-AVS)
 * - 2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_SUPPORT_PARAMETER_SUPPORT_H
#define OUTPOST_SUPPORT_PARAMETER_SUPPORT_H

#include <stdint.h>
#include <outpost/time/duration.h>

namespace outpost
{
namespace support
{
namespace parameter
{

/**
 * List of sources of heartbeat ticks.
 *
 * Needs to be adapted by the project to include all thread instances. Threads
 * Instantiated multiple times should each have their own entry.
 *
 * The enumerators should be automatically defined. Otherwise they must follow
 * the rules of the automatic generation (start at zero and always increment by
 * one with each value being unique). In case of gaps in the enumerator
 * identifiers the automatic allocation of resources for each heartbeat source
 * does **not** work.
 */
enum class HeartbeatSource : uint16_t
{
    default0,
    default1,

    /// Dummy entry to detect the number of heartbeat sources. The value
    /// is automatically set by the compiler.
    lastId
};

/**
 * Default heartbeat interval.
 *
 * Used as the polling interval for threads waiting on resources.
 *
 * \see outpost::support::Heartbeat for a more detailed description of the
 *      heartbeat timing.
 */
constexpr outpost::time::Duration defaultHeartbeatInterval = outpost::time::Seconds(10);

/**
 * Tolerance for timing jitter in the heartbeat generation.
 *
 * \see outpost::support::Heartbeat for a more detailed description of the
 *      heartbeat timing.
 */
constexpr outpost::time::Duration heartbeatTolerance = outpost::time::Seconds(1);

} // namespace parameter
} // namespace support
} // namespace outpost

#endif
