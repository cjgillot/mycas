add_subdirectory(vectorseq)
add_subdirectory(function)
add_subdirectory(stdfunc)
add_subdirectory(pseries)
add_subdirectory(basic)

file(GLOB src *.cpp)
file(GLOB hdr *.hpp *.ipp)

add_library(analysis ${src} ${hdr})

target_link_libraries(analysis analysis_basic)
target_link_libraries(analysis analysis_stdfunc)
target_link_libraries(analysis analysis_vectorseq)

target_link_libraries(analysis rtti)
# target_link_libraries(analysis pseries)
