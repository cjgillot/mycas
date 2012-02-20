file(GLOB src *.cpp)
file(GLOB hdr *.hpp)

add_library(analysis_basic ${src} ${hdr})
target_link_libraries(analysis_basic numeric)
target_link_libraries(analysis_basic analysis)
