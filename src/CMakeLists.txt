find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB PANO_LIB_SRC *.cpp)

add_executable(pano ${PANO_LIB_SRC}
                    ${pano_SOURCE_DIR}/vendor/glad/src/glad.c)

target_include_directories(pano PUBLIC ${pano_SOURCE_DIR}/include
                                       ${pano_SOURCE_DIR}/vendor/glad/include
                                       ${pano_SOURCE_DIR}/vendor/stb)

target_link_libraries(pano ${OPENGL_gl_LIBRARY} glm glfw dl)
