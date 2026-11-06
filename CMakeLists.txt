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
  src/Renderer.cpp
  src/VertexBuffer.cpp
  src/VertexArray.cpp
  src/IndexBuffer.cpp
)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

find_program(XXD_CMD xxd)
if(NOT XXD_CMD)
  message(FATAL_ERROR "Could not find program 'xxd'. Install it.")
endif()
function(embed_file INPUT OUTPUT COMMENT_TEXT)
    get_filename_component(INPUT_DIR "${INPUT}" DIRECTORY)
    get_filename_component(INPUT_NAME "${INPUT}" NAME)
    add_custom_command(
        OUTPUT "${OUTPUT}"
        COMMAND ${XXD_CMD} -i "${INPUT_NAME}" "${OUTPUT}"
        WORKING_DIRECTORY "${INPUT_DIR}"
        DEPENDS "${INPUT}"
        COMMENT "${COMMENT_TEXT}"
        VERBATIM
    )
endfunction()
set(SHADERS_OUT "${CMAKE_CURRENT_BINARY_DIR}/shaderz.h")
embed_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders.glsl" "${SHADERS_OUT}" "Generating shader header...")

add_executable(gl-proj
  ${SRC_FILES}
  ${SHADERS_OUT}
)

target_include_directories(gl-proj PRIVATE
  include
  ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(gl-proj PRIVATE OpenGL::GL glfw)

if(MSVC)
  target_compile_options(gl-proj PRIVATE /W3)
else()
  target_compile_options(gl-proj PRIVATE -Wall -Wextra)
endif()
