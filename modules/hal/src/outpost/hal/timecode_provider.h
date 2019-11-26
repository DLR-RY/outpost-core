/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
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

#ifndef OUTPOST_HAL_TIMECODE_PROVIDER_H_
#define OUTPOST_HAL_TIMECODE_PROVIDER_H_

#include "timecode.h"

#include <outpost/rtos/queue.h>

namespace outpost
{
namespace hal
{
class TimeCodeProvider
{
public:
    TimeCodeProvider() = default;
    virtual ~TimeCodeProvider() = default;

    /**
     * Add a listener for timecode
     * @param queue the queue to add
     * @return false if queue == nullptr or all places for Listener are filled
     */
    virtual bool
    addTimeCodeListener(outpost::rtos::Queue<TimeCode>* queue) = 0;
};

}  // namespace hal
}  // namespace outpost

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_TIMECODE_PROVIDER_H_ */
