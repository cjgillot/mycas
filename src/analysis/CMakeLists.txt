add_subdirectory(vectorseq)
add_subdirectory(function)
add_subdirectory(stdfunc)
add_subdirectory(pseries)
add_subdirectory(basic)
add_subdirectory(caml)

file(GLOB src *.cpp)
file(GLOB hdr *.hpp *.ipp)

add_library(analysis rtti.hpp ${src} ${hdr})

target_link_libraries(analysis analysis_basic)
target_link_libraries(analysis analysis_function)
target_link_libraries(analysis analysis_pseries)
target_link_libraries(analysis analysis_stdfunc)
target_link_libraries(analysis analysis_vectorseq)

target_link_libraries(analysis rtti)
