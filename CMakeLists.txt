cmake_minimum_required(VERSION 2.8)

project(mycas)

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} )

include(FindOCaml)
find_package( OCaml REQUIRED )

link_directories(${CMAKE_OCAML_STDLIB_PATH})

include(FindBoost)
find_package( Boost 1.48.0 )

# add_definitions(-std=c++0x)
add_definitions(-g -Wall -Wextra -Winit-self -pedantic -ansi -Wunreachable-code -Wshadow)
# add_definitions(-O3 -DNDEBUG -pg)
# add_definitions(-frepo)
# add_definitions(-fno-rtti)

set_property(GLOBAL PROPERTY CAML_STARTUP_LIBS "")

include_directories(src/caml)
include_directories(src)
add_subdirectory(src)

file(GLOB_RECURSE doc_file *.hpp *.cpp)

add_custom_target(doc
  COMMAND doxygen mycas.doxyfile
  MAIN_DEPENDENCY mycas.doxyfile
  DEPENDS ${doc_file}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(mycas src/main.cpp)
target_link_libraries(mycas analysis)
target_link_libraries(mycas numeric)
# target_link_libraries(mycas caml_support)
