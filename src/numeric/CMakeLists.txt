file(GLOB src *.cpp)
file(GLOB hdr *.hpp *.ipp)

add_library(numeric number.cpp)

target_link_libraries(numeric gmpfrxx mpfr gmp)
