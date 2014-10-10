/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
// ----------------------------------------------------------------------------

#include <cobc/rtos/failure_handler.h>

static void
defaultHandler(cobc::rtos::FailureCode code)
{
    // Avoid warnings about unused parameters.
    (void) code;
}

cobc::rtos::FailureHandler::Handler cobc::rtos::FailureHandler::handler = &defaultHandler;
