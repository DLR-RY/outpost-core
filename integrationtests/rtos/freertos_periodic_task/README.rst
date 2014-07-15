
FreeRTOS Test
=============

Tests the periodic execution of threads through the `cobc::rtos::RateMonotonicPeriod` class.

As of 2013-09-12.

Environment
-----------

This tests runs on a plain STM32F4Discovery Board with the four user LEDs.


Procedure
---------

Every 500ms the green LED toggles, while the blue LED is switch on during the first 20ms of that 500ms period. To achieve this a single thread with default priority is created. Within this thread two RateMonotonicPeriod instances control the execution time.

The first waits for the 500ms period and is always running. If this instance ever misses the period the orange LED is switch on and the normal operation is halted.

A second instance of RateMonotonicPeriod first initializes a 20ms period, switches the blue LED on and than wait for the period to end.
