/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <stdio.h>
#include <unistd.h>

#include <cobc/rtos/timer.h>

#include "timing_test.h"

static TimingTest timingTest;

int
main(void)
{
	timingTest.start();
	sleep(10);

	return 0;
}
