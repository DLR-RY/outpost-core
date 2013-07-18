
include_directories(.)
include_directories(${cobc_library_path}/time/trunk/src)
include_directories(${cobc_library_path}/utils/trunk/src)

set(src
    cobc/rtos/failure_handler.cpp
    cobc/hal/spacewire.cpp
    cobc/hal/uart.cpp)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "leon3")
    set(src ${src}
        cobc/rtos/rtems/clock.cpp
        cobc/rtos/rtems/mutex.cpp
        cobc/rtos/rtems/semaphore.cpp
        cobc/rtos/rtems/thread.cpp
        cobc/rtos/rtems/timer.cpp
        
        cobc/hal/leon3-rtems/spwl/spacewirelight.c
        cobc/hal/leon3-rtems/spacewire_light.cpp
        cobc/hal/nexys3/gpio.cpp
        cobc/hal/nexys3/sevensegment.cpp)
else()
    set(src ${src}
        cobc/rtos/posix/clock.cpp
        cobc/rtos/posix/semaphore.cpp
        cobc/rtos/posix/thread.cpp
        cobc/rtos/posix/timer.cpp)
endif()

add_library(cobc_rtos STATIC ${src})

target_link_libraries(cobc_rtos cobc_time)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "leon3")
else()
    # For POSIX timers librt is needed, for semaphorse libpthread.
    target_link_libraries(cobc_rtos rt pthread)
endif()
