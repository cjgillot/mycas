file(GLOB src *.cxx)
file(GLOB hdr *.hxx *.ixx)

add_library(algebra ${src} ${hdr})
