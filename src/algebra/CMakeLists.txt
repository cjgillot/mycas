file(GLOB src *.cpp)
file(GLOB hdr *.hpp *.ipp)

add_library(algebra ${src} ${hdr})
