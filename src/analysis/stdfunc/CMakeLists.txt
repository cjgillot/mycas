file(GLOB src *.cpp)
file(GLOB hdr *.hpp)

add_library(analysis_stdfunc ${src} ${hdr})

target_link_libraries(analysis_stdfunc analysis)
target_link_libraries(analysis_stdfunc analysis_basic)
