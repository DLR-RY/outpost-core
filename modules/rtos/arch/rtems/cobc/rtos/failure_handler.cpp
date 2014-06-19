/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <cobc/rtos/failure_handler.h>

#include <rtems.h>

defaultHandler(cobc::rtos::FailureCode code)
{
    rtems_fatal_error_occurred(code.getCode());
}

cobc::rtos::FailureHandler::Handler cobc::rtos::FailureHandler::handler = &defaultHandler;
