add_subdirectory(expand)

file(GLOB_RECURSE src *.cpp)
file(GLOB_RECURSE hdr *.hpp)

add_library(analysis_vectorseq ${src} ${hdr})
