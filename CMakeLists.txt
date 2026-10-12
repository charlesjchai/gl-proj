cmake_minimum_required(VERSION 3.15)
project(opengl-proj
  VERSION 0.1.0
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_FILES
  src/glad.c
  src/main.cpp
  src/shader-parser.cpp
)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
add_executable(gl-proj ${SRC_FILES})

target_include_directories(gl-proj PRIVATE include)
target_link_libraries(gl-proj PRIVATE OpenGL::GL glfw)

if(MSVC)
  target_compile_options(gl-proj PRIVATE /W3)
else()
  target_compile_options(gl-proj PRIVATE -Wall -Wextra)
endif()
