/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
