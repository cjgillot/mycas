add_subdirectory(function)

file(GLOB_RECURSE src *.cpp)
file(GLOB_RECURSE hdr *.hpp *.ipp)

add_library(analysis ${src} ${hdr})
target_link_libraries(analysis algebra)
