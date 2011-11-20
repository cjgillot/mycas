add_subdirectory(integer)
add_subdirectory(rational)
add_subdirectory(modular)
add_subdirectory(real)

file(GLOB_RECURSE src *.cpp *.c)
file(GLOB_RECURSE hdr *.hpp *.ipp)

add_library(numeric ${src} ${hdr})

target_link_libraries(numeric flint)
