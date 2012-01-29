include( FindPython )

# find base ocaml binary
find_program( CMAKE_PYTHON_PYTHON
    NAMES python
    )
# get the directory, used for hints for the rest of the binaries
if( CMAKE_PYTHON_PYTHON )
    get_filename_component( PYTHON_ROOT_DIR ${CMAKE_PYTHON_PYTHON} PATH )
endif()

execute_process(
    COMMAND ${CMAKE_PYTHON_PYTHON} -version
    OUTPUT_VARIABLE CMAKE_PYTHON_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_package_handle_standard_args( PYTHON "Could NOT find Python."
    CMAKE_PYTHON_VERSION
    CMAKE_PYTHON_STDLIB_PATH
    CMAKE_PYTHON_PYTHON
)

mark_as_advanced(
    CMAKE_PYTHON_PYTHON
)

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

macro( export_add_ocaml_symbol target definition object )
  get_filename_component( path ${definition} PATH )
  get_filename_component( name ${definition} NAME )
#   get_filename_component( name_we ${definition} NAME_WE )
#   get_filename_component( ext ${definition} EXT )

  set( out_name "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/${name}.c" )
  add_custom_command(
    OUTPUT ${out_name}
    COMMAND ${CMAKE_PYTHON_PYTHON}
      ${scripts}/gen_funcs.py
      ${object} ${definition} ${out_name}

    MAIN_DEPENDENCY ${definition}
    DEPENDS ${object} ${definition}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building OCaml glue file ${name}.c"
  )
  add_custom_target( ${target}.${name}.glue
    DEPENDS ${out_name}
  )
  set_target_properties( ${target}.${name}.glue PROPERTIES
    OUTPUT "${out_name}"
    OBJECT "${object}"
    SOURCE "${definition}"
  )
  list( APPEND OCAML_EXPORT_${target}_GLUE "${target}.${name}.glue" )
endmacro()

# {{{1 add_ocaml_library
macro( add_ocaml_glue target )
  #message( STATUS "add_ocaml_library( ${target} )" )

  ocaml_export_parse_arguments( OCAML_EXPORT_${target}
    "SOURCES;LIBRARIES;PACKAGES;OCAMLOPTS;OCAMLCOPTS"
    ""
    ${ARGN}
  )
  #message( STATUS "   OCAML_${target}_SOURCES:          ${OCAML_${target}_SOURCES}" )
  #message( STATUS "   OCAML_${target}_LIBRARIES:        ${OCAML_${target}_LIBRARIES}" )
  #message( STATUS "   OCAML_${target}_PACKAGES:         ${OCAML_${target}_PACKAGES}" )
  #message( STATUS "   OCAML_${target}_OCAMLOPTS:        ${OCAML_${target}_OCAMLOPTS}" )
  #message( STATUS "   OCAML_${target}_OCAMLCOPTS:       ${OCAML_${target}_OCAMLCOPTS}" )

  ocaml_set_target_deplibs( ${target} )
  #message( STATUS "   OCAML_${target}_TARGET_DEPLIBS:   ${OCAML_${target}_TARGET_DEPLIBS}" )

  ocaml_set_target_translibs( ${target} )
  #message( STATUS "   OCAML_${target}_TARGET_TRANSLIBS: ${OCAML_${target}_TARGET_TRANSLIBS}" )

  ocaml_set_target_transpkgs( ${target} )
  #message( STATUS "   OCAML_${target}_TARGET_TRANSPKGS: ${OCAML_${target}_TARGET_TRANSPKGS}" )

  ocaml_set_include_dirs( ${target} )
  #message( STATUS "   OCAML_${target}_INCLUDE_DIRS:     ${OCAML_${target}_INCLUDE_DIRS}" )

  ocaml_set_link_dirs( ${target} )
  #message( STATUS "   OCAML_${target}_LINK_DIRS:        ${OCAML_${target}_LINK_DIRS}" )

  # find the real sources for the target
  ocaml_set_real_srcs( ${target} )
  #message( STATUS "   OCAML_${target}_REAL_SRCS:        ${OCAML_${target}_REAL_SRCS}" )

  # create all the object targets, and collect information needed for the
  # archives
  ocaml_add_objs( ${target} )
  #message( STATUS "   OCAML_${target}_OBJ_TARGETS:      ${OCAML_${target}_OBJ_TARGETS}" )

  # create the target for the archive
  ocaml_add_archives( ${target} )
  #message( STATUS "   OCAML_${target}_ARCHIVES:         ${OCAML_${target}_ARCHIVES}" )

  # create the top-level target for this library and add dependencies on each
  # object target
  add_custom_target( ${target} ALL )
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
  add_dependencies( ${target} ${OCAML_${target}_ARCHIVES} )
endmacro()
