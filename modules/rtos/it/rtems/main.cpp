/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
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
 * \brief RTEMS usage example for the Nexys3 (LEON3).
 */

#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include <fcntl.h>

#include <cobc/rtos/thread.h>

#include <bsp.h>
#include <rtems.h>

#include <apbuart.h>

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
#include <../../it/rtems/system.h>

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
    // -- manual driver manager startup initialization
    // register our apbuart driver substitute
    apbuart_register_drv();
    // manually register root bus and initialize driver manager
    ambapp_grlib_root_register(&grlib_bus_config);
    drvmgr_init();

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
