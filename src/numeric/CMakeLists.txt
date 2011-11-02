file(GLOB_RECURSE src *.cpp *.c)
file(GLOB_RECURSE hdr *.hpp *.ipp)

add_library(numeric ${src} ${hdr})

target_link_libraries(numeric flint)
