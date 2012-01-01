file(GLOB src *.cpp *.c)
file(GLOB hdr *.hpp *.ipp)

add_library(numeric ${src} ${hdr})

target_link_libraries(numeric flint mpirxx)

add_executable(numeric_test number.cpp)
target_link_libraries(numeric_test numeric)
