include( UseOCaml )

file( GLOB ml_src *.ml *.mli )

add_ocaml_file_options( mul
  SOURCES mul.ml
  OCAMLOPTS -w p
)

add_ocaml_library( ml_pseries
  SOURCES ${ml_src}
  PACKAGES bigarray
)

file( GLOB cpp_src *.cpp *.hpp )

get_target_property( ml_arch ml_pseries.cmxa CLIB )

set_property(GLOBAL APPEND PROPERTY CAML_SUPPORT_LIBS ml_pseries)

# set_source_files_properties(
#   ${ml_obj}
#   PROPERTIES
#   EXTERNAL_OBJECT true
# )

add_library( pseries ${cpp_src} ${ml_obj} ${ml_arch} )
add_dependencies( pseries ml_pseries )
