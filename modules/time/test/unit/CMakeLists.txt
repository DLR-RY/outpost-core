
# Set paths
set(cobc_library_path ${PROJECT_SOURCE_DIR}/../../../..)
set(cobc_vendor_path ${PROJECT_SOURCE_DIR}/../../../../../../vendor)

set(cobc_require_libraries time utils)

cmake_minimum_required(VERSION 2.8)

include(${PROJECT_SOURCE_DIR}/../../conf/cmake/hosted.cmake)
add_definitions(-DUNITTEST)

# Build google mock & test
add_subdirectory(${cobc_library_path}/utils/trunk/ext/gmock-1.6.0 build-gmock)

include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Build required COBC libraries
foreach(lib ${cobc_require_libraries})
    message(STATUS "Use library '${lib}'")
    add_subdirectory(${cobc_library_path}/${lib}/trunk/src build-${lib})
    include_directories(${cobc_library_path}/${lib}/trunk/src)
endforeach(lib)

set(src
    main.cpp
    duration_test.cpp)

add_executable(unittest ${src})

enable_testing()
add_test(unittest unittest)

# pthread is needed by googletest
target_link_libraries(unittest cobc_time gmock pthread)
