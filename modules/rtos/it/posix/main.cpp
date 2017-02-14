/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>

#include <../reference/consumer.h>
#include <../reference/producer.h>

outpost::rtos::Queue<uint32_t> queue(10);

Producer producer(queue);
Consumer consumer(queue);

int
main(void)
{
    producer.start();
    consumer.start();

	while (1)
	{
	    consumer.waitForNewValue();
	    uint32_t value = consumer.getCurrentValue();

	    printf("value: %i\n", static_cast<int>(value));
	}
}
