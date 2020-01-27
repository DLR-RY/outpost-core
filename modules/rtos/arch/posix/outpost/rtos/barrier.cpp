/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include "barrier.h"

#include <outpost/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
outpost::rtos::Barrier::Barrier(uint32_t numberOfThreads)
{
    pthread_barrier_init(&mBarrier, nullptr, numberOfThreads);
}

outpost::rtos::Barrier::~Barrier()
{
    pthread_barrier_destroy(&mBarrier);
}

void
outpost::rtos::Barrier::wait()
{
    pthread_barrier_wait(&mBarrier);
}
