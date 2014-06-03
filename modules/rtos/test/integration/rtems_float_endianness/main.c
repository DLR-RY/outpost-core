/*
 * A simple rtems program to show how to use configuration templates
 */

#define CONFIGURE_INIT
#include "system.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

rtems_task
task_system_init(rtems_task_argument ignored)
{
    (void) ignored;

    printf("\nEndianness Test\n\n");

    uint32_t i = 133124;
    printf("Test integer: %"PRIu32"\n", i);

    const uint8_t* ptr = (const uint8_t *) &i;

    printf("byte array  : %02X:%02X:%02X:%02X\n", ptr[0], ptr[1], ptr[2], ptr[3]);

    if (ptr[0] == 0x00 && ptr[1] == 0x02 && ptr[2] == 0x08 && ptr[3] == 0x04) {
        printf("-> Big endian\n");
    }
    else if (ptr[0] == 0x04 && ptr[1] == 0x08 && ptr[2] == 0x02 && ptr[3] == 0x00) {
        printf("-> Little endian\n");
    }
    else {
        printf("-> Invalid encoding!\n");
    }
    printf("\n");

    float f = 3.14159;

    printf("Test float  : %f\n", f);
    ptr = (const uint8_t *) &f;

    printf("byte array  : %02X:%02X:%02X:%02X\n", ptr[0], ptr[1], ptr[2], ptr[3]);

    if (ptr[0] == 0x40 && ptr[1] == 0x49 && ptr[2] == 0x0F && ptr[3] == 0xD0) {
        printf("-> Big endian\n");
    }
    else if (ptr[0] == 0xD0 && ptr[1] == 0x0F && ptr[2] == 0x49 && ptr[3] == 0x40) {
        printf("-> Little endian\n");
    }
    else {
        printf("-> Invalid encoding!\n");
    }
    printf("\n");

    while (1) {
        // Wait for the output to finish before ending this thread.
        rtems_task_wake_after(1000);
        printf("Finished!\n");
    }
    rtems_task_delete( RTEMS_SELF );
}
