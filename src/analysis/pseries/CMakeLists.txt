include( UseOCaml )
include( ExportOCaml )

file( GLOB ml_src *.ml *.mli )

add_ocaml_file_options( mul
  SOURCES mul.ml
  OCAMLOPTS -w p
)
add_ocaml_library( ml_pseries
  SOURCES ${ml_src}
  PACKAGES bigarray
)
set_property(GLOBAL APPEND PROPERTY CAML_STARTUP_LIBS ml_pseries)

add_ocaml_glue( ml_pseries_glue
  DEFINITION pseries_export.def
#   OBJECT ml_pseries.cmxa
)

file( GLOB cpp_src *.cpp *.hpp )

add_library( pseries ${cpp_src} ${ml_arch} )
add_dependencies( pseries ml_pseries_glue )
