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

#ifndef OUTPOST_FREERTOS_THREAD_PRIORITIES_H
#define OUTPOST_FREERTOS_THREAD_PRIORITIES_H

#include <stdint.h>
#include <cstddef>

// FreeRTOS supports priorities between 0 and (configMAX_PRIORITIES - 1).
// Lower values represent a lower priority, 0 is the priority of the idle task
// and the overall lowest priority.
//
static inline uint8_t
toFreeRtosPriority(uint8_t priority, size_t numPriorities)
{
    const uint8_t stepWidth = 256 / numPriorities;

    uint8_t out = (priority / stepWidth);

    if (out > (numPriorities - 1)) {
        return (numPriorities - 1);
    }
    else {
        return out;
    }
}

static inline uint8_t
fromFreeRtosPriority(uint8_t priority, size_t numPriorities)
{
    const uint8_t stepWidth = 256 / numPriorities;
    const uint8_t offset = stepWidth / 2;

    return (priority * stepWidth + offset);
}

#endif
