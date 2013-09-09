
#include <unittest/harness.h>

#include <cobc/rtos/freertos/thread_priorities.h>

/*
 * FreeRTOS supports priorities between 0 and (configMAX_PRIORITIES - 1).
 * Lower values represent a lower priority, 0 is the priority of the idle task
 * and the overall lowest priority.
 */

TEST(FreeRtosThreadPriorities, toFreeRtos)
{
	// 0..255 -> 0..4
	//
	// stepWidth = 51
	EXPECT_EQ(0, toFreeRtosPriority(0, 5));
	EXPECT_EQ(0, toFreeRtosPriority(16, 5));
	EXPECT_EQ(0, toFreeRtosPriority(32, 5));
	EXPECT_EQ(0, toFreeRtosPriority(48, 5));
	EXPECT_EQ(1, toFreeRtosPriority(64, 5));
	EXPECT_EQ(1, toFreeRtosPriority(80, 5));
	EXPECT_EQ(1, toFreeRtosPriority(96, 5));
	EXPECT_EQ(2, toFreeRtosPriority(112, 5));
	EXPECT_EQ(2, toFreeRtosPriority(128, 5));
	EXPECT_EQ(2, toFreeRtosPriority(144, 5));
	EXPECT_EQ(3, toFreeRtosPriority(160, 5));
	EXPECT_EQ(3, toFreeRtosPriority(176, 5));
	EXPECT_EQ(3, toFreeRtosPriority(192, 5));
	EXPECT_EQ(4, toFreeRtosPriority(224, 5));
	EXPECT_EQ(4, toFreeRtosPriority(255, 5));

	// 0..255 -> 0..127
	//
	// stepWidth = 2
	EXPECT_EQ(0, toFreeRtosPriority(0, 128));
	EXPECT_EQ(8, toFreeRtosPriority(16, 128));
	EXPECT_EQ(16, toFreeRtosPriority(32, 128));
	EXPECT_EQ(24, toFreeRtosPriority(48, 128));
	EXPECT_EQ(32, toFreeRtosPriority(64, 128));
	EXPECT_EQ(40, toFreeRtosPriority(80, 128));
	EXPECT_EQ(48, toFreeRtosPriority(96, 128));
	EXPECT_EQ(56, toFreeRtosPriority(112, 128));
	EXPECT_EQ(64, toFreeRtosPriority(128, 128));
	EXPECT_EQ(72, toFreeRtosPriority(144, 128));
	EXPECT_EQ(80, toFreeRtosPriority(160, 128));
	EXPECT_EQ(88, toFreeRtosPriority(176, 128));
	EXPECT_EQ(96, toFreeRtosPriority(192, 128));
	EXPECT_EQ(112, toFreeRtosPriority(224, 128));
	EXPECT_EQ(127, toFreeRtosPriority(255, 128));
}

TEST(FreeRtosThreadPriorities, fromFreeRtos)
{
	// 0..4 -> 0..255
	//
	// stepWidth = 51
	// offset    = 25
	EXPECT_EQ( 25, fromFreeRtosPriority(0, 5));
	EXPECT_EQ( 76, fromFreeRtosPriority(1, 5));
	EXPECT_EQ(127, fromFreeRtosPriority(2, 5));
	EXPECT_EQ(178, fromFreeRtosPriority(3, 5));
	EXPECT_EQ(229, fromFreeRtosPriority(4, 5));
}
