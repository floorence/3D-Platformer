cmake_minimum_required(VERSION 3.16)

project(Platformer)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(Platformer
    src/Main.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Mesh.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/shaderClass.cpp
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