/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include "producer.h"

Producer::Producer(cobc::rtos::Queue<uint32_t>& queue) :
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
        sleep(cobc::time::Milliseconds(500));

        mQueue.send(counter);
        counter++;
    }
}
