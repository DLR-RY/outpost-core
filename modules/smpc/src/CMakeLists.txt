
include_directories(.)
include_directories(${cobc_library_path}/rtos/trunk/src)
include_directories(${cobc_library_path}/utils/trunk/src)

add_library(cobc_smpc STATIC
    cobc/smpc/subscription.cpp
    cobc/smpc/topic.cpp
    cobc/smpc/subscription_raw.cpp
    cobc/smpc/topic_raw.cpp)

target_link_libraries(cobc_smpc cobc_rtos)
