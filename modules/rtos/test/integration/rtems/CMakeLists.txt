
include(${PROJECT_SOURCE_DIR}/../../../conf/cmake/leon3-rtems.cmake)

# Set paths
set(cobc_library_path ${PROJECT_SOURCE_DIR}/../../../../..)
set(cobc_vendor_path ${PROJECT_SOURCE_DIR}/../../../../../../../vendor)

set(cobc_require_libraries rtos time utils)

cmake_minimum_required(VERSION 2.8)

# Build required COBC libraries
foreach(lib ${cobc_require_libraries})
    message(STATUS "Use library '${lib}'")
    add_subdirectory(${cobc_library_path}/${lib}/trunk/src build-${lib})
    include_directories(${cobc_library_path}/${lib}/trunk/src)
endforeach(lib)

set(src
    main.c)

set(executable test-rtems)

add_executable(${executable} ${src})

install(TARGETS ${executable} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/../../../bin)

target_link_libraries(${executable} cobc_rtos)
