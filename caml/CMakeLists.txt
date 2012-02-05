include( UseOCaml )

file(GLOB src *.hpp *.cpp)

get_property( libs GLOBAL PROPERTY CAML_STARTUP_LIBS )

message( STATUS "ocaml libraries : ${libs}" )

add_ocaml_library( caml_support_libs
  SOURCES dummy.ml
  LIBRARIES ${libs}
)
add_ocaml_output_obj( caml_support_objs
  LIBRARIES caml_support_libs
)
get_target_property( object caml_support_objs.opt.o OUTPUT )

get_target_property( archive caml_support_libs.cmxa CLIB )
add_library( caml_support_code STATIC IMPORTED )
set_target_properties( caml_support_code PROPERTIES
  IMPORTED_LOCATION ${archive}
)
add_dependencies( caml_support_code caml_support_libs )


add_library(caml_support ${src} ${object} )
target_link_libraries(caml_support caml_support_code)
target_link_libraries(caml_support bigarray unix asmrun dl)
add_dependencies(caml_support caml_support_objs)

add_executable(caml_support_x caml_support.cpp)
target_link_libraries(caml_support_x caml_support)
