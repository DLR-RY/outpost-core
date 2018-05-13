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

#ifndef PRODUCER_H
#define PRODUCER_H

#include <outpost/rtos/thread.h>
#include <outpost/rtos/queue.h>

class Producer : public outpost::rtos::Thread
{
public:
    Producer(outpost::rtos::Queue<uint32_t>& queue);

private:
    virtual void
    run();

    outpost::rtos::Queue<uint32_t>& mQueue;
};

#endif // PRODUCER_H
