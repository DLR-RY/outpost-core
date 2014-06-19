/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <cobc/rtos/failure_handler.h>

#include <stdio.h>
#include <cstdlib>

#include <inttypes.h>

static void
defaultHandler(cobc::rtos::FailureCode code)
{
    //printf("Fatal Handler: %"PRIu32"\n", code.getCode());
    printf("Fatal Handler: 0x%08X\n", static_cast<int>(code.getCode()));
    exit(1);
}

cobc::rtos::FailureHandler::Handler cobc::rtos::FailureHandler::handler = &defaultHandler;
