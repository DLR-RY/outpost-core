/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_FREERTOS_CLOCK_H
#define COBC_RTOS_FREERTOS_CLOCK_H

#include <cobc/time/time_point.h>

namespace cobc
{
    namespace rtos
    {
        /**
         *
         * \ingroup    rtos
         */
        class Clock
        {
        public:
            static time::TimePoint
            now();
        };

#ifdef UNITTEST
        class TestingClock
        {
        public:
            static void
            setTime(time::TimePoint time);
        };
#endif
    }
}

#endif // COBC_RTOS_FREERTOS_CLOCK_H
