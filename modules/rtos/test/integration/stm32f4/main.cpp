/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <cobc/rtos/thread.h>

using namespace cobc;

class Thread : public rtos::Thread
{
public:
    Thread() :
        rtos::Thread(1)
    {
    }

private:
    virtual void
    run()
    {
        while (1) {
            yield();
        }
    }
};

Thread thread;

int
main(void)
{
    thread.start();

    return 0;
}
