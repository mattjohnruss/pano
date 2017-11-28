find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

file(GLOB ENGINE_SRC src/*.cpp)
file(GLOB ENGINE_INC include/*.h)

add_library(engine SHARED ${ENGINE_SRC} ${ENGINE_INC} ${pano_SOURCE_DIR}/vendor/glad/src/glad.c)

target_include_directories(engine PUBLIC include
                                         ${pano_SOURCE_DIR}/vendor/glad/include
                                         ${pano_SOURCE_DIR}/vendor/stb)

target_link_libraries(engine ${OPENGL_gl_LIBRARY} glm glfw dl assimp)
