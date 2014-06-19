/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "failure_handler.h"

void
cobc::rtos::FailureHandler::fatal(FailureCode code)
{
    // forward call to handler function
    handler(code);
}

void
cobc::rtos::FailureHandler::setFailureHandlerFunction(Handler newHandler)
{
    handler = newHandler;
}
