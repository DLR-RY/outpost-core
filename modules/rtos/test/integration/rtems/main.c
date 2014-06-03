/*
 * A simple rtems program to show how to use configuration templates
 */

#define CONFIGURE_INIT
#include "system.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Macro to hide the ugliness of printing the time.
#define print_time(_s1, _tb, _s2) \
  do { \
    printf( "%s%02ld:%02ld:%02ld   %02ld/%02ld/%04ld%s", \
       _s1, (_tb)->hour, (_tb)->minute, (_tb)->second, \
       (_tb)->month, (_tb)->day, (_tb)->year, _s2 ); \
    fflush(stdout); \
  } while ( 0 )

// Macro to print an task name that is composed of ASCII characters.
#define put_name( _name, _crlf ) \
  do { \
    uint32_t c0, c1, c2, c3; \
    \
    c0 = ((_name) >> 24) & 0xff; \
    c1 = ((_name) >> 16) & 0xff; \
    c2 = ((_name) >> 8) & 0xff; \
    c3 = (_name) & 0xff; \
    putchar( (char)c0 ); \
    if ( c1 ) putchar( (char)c1 ); \
    if ( c2 ) putchar( (char)c2 ); \
    if ( c3 ) putchar( (char)c3 ); \
    if ( (_crlf) ) \
      putchar( '\n' ); \
  } while (0)

/**
 *  static inline routine to make obtaining ticks per second easier.
 */

static inline uint32_t get_ticks_per_second( void )
{
  rtems_interval ticks_per_second;
  (void) rtems_clock_get( RTEMS_CLOCK_GET_TICKS_PER_SECOND, &ticks_per_second );  return ticks_per_second;
}


/**
 *  This allows us to view the "Test_task" instantiations as a set
 *  of numbered tasks by eliminating the number of application
 *  tasks created.
 *
 *  In reality, this is too complex for the purposes of this
 *  example.  It would have been easier to pass a task argument. :)
 *  But it shows how rtems_id's can sometimes be used.
 */

#define task_number( tid ) \
  ( rtems_object_id_get_index( tid ) - \
     rtems_configuration_get_rtems_api_configuration()->number_of_initialization_tasks )

/**
 *  Keep the names and IDs in global variables so another task can use them.
 */
rtems_id   Task_id[4];         // array of task ids 
rtems_name Task_name[4];       // array of task names 

enum SevenSegment
{
    A = (1 << 0),
    B = (1 << 1),
    C = (1 << 2),
    D = (1 << 3),
    E = (1 << 4),
    F = (1 << 5),
    G = (1 << 6),
    H = (1 << 7),
};

static const uint8_t numbers[10] =
{
    A | B | C | D | E | F,        // 0
        B | C,                    // 1
    A | B |     D | E |     G,    // 2
    A | B | C | D |         G,    // 3
        B | C |         F | G,    // 4
    A |     C | D |     F | G,    // 5
    A |     C | D | E | F | G,    // 6
    A | B | C,                    // 7
    A | B | C | D | E | F | G,    // 8
    A | B | C | D |     F | G,    // 9
};

rtems_task
task_test(rtems_task_argument unused);

rtems_task
task_test(rtems_task_argument unused)
{
    rtems_id tid;
    rtems_time_of_day time;
    uint32_t task_index;
    rtems_status_code status;

    status = rtems_task_ident(RTEMS_SELF, RTEMS_SEARCH_ALL_NODES, &tid);
    task_index = task_number( tid );
    for (;;) {
        status = rtems_clock_get(RTEMS_CLOCK_GET_TOD, &time);
        if (time.second >= 3335) {
            puts("*** END OF CLOCK TICK TEST ***");
            exit(0);
        }
        put_name( Task_name[ task_index ], FALSE);
        print_time( " - rtems_clock_get - ", &time, "\n");
        status = rtems_task_wake_after(task_index * 5 * get_ticks_per_second());
    }
}

rtems_task
task_system_init(rtems_task_argument ignored)
{
    printf( "Hello World\n" );
    //rtems_task_wake_after(100);
    printk( "Hello World over printk() on Debug console\n\n" );

    rtems_status_code status;
    rtems_time_of_day time;

    puts( "\n\n*** CLOCK TICK TEST ***" );

    time.year   = 1988;
    time.month  = 12;
    time.day    = 31;
    time.hour   = 9;
    time.minute = 0;
    time.second = 0;
    time.ticks  = 0;

    status = rtems_clock_set( &time );

    Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );
    Task_name[ 2 ] = rtems_build_name( 'T', 'A', '2', ' ' );
    Task_name[ 3 ] = rtems_build_name( 'T', 'A', '3', ' ' );

    status = rtems_task_create(
    Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ]
    );
    status = rtems_task_create(
    Task_name[ 2 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 2 ]
    );
    status = rtems_task_create(
    Task_name[ 3 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 3 ]
    );

    status = rtems_task_start( Task_id[ 1 ], task_test, 1 );
    status = rtems_task_start( Task_id[ 2 ], task_test, 2 );
    status = rtems_task_start( Task_id[ 3 ], task_test, 3 );

    // -----------------
    // print "dlr" on the seven segment display
    (*(uint32_t *)(0x80000300)) = 0x5e385000;

    for (uint_fast8_t i = 0; i < 10; ++i) {
        rtems_task_wake_after(100);

        (*(uint32_t *)(0x80000300)) = 0x5e385000 | numbers[i % 10];
    }

    rtems_task_wake_after(100);
    (*(uint32_t *)(0x80000300)) = 0x5e385000;
    // -----------------

    status = rtems_task_delete( RTEMS_SELF );
}
