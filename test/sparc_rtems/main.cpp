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

/**
 * \file
 * \author Fabian Greif
 *
 * \copyright German Aerospace Center (DLR)
 *
 * \brief RTEMS usage example for the Nexys3 (LEON3).
 */

#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include <fcntl.h>

#include <outpost/rtos/thread.h>

#include <bsp.h>
#include <rtems.h>

void
fatalErrorHandler(Internal_errors_Source source, bool isInternal, uint32_t errorCode);

rtems_extensions_table User_extensions = {
    NULL,    // task_create_extension,
    NULL,    // task_start_extension
    NULL,    // task_restart_extension
    NULL,    // task_delete_extension,
    NULL,    // task_switch_extension,
    NULL,    // task_begin_extension
    NULL,    // task_exitted_extension
    &fatalErrorHandler        // fatal_extension
};

#define CONFIGURE_INITIAL_EXTENSIONS    User_extensions

#define CONFIGURE_INIT
#include "system.h"

void
fatalErrorHandler(Internal_errors_Source source, bool isInternal, uint32_t errorCode)
{
    printf("Fatal error handler: %i, %i, %ld\n", source, isInternal, errorCode);
    while (1)
    {
    }
}


// ----------------------------------------------------------------------------
/*
 * Set up the environment.
 */
rtems_task
task_system_init(rtems_task_argument /*ignored*/)
{
    // NOTE:
    // apbuart0 - debug messages (only accessible via printk(...))
    // apbuart1 - receive/transmit tc and tm
    FILE* uart2 = fopen("/dev/apbuart1", "w");
    fprintf(uart2, "Hello World!!\r\n");

    printk("Hello World!!!\r\n");

    while (1)
    {

    }

    rtems_task_delete(RTEMS_SELF);
}
