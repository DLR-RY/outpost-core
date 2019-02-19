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

#ifndef OUTPOST_RTOS_FREERTOS_CLOCK_H
#define OUTPOST_RTOS_FREERTOS_CLOCK_H

#include <outpost/time/clock.h>
#include <outpost/time/time_point.h>

namespace outpost
{
namespace rtos
{
/**
 *
 * \ingroup    rtos
 */
class SystemClock : public time::Clock
{
public:
    virtual time::SpacecraftElapsedTime
    now() const override;
};

}  // namespace rtos
}  // namespace outpost

#endif
