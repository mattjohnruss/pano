cmake_minimum_required(VERSION 2.6)

project(pano C CXX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPANO_DEBUG")

add_subdirectory(engine)
add_subdirectory(vendor/glm)
add_subdirectory(drivers)
