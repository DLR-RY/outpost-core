
#include "timing_test.h"

#include <cobc/time/duration.h>
#include <cobc/hal/nexys3/sevensegment.h>

#include <cobc/rtos/periodic_task_manager.h>

#include <stdio.h>
#include <stdlib.h>

using namespace cobc;

TimingTest::TimingTest() :
	rtos::Thread(128, 4096, "TTet")
{
}

void
TimingTest::run()
{
	rtos::PeriodicTaskManager period;

	bool state = true;
	while (1)
	{
		if (period.nextPeriod(time::Milliseconds(500)) == rtos::PeriodicTaskManager::TIMEOUT) {
			// Period missed
			break;
		}

		state = !state;

		if (state) {
			nexys3::SevenSegment::write(0, 'A');
		}
		else {
			nexys3::SevenSegment::write(0, 'B');
		}
	}

	// DO SOMETHING
}
