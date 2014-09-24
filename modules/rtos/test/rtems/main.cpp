/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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

#include <cobc/rtos/thread.h>

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
task_system_init(rtems_task_argument ignored)
{
    (void) ignored;

    // -- manual driver manager startup initialization
    // register our apbuart driver substitute
    //apbuart_register_drv();
    // manually register root bus and initialize driver manager
    //ambapp_grlib_root_register(&grlib_bus_config);
    //drvmgr_init();

    // NOTE:
    // apbuart0 - debug messages
    // apbuart1 - receive/transmit tc and tm
    //fclose(stderr);
    //stderr = fopen("/dev/apbuart0", "w");
    //fprintf(stderr,"\n");

    rtems_task_delete(RTEMS_SELF);
}
