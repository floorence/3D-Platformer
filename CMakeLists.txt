cmake_minimum_required(VERSION 3.16)

project(Platformer)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Platformer
    src/Main.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Mesh.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/Shader.cpp
    src/stb.cpp
    src/glad.c
)

target_include_directories(Platformer PRIVATE
    include
    Libraries/include
)

target_link_libraries(Platformer PRIVATE
    OpenGL::GL
    glfw
)

target_compile_options(Platformer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -fsanitize=address
    -fsanitize=undefined
    -g
)

target_link_options(Platformer PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)

set(PEDANTIC_EXCEPTIONS
    src/glad.c
)

set_source_files_properties(${PEDANTIC_EXCEPTIONS} 
    PROPERTIES COMPILE_OPTIONS "-Wno-pedantic"
)
