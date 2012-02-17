# include( UseOCaml )
# include( ExportOCaml )
#
# add_ocaml_file_options( mul
#   SOURCES mul.ml
#   OCAMLOPTS -w p
# )
# add_ocaml_library( ml_pseries
#   SOURCES ${ml_src}
#   PACKAGES bigarray
# )
# set_property(GLOBAL APPEND PROPERTY CAML_STARTUP_LIBS ml_pseries)
#
# add_ocaml_glue( ml_pseries_glue
#   DEFINITION pseries_export.def
# #   OBJECT ml_pseries.cmxa
# )
#
# file( GLOB cpp_src *.cpp *.hpp )
#
# add_library( pseries ${cpp_src} ${ml_arch} )
# add_dependencies( pseries ml_pseries_glue )

add_library(libpseries STATIC IMPORTED)
set_target_properties(libpseries PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libpseries.a"
)

file( GLOB src *.cpp )
add_library(analysis_pseries export.h ${src} "../caml/expr_wrap.cpp" )
target_link_libraries(analysis_pseries libpseries caml_support bigarray asmrun dl)

add_custom_command(
  OUTPUT export.h
  COMMAND $(MAKE) -C ${CMAKE_CURRENT_SOURCE_DIR} "export.h"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  MAIN_DEPENDENCY "export.def"
)

file( GLOB_RECURSE mlsrc *.ml *.mli )

add_custom_command(
  TARGET analysis_pseries
  PRE_LINK
  COMMAND $(MAKE) -C ${CMAKE_CURRENT_SOURCE_DIR} DEBUG=on
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${mlsrc}
)
