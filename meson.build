project('opengl-proj', 'c', 'cpp',
  version: '0.1.0',
  default_options: ['warning_level=2', 'cpp_std=c++20']
)

src_files = files(
  'src/main.cpp',
  'src/glad.c',
)

gl_dep = dependency('gl')
glfw_dep = dependency('glfw3')

executable('gl-proj', src_files, dependencies: [gl_dep, glfw_dep],
  include_directories: ['include']
)
