add_subdirectory(function)

file(GLOB_RECURSE src *.cxx)
file(GLOB_RECURSE hdr *.hxx *.ixx)

add_library(analysis ${src} ${hdr})
target_link_libraries(analysis algebra)
