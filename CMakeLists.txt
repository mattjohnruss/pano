#cmake_minimum_required(VERSION 2.6)
cmake_minimum_required(VERSION 3.9)

project(pano C CXX)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPANO_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPANO_DEBUG")

# TODO enable later - off for now to speed up compilation times
# enable link time optimisation if supported
#include(CheckIPOSupported)
#check_ipo_supported(RESULT ipo_supported OUTPUT error)

if(ipo_supported)
    message(STATUS "IPO / LTO enabled")
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

add_subdirectory(engine)
add_subdirectory(vendor/glm)
add_subdirectory(drivers)
