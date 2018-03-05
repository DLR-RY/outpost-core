/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "failure_handler.h"

void
outpost::rtos::FailureHandler::fatal(FailureCode code)
{
    // forward call to handler function
    cleanup(code);
    handler(code);
}

void
outpost::rtos::FailureHandler::setFailureHandlerFunction(Handler newHandler)
{
    handler = newHandler;
}

void
outpost::rtos::FailureHandler::setCleanupHandlerFunction(Handler newHandler)
{
    cleanup = newHandler;
}
