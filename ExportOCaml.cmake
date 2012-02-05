include( UseOCaml )

find_package( Perl )

set( scripts "${CMAKE_SOURCE_DIR}/caml" )

# {{{1 ocaml_parse_arguments
# parse macro arguments
# copied from http://www.cmake.org/Wiki/CMakeMacroParseArguments
macro( ocaml_export_parse_arguments prefix arg_names option_names )
    set( default_args )
    foreach( arg_name ${arg_names} )
        set( ${prefix}_${arg_name} )
    endforeach( arg_name )
    foreach( option ${option_names} )
        set( ${prefix}_${option} FALSE )
    endforeach( option )

    set( current_arg_name DEFAULT_ARGS )
    set( current_arg_list )
    foreach( arg ${ARGN} )
        set( larg_names ${arg_names} )
        list( FIND larg_names "${arg}" is_arg_name )
        if( is_arg_name GREATER -1 )
            set( ${prefix}_${current_arg_name} ${current_arg_list} )
            set( current_arg_name ${arg} )
            set( current_arg_list )
        else( is_arg_name GREATER -1 )
            set( loption_names ${option_names} )
            list( FIND loption_names "${arg}" is_option )
            if( is_option GREATER -1 )
                set( ${prefix}_${arg} TRUE )
            else( is_option GREATER -1 )
                set( current_arg_list ${current_arg_list} ${arg} )
            endif()
        endif()
    endforeach()
    set( ${prefix}_${current_arg_name} ${current_arg_list} )
endmacro()

macro( export_add_ocaml_native_symbol target definition object )
  get_filename_component( path ${definition} PATH )
  get_filename_component( name ${definition} NAME )
#   get_filename_component( name_we ${definition} NAME_WE )
#   get_filename_component( ext ${definition} EXT )

  set( out_path "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/" )
  set( out_base "${out_path}/${name}" )
  set( out_name "${out_base}.c" )

  add_custom_command(
    OUTPUT ${out_name}
    COMMAND ${PERL_EXECUTABLE}
      ${scripts}/gen_funcs.pl
      ${out_base}
      ${definition} ${object}

    MAIN_DEPENDENCY ${definition}
    DEPENDS ${object} ${definition}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building OCaml glue file ${name}.c"
  )
  add_custom_target( ${target}.glue
    DEPENDS ${out_name}
  )
  set_target_properties( ${target}.glue PROPERTIES
    OUTPUT "${out_name}"
    OBJECT "${object}"
    SOURCE "${definition}"
    KIND   "NATGLUE"
  )
  list( APPEND OCAML_EXPORT_${target}_GLUE "${target}.glue" )
endmacro()

macro( export_add_ocaml_byte_symbol target definition )
  get_filename_component( path ${definition} PATH )
  get_filename_component( name ${definition} NAME )
#   get_filename_component( name_we ${definition} NAME_WE )
#   get_filename_component( ext ${definition} EXT )

  set( out_path "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/" )
  set( out_base "${out_path}/${name}" )
  set( out_name "${out_base}.c" )
  set( out_ml   "${out_base}.ml")

  add_custom_command(
    OUTPUT ${out_name}
    COMMAND ${PERL_EXECUTABLE}
      ${scripts}/gen_funcs.pl
      ${out_base}
      ${definition}

    MAIN_DEPENDENCY ${definition}
    DEPENDS ${definition}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building OCaml glue files ${name}.c and ${name}.ml"
  )
  add_custom_target( ${target}.glue
    DEPENDS ${out_name}
  )
  set_target_properties( ${target}.glue PROPERTIES
    OUTPUT "${out_name}"
    MLOUT  "${out_ml}"
    SOURCE "${definition}"
    KIND   "BYTEGLUE"
  )
  list( APPEND OCAML_EXPORT_${target}_GLUE "${target}.glue" )
endmacro()

# {{{1 add_ocaml_library
macro( add_ocaml_glue target )
  ocaml_export_parse_arguments( OCAML_EXPORT_${target}
    "DEFINITION;OCAMLOPTS;OBJECT"
    ""
    ${ARGN}
  )

  # XXX where is the object file ?
  if( ${OCAML_EXPORT_${target}_OBJECT} )
    export_add_ocaml_native_symbol( ${target} ${OCAML_EXPORT_${target}_DEFINITION} ${OCAML_EXPORT_${target}_OBJECT} )
    get_target_property( out_cdef "${target}.glue" OUTPUT )
    add_library( ${target} ${out_cdef} )
  else( ${OCAML_EXPORT_${target}_OBJECT} )
    export_add_ocaml_byte_symbol( ${target}   ${OCAML_EXPORT_${target}_DEFINITION} )
    add_custom_target( ${target} ALL )
  endif( ${OCAML_EXPORT_${target}_OBJECT} )

  # create the top-level target for this library and add dependencies on each
  # object target
  set_target_properties( ${target} PROPERTIES
      SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OBJECT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir"
      KIND "LIBRARY"
      SOURCES "${OCAML_${target}_SOURCES}"
      LIBRARIES "${OCAML_${target}_LIBRARIES}"
      PACKAGES "${OCAML_${target}_PACKAGES}"
      OCAMLCOPTS "${OCAML_${target}_OCAMLCOPTS}"
      TARGET_DEPLIBS "${OCAML_${target}_TARGET_DEPLIBS}"
      TARGET_TRANSLIBS "${OCAML_${target}_TARGET_TRANSLIBS}"
      TARGET_TRANSPKGS "${OCAML_${target}_TARGET_TRANSPKGS}"
      INCLUDE_DIRS "${OCAML_${target}_INCLUDE_DIRS}"
      LINK_DIRS "${OCAML_${target}_LINK_DIRS}"
      REAL_SRCS "${OCAML_${target}_REAL_SRCS}"
      OBJ_TARGET "${OCAML_${target}_OBJ_TARGETS}"
      ARCHIVES "${OCAML_${target}_ARCHIVES}"
  )
  add_dependencies( ${target} ${OCAML_EXPORT_${target}_GLUE} )
endmacro()
