/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <stdio.h>
#include <unistd.h>

#include <cobc/rtos/clock.h>
#include <cobc/rtos/thread.h>

using namespace cobc::rtos;
using namespace cobc::time;

class TimeThread : public Thread
{
public:
    TimeThread() :
        Thread(1)
    {
    }

    virtual void
    run()
    {
        TimePoint t1 = cobc::rtos::Clock::now();
        sleep(Milliseconds(200));
        TimePoint t2 = cobc::rtos::Clock::now();
        sleep(Milliseconds(2000));
        TimePoint t3 = cobc::rtos::Clock::now();

        Duration d1 = t2 - t1;
        Duration d2 = t3 - t2;

        printf("t1 %lu\n", t1.asMicroseconds());
        printf("t2 %lu\n", t2.asMicroseconds());
        printf("t3 %lu\n", t3.asMicroseconds());

        printf("d1 %lu\n", d1.milliseconds());
        printf("d2 %lu\n", d2.milliseconds());
    }
};

TimeThread thread;

int
main(void)
{
    thread.start();

    TimePoint now = cobc::rtos::Clock::now();
    printf("timepoint : %lu\n", now.asMicroseconds());

    uint64_t seconds = now.asMicroseconds() / 1000000;
    printf("seconds   : %lu\n", seconds);

    while (1)
    {
        sleep(1);
    }

    return 0;
}
