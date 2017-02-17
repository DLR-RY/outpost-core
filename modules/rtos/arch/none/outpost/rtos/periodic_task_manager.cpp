/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "periodic_task_manager.h"

using namespace outpost::rtos;

PeriodicTaskManager::PeriodicTaskManager()
{
}

PeriodicTaskManager::~PeriodicTaskManager()
{
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::nextPeriod(time::Duration period)
{
    (void) period;
    return Status::idle;
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::status()
{
    return Status::idle;
}

void
PeriodicTaskManager::cancel()
{
}
