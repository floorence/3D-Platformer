cmake_minimum_required(VERSION 3.16)

project(Platformer)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(fmt)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
    "src/*.cpp"
    "src/*.c"
)

add_executable(Platformer ${SOURCE_FILES})

target_include_directories(Platformer PRIVATE
    include
)

target_include_directories(Platformer SYSTEM PRIVATE
    Libraries/include
)

target_link_libraries(Platformer PRIVATE
    OpenGL::GL
    glfw
    fmt::fmt
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
