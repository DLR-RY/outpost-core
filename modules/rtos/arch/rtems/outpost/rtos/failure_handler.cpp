/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2018, Fabian Greif (DLR RY-AVS)
 * - 2018, Jan Sommer (DLR SC-SRV)
 */

#include <rtems.h>

#include <outpost/rtos/failure_handler.h>

void
defaultFatalHandler(outpost::rtos::FailureCode code);

void
defaultFatalHandler(outpost::rtos::FailureCode code)
{
    rtems_fatal_error_occurred(code.getCode());
}

void
defaultCleanupHandler(outpost::rtos::FailureCode code);

void
defaultCleanupHandler(outpost::rtos::FailureCode /*code*/)
{
}

outpost::rtos::FailureHandler::Handler outpost::rtos::FailureHandler::handler =
        &defaultFatalHandler;
outpost::rtos::FailureHandler::Handler outpost::rtos::FailureHandler::cleanup =
        &defaultCleanupHandler;
