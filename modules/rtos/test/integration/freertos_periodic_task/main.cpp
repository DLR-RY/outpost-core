/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
/**
 * \file
 * \author Fabian Greif
 *
 * \copyright German Aerospace Center (DLR)
 *
 * \brief FreeRTOS example for the STM32F4Discovery Board.
 *
 * Uses the xpcc library to provide the STM32F4xx hardware drivers.
 */

#include <cobc/rtos.h>

#include <xpcc/workflow/rtos.hpp>

#include "hardware.h"
#include "timing_test.h"

using namespace xpcc::stm32;

static bool
initClock()
{
    // use external 8MHz crystal
    if (!Clock::enableHse(Clock::HseConfig::HSE_CRYSTAL)) {
        return false;
    }

    Clock::enablePll(Clock::PllSource::PLL_HSE, 4, 168);
    return Clock::switchToPll();
}

static void
failureHandler(cobc::rtos::FailureCode code)
{
    (void) code;

    LedRed::set();

    while (1) {
        // wait forever
    }
}

// ----------------------------------------------------------------------------
static TimingTest timingTest;

// ----------------------------------------------------------------------------
int
main(void)
{
    initClock();

    LedOrange::setOutput(xpcc::gpio::LOW);
    LedGreen::setOutput(xpcc::gpio::LOW);
    LedRed::setOutput(xpcc::gpio::LOW);
    LedBlue::setOutput(xpcc::gpio::LOW);

    cobc::rtos::FailureHandler::setFailureHandlerFunction(&failureHandler);

    timingTest.start();

    LedGreen::set();

    xpcc::rtos::Scheduler::schedule();
    LedRed::set();
}
