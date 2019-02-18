/*
 * Copyright (c) 2014-2017, 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, 2019, Fabian Greif (DLR RY-AVS)
 */

#include <stdint.h>
#include <stdio.h>

// POSIX specific
#include <outpost/rtos/internal/time.h>

#include "../reference/consumer.h"
#include "../reference/producer.h"

outpost::rtos::Queue<uint32_t> queue(10);

Producer producer(queue);
Consumer consumer(queue);

int
main(void)
{
    timespec relative = outpost::rtos::toRelativeTime(outpost::time::Duration::infinity());
    printf("%lu %li\n", relative.tv_sec, relative.tv_nsec);

    timespec negative = outpost::rtos::toRelativeTime(-outpost::time::Milliseconds(1500));
    printf("%li %li\n", negative.tv_sec, negative.tv_nsec);

    timespec absolute = outpost::rtos::toAbsoluteTime(outpost::time::Duration::infinity());
    printf("%lu %li\n", absolute.tv_sec, absolute.tv_nsec);

    producer.start();
    consumer.start();

	while (1)
	{
	    consumer.waitForNewValue();
	    uint32_t value = consumer.getCurrentValue();

	    printf("value: %i\n", static_cast<int>(value));
	}
}
