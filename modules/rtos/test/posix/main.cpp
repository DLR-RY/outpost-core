
#include <stdint.h>
#include <stdio.h>

#include "producer.h"
#include "consumer.h"

cobc::rtos::Queue<uint32_t> queue(10);

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
