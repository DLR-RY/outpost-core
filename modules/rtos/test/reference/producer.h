/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef PRODUCER_H
#define PRODUCER_H

#include <cobc/rtos/thread.h>
#include <cobc/rtos/queue.h>

class Producer : public cobc::rtos::Thread
{
public:
    Producer(cobc::rtos::Queue<uint32_t>& queue);

private:
    virtual void
    run();

    cobc::rtos::Queue<uint32_t>& mQueue;
};

#endif // PRODUCER_H
