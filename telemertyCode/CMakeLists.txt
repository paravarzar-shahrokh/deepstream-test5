cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ~/Documents/MAVSDK/install/lib/cmake/MAVSDK/)
set (CMAKE_PREFIX_PATH ~/Documents/MAVSDK/install/lib/cmake/MAVSDK/)
project(telemetry)

if(NOT MSVC)
    add_definitions("-Wall -Wextra -Werror")
        # This warning is triggered by the MAVLink includes.
    add_definitions("-Wno-address-of-packed-member")
else()
    add_definitions("-WX -W2")
endif()

find_package(MAVSDK REQUIRED)

add_executable(telemetry
    telemetry.cpp
)

target_link_libraries(telemetry
    MAVSDK::mavsdk
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk_mavlink_passthrough
)