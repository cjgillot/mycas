add_subdirectory(vectorseq)
add_subdirectory(function)
add_subdirectory(stdfunc)
add_subdirectory(pseries)
add_subdirectory(expand)

file(GLOB_RECURSE src *.cpp)
file(GLOB_RECURSE hdr *.hpp *.ipp)

add_library(analysis ${src} ${hdr})
target_link_libraries(analysis algebra)
target_link_libraries(analysis rtti)
target_link_libraries(analysis pseries)
