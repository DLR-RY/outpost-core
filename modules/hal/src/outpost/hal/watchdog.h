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

#include <outpost/time/duration.h>

#ifndef OUTPOST_HAL_WATCHDOG_H_
#define OUTPOST_HAL_WATCHDOG_H_

namespace outpost
{
namespace hal
{
/**
 * Interface for an (hardware) watchdog
 */
class Watchdog
{
public:
    Watchdog() = default;
    virtual ~Watchdog() = default;

    /*
     * Resets the timer of the watchdog.
     *
     * @return true if reset was successful, false otherwise
     */
    virtual bool
    reset() = 0;

    /**
     * Returns the interval within which a reset must occur.
     * If reset is not called within this interval since the last
     * call to reset watchdog will trigger.
     *
     * @return the interval in which a reset must occur
     */
    virtual outpost::time::Duration
    getInterval() = 0;
};
}  // namespace hal
}  // namespace outpost

#endif
