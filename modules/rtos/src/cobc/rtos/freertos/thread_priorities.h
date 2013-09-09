
#ifndef COBC_FREERTOS_THREAD_PRIORITIES_H
#define COBC_FREERTOS_THREAD_PRIORITIES_H

#include <stdint.h>
#include <cstddef>

/*
 * FreeRTOS supports priorities between 0 and (configMAX_PRIORITIES - 1).
 * Lower values represent a lower priority, 0 is the priority of the idle task
 * and the overall lowest priority.
 */
static inline uint8_t
toFreeRtosPriority(uint8_t priority, std::size_t configMAX_PRIORITIES)
{
	const uint8_t stepWidth = 256 / configMAX_PRIORITIES;

	uint8_t out = (priority / stepWidth);

	if (out > (configMAX_PRIORITIES - 1)) {
		return (configMAX_PRIORITIES - 1);
	}
	else {
		return out;
	}
}

static inline uint8_t
fromFreeRtosPriority(uint8_t priority, std::size_t configMAX_PRIORITIES)
{
	const uint8_t stepWidth = 256 / configMAX_PRIORITIES;
	const uint8_t offset = stepWidth / 2;

	return (priority * stepWidth + offset);
}

#endif // COBC_FREERTOS_THREAD_PRIORITIES_H
