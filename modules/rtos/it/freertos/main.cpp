/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
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

#include <outpost/rtos/thread.h>
#include <outpost/rtos/queue.h>
#include <outpost/rtos/failure_handler.h>

#include <xpcc/architecture.hpp>
#include <xpcc/processing/rtos.hpp>

using namespace xpcc::stm32;

typedef GpioOutputD13 LedOrange;    // User LED 3
typedef GpioOutputD12 LedGreen; // User LED 4
typedef GpioOutputD14 LedRed;   // User LED 5
typedef GpioOutputD15 LedBlue;  // User LED 6

typedef GpioOutputA9 VBusPresent;   // green LED (LD7)
typedef GpioOutputD5 VBusOvercurrent;   // red LED (LD8)
typedef GpioInputA0 Button;
typedef GpioOutputA8 ClockOut;
typedef GpioOutputC9 SystemClockOut;

/// STM32F4 running at 168MHz (USB Clock at 48MHz) generated from the
/// external on-board 8MHz crystal
typedef SystemClock<Pll<ExternalCrystal<MHz8>, MHz168, MHz48> > DefaultSystemClock;

outpost::rtos::Queue<uint32_t> queue(100);

static void
failureHandler(outpost::rtos::FailureCode code)
{
    (void) code;

    LedRed::set();

    while (1) {
        // wait forever
    }
}

int
main(void)
{
    DefaultSystemClock::enable();

    LedOrange::setOutput(xpcc::Gpio::Low);
    LedGreen::setOutput(xpcc::Gpio::Low);
    LedRed::setOutput(xpcc::Gpio::Low);
    LedBlue::setOutput(xpcc::Gpio::Low);

    outpost::rtos::FailureHandler::setFailureHandlerFunction(&failureHandler);

    LedGreen::set();

    queue.send(1245);

    uint32_t value;
    queue.receive(value);

    xpcc::rtos::Scheduler::schedule();
    LedRed::set();
}
