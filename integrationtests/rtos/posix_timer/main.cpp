/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <stdio.h>
#include <unistd.h>

#include <cobc/rtos/timer.h>

using namespace cobc;

class TimerTest
{
public:
    TimerTest() :
        timer1(this, &TimerTest::test),
        timer2(this, &TimerTest::test2)
    {
    }

    void
    start()
    {
        timer1.start(time::Milliseconds(900));
        timer2.start(time::Milliseconds(2000));
    }

    void
    test(rtos::Timer * timer)
    {
        printf("trigger1\n");
        timer->reset();
    }

    void
    test2(rtos::Timer * timer)
    {
        (void) timer;
        printf("trigger2\n");

        timer1.cancel();
    }

private:
    rtos::Timer timer1;
    rtos::Timer timer2;
};

int
main(void)
{
    TimerTest test;

    test.start();
    printf("sleep\n");
    sleep(5);
    printf("done\n");

    return 0;
}
