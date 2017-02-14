/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#include <outpost/rtos/failure_handler.h>

#include <rtems.h>

void
defaultHandler(outpost::rtos::FailureCode code);

void
defaultHandler(outpost::rtos::FailureCode code)
{
    rtems_fatal_error_occurred(code.getCode());
}

outpost::rtos::FailureHandler::Handler outpost::rtos::FailureHandler::handler = &defaultHandler;

