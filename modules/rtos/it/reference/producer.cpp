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

#include "../reference/producer.h"

Producer::Producer(outpost::rtos::Queue<uint32_t>& queue) :
    Thread(20, defaultStackSize, "Consumer"),
    mQueue(queue)
{
}

void
Producer::run()
{
    uint32_t counter = 0;
    while (1)
    {
        sleep(outpost::time::Milliseconds(500));

        mQueue.send(counter);
        counter++;
    }
}
