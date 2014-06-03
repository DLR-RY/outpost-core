
#include "timing_test.h"

#include <cobc/time/duration.h>
#include <cobc/rtos/periodic_task_manager.h>

#include <iostream>

using namespace cobc;

TimingTest::TimingTest() :
    rtos::Thread(128, 4096, "TTet")
{
}

void
TimingTest::run()
{
    rtos::PeriodicTaskManager period;

    std::cout << "Start with period 500 ms" << std::endl;

    int count = 0;
    bool state = true;
    while (1)
    {
        if (period.nextPeriod(time::Milliseconds(500)) == rtos::PeriodicTaskManager::Status::timeout) {
            // Period missed
            std::cout << "Error: period missed!" << std::endl;
        }

        state = !state;

        if (state) {
            std::cout << "A" << std::endl;
        }
        else {
            std::cout << "B" << std::endl;
        }

        count++;
        if (count == 3) {
            std::cout << "wait 600 ms" << std::endl;
            sleep(time::Milliseconds(600));
        }
        if (count == 8) {
            std::cout << "wait 1200 ms" << std::endl;
            sleep(time::Milliseconds(1200));
        }
    }
}

#include "timing_test.h"

#include <cobc/time/duration.h>
#include <cobc/rtos/periodic_task_manager.h>

#include <iostream>

using namespace cobc;

TimingTest::TimingTest() :
    rtos::Thread(128, 4096, "TTet")
{
}

void
TimingTest::run()
{
    rtos::PeriodicTaskManager period;

    std::cout << "Start with period 500 ms" << std::endl;

    int count = 0;
    bool state = true;
    while (1)
    {
        if (period.nextPeriod(time::Milliseconds(500)) == rtos::PeriodicTaskManager::Status::timeout) {
            // Period missed
            std::cout << "Error: period missed!" << std::endl;
        }

        state = !state;

        if (state) {
            std::cout << "A" << std::endl;
        }
        else {
            std::cout << "B" << std::endl;
        }

        count++;
        if (count == 3) {
            std::cout << "wait 600 ms" << std::endl;
            sleep(time::Milliseconds(600));
        }
        if (count == 8) {
            std::cout << "wait 1200 ms" << std::endl;
            sleep(time::Milliseconds(1200));
        }
    }
}
