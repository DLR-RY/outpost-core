/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
