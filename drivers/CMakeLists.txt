file(GLOB SHADERS_SRC ${pano_SOURCE_DIR}/data/shaders/*.vert ${pano_SOURCE_DIR}/data/shaders/*.frag)

add_executable(pano pano.cpp ${SHADERS_SRC})
target_link_libraries(pano LINK_PUBLIC engine)

add_executable(gooch gooch.cpp ${SHADERS_SRC})
target_link_libraries(gooch LINK_PUBLIC engine)

add_executable(hatching hatching.cpp ${SHADERS_SRC})
target_link_libraries(hatching LINK_PUBLIC engine)
