add_library(libexpr_wrap STATIC IMPORTED)
set_target_properties(libexpr_wrap PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libexpr_wrap.a"
)

file( GLOB src *.cpp )

add_library(analysis_caml ${src})
target_link_libraries(analysis_caml libexpr_wrap caml_support bigarray asmrun dl)

file( GLOB_RECURSE mlsrc *.ml *.mli )

add_custom_command(
  TARGET analysis_caml
  PRE_LINK
  COMMAND $(MAKE) -C ${CMAKE_CURRENT_SOURCE_DIR} DEBUG=on
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${mlsrc}
)
