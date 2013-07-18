
include_directories(.)

add_library(cobc_time STATIC
    cobc/time/duration.cpp
    cobc/time/time_model.cpp)
