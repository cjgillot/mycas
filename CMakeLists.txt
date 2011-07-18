cmake_minimum_required(VERSION 2.8)

project(mycas)

add_definitions(-Wall -Wextra -Winline -Winit-self -pedantic -ansi -Wunreachable-code -Wshadow)
# add_definitions(-O3 -Os -fexpensive-optimizations -pg)

include_directories(src)
add_subdirectory(src)

file(GLOB_RECURSE doc_file *.hpp)

add_custom_command(
  OUTPUT doc
  COMMAND doxygen ARGS -g mycas.doxyfile
  MAIN_DEPENDENCY mycas.doxyfile
  DEPENDS ${doc_file}
)

add_executable(mycas src/main.cpp)
target_link_libraries(mycas analysis)
