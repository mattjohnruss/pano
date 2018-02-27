file(GLOB SHADERS_SRC ${pano_SOURCE_DIR}/data/shaders/*.vert ${pano_SOURCE_DIR}/data/shaders/*.frag)

file(GLOB DRIVERS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(driver_src ${DRIVERS_SRC})
    string(REPLACE ".cpp" "" driver ${driver_src})
    add_executable(${driver} ${driver_src} ${SHADERS_SRC})
    target_link_libraries(${driver} PUBLIC engine profiler)

    if(ipo_supported)
        set_property(TARGET ${driver} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endforeach(driver_src)
