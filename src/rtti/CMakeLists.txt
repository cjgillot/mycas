add_subdirectory(visitor)

file(GLOB_RECURSE src *.cpp)
file(GLOB_RECURSE hdr *.hpp *.ipp)

add_library(rtti ${src} ${hdr})
