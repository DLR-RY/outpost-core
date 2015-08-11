/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include <cobc/rtos/thread.h>
#include <cobc/rtos/queue.h>
#include <cobc/rtos/failure_handler.h>

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

cobc::rtos::Queue<uint32_t> queue(100);

static void
failureHandler(cobc::rtos::FailureCode code)
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

    cobc::rtos::FailureHandler::setFailureHandlerFunction(&failureHandler);

    LedGreen::set();

    queue.send(1245);

    uint32_t value;
    queue.receive(value);

    xpcc::rtos::Scheduler::schedule();
    LedRed::set();
}
