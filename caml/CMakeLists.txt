include( UseOCaml )

get_property( caml_objects GLOBAL PROPERTY CAML_SUPPORT_LIBS )
add_ocaml_output_obj(caml_support_obj
  LIBRARIES ${caml_objects}
)
get_target_property(caml_support_object caml_support_obj.opt.o OUTPUT)

file(GLOB caml_support_src *.hpp *.cpp)

add_library(caml_support ${caml_support_src} ${caml_support_object})
target_link_libraries(caml_support asmrun)
add_dependencies(caml_support caml_support_obj)
