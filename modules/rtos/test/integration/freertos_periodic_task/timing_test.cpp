
#include "timing_test.h"

#include <cobc/time/duration.h>
#include <cobc/rtos/rate_monotonic_period.h>

#include "hardware.h"

using namespace cobc;

TimingTest::TimingTest() :
	rtos::Thread(128, 4096, "TTet")
{
}

void
TimingTest::run()
{
	bool state = false;
	rtos::RateMonotonicPeriod period;
	rtos::RateMonotonicPeriod period2;
	while (1)
	{
		if (period.nextPeriod(time::Milliseconds(500)) == rtos::RateMonotonicPeriod::TIMEOUT) {
			// Period missed
			break;
		}

		state = !state;

		// Start a new period of 20 ms
		period2.nextPeriod(time::Milliseconds(20));
		LedBlue::set();

		// Wait for the previous period to end
		period2.nextPeriod(time::Milliseconds(20));
		LedBlue::reset();

		period2.cancel();

		if (state) {
			LedGreen::set();
		}
		else {
			LedGreen::reset();
		}
	}

	LedOrange::set();
	while (1) {
		// DO SOMETHING
	}
}
