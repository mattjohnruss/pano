find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

file(GLOB ENGINE_SRC src/*.cpp)

add_library(engine SHARED ${ENGINE_SRC} ${pano_SOURCE_DIR}/vendor/glad/src/glad.c)

target_include_directories(engine PUBLIC include
                                         ${pano_SOURCE_DIR}/vendor/glad/include
                                         ${pano_SOURCE_DIR}/vendor/stb)

target_link_libraries(engine ${OPENGL_gl_LIBRARY} glm glfw dl assimp)
