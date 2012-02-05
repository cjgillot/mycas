# - Use OCaml
# Provide useful macros for OCaml.
#
# The following cache variables are defined:
#   CMAKE_OCaml_FLAGS
#   CMAKE_OCaml_FLAGS_DEBUG
#   CMAKE_OCaml_FLAGS_MINSIZEREL
#   CMAKE_OCaml_FLAGS_RELEASE
#   CMAKE_OCaml_FLAGS_RELWITHDEBINFO
#   CMAKE_OCaml_LINKER_FLAGS
#   CMAKE_OCaml_LINKER_FLAGS_DEBUG
#   CMAKE_OCaml_LINKER_FLAGS_MINSIZEREL
#   CMAKE_OCaml_LINKER_FLAGS_RELEASE
#   CMAKE_OCaml_LINKER_FLAGS_RELWITHDEBINFO
#   CMAKE_OCaml_NATIVE, specify default compiler-mode: native or bytecode.
#
# The following macros are defined:
#
# find_ocaml_package (<name>)
#   Use ocamlfind to find an OCaml package.
#   Variables ${name}_INCLUDE_DIRS and ${name}_LIBRARIES are set.
#   Cache variables ${name}_INCLUDE_DIR and ${name}_LIBRARY are also set.
#
# add_ocaml_executable (<name> [NATIVE | BYTECODE] source1 source2 ... sourceN)
#   sourcefiles should be mli or ml files.
#   To generate the executable, you have to call target_link_ocaml_libraries.
#   Sets the OCAML_${name}_NATIVE variable.
#   To specify the include directories, use the standard macro include_directories.
#
# add_ocaml_library (<name> [NATIVE | BYTECODE] source1 source2 ... sourceN)
#   sourcefiles should be mli or ml files.
#   To generate the library, you have to call target_link_ocaml_libraries.
#   Sets the OCAML_${name}_NATIVE variable.
#   To specify the include directories, use the standard macro include_directories.
#
# target_link_ocaml_libraries (<name> lib1 lib2 ... libN)
#   There are four ways to add a library :
#   - If it is another library of the current project, just specify its name.
#   - If it is an exported library, include the export file and specify its name.
#   - If it is a standard library, just specify its name.
#   - For other libraries, give an absolute path to the library.
#   Library dependencies are transitive.
#   Also set properties on target ocaml.${name}. Properties are
#   - KIND: a constant string which is equal to "EXECUTABLE" or "LIBRARY".
#   - LOCATION: indicates where the target is located.
#   - LINK_INTERFACE_LIBRARIES: indicates with which libraries the current library must be linked. Empty for an executable.
#   - OUTPUT_NAME: real name of the target.
#
# add_ocaml_target (<name>
#                   MAIN      <main>
#                   SOURCES   source1 source2 ... sourceN
#                   HEADERS   header1 header2 ... headerN
#                   LIBRARIES lib1 lib2 ... libN
#                   INCLUDES  include1 include2 ... includeN
#                  )
#   A shortcut macro for add_ocaml_executable, add_ocaml_library and target_link_ocaml_libraries.
#   If MAIN is specified, the target is considered as an executable.
#
# install_ocaml_targets (target1 target2 ... targetN DESTINATION <dir>)
#   Generates installation rules for OCaml targets.
#   Set the INSTALL_LOCATION property.
#
# install_ocaml_interfaces (<target> interfacename1 interfacename2 ... interfacenameN DESTINATION <dir>)
#   Installs CMI or CMX files according to the variable OCAML_${target}_NATIVE.
#
# install_ocaml_exports (target1 target2 ... targetN DESTINATION <dir> FILE <export-file>)
#   Generates and installs a CMake file containing code to import OCaml targets from the installation tree.
#
# gen_ocaml_lexers (outfilesname lexsources)
#   For each lex source, generates OCaml code by calling ocamllex.
#   The name of the result sources are put into the variable ${outfilesname}.
#   Because of dependency reasons, the OCaml code is also generated at the
# first configuration time.
#
# gen_ocaml_parsers (outfilesname yaccsources)
#   For each yacc source, generates OCaml code by calling ocamlyacc.
#   The name of the result sources are put into the variable ${outfilesname}.
#   Because of dependency reasons, the OCaml code is also generated at the
# first configuration time.
#
# TODO : see if it is possible to call the dependency generator at compile time
# before compiling source files but after generating some source files.
#
# Copyright (c) 2010, Judica�l Bedouet, j dot bedouet at infonie dot fr.
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

get_filename_component (CMAKE_USE_OCAML_DIR  "${CMAKE_CURRENT_LIST_FILE}" PATH)
set (CMAKE_USE_OCAML_FILE "${CMAKE_USE_OCAML_DIR}/OCamlDep.cmake")

if (NOT DEFINED CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "" CACHE STRING "CMake build type: none, debug, minsizerel, release or relwithdebinfo")
endif (NOT DEFINED CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE)
  string (TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)
endif (CMAKE_BUILD_TYPE)

option (CMAKE_OCaml_NATIVE "Compile OCaml targets with native compiler")

set (CMAKE_OCaml_FLAGS ""
  CACHE STRING "Flags used by the compiler during all build types"
  )
set (CMAKE_OCaml_FLAGS_DEBUG -g
  CACHE STRING "Flags used by the compiler during debug builds"
  )
set (CMAKE_OCaml_FLAGS_MINSIZEREL -ccopt -Os
  CACHE STRING "Flags used by the compiler during minsizerel builds"
  )
set (CMAKE_OCaml_FLAGS_RELEASE -noassert -unsafe -ccopt -O3
  CACHE STRING "Flags used by the compiler during release builds"
  )
set (CMAKE_OCaml_FLAGS_RELWITHDEBINFO -g -ccopt -O2
  CACHE STRING "Flags used by the compiler during relwithdebinfo builds"
  )

set (CMAKE_OCaml_LINKER_FLAGS ""
  CACHE STRING "Flags used for linking binaries during all build types"
  )
set (CMAKE_OCaml_LINKER_FLAGS_DEBUG -g
  CACHE STRING "Flags used for linking binaries during debug builds"
  )
set (CMAKE_OCaml_LINKER_FLAGS_MINSIZEREL ""
  CACHE STRING "Flags used for linking binaries during minsizerel builds"
  )
set (CMAKE_OCaml_LINKER_FLAGS_RELEASE ""
  CACHE STRING "Flags used for linking binaries during release builds"
  )
set (CMAKE_OCaml_LINKER_FLAGS_RELWITHDEBINFO -g
  CACHE STRING "Flags used for linking binaries during relwithdebinfo builds"
  )

mark_as_advanced (
  CMAKE_OCaml_FLAGS
  CMAKE_OCaml_FLAGS_DEBUG
  CMAKE_OCaml_FLAGS_MINSIZEREL
  CMAKE_OCaml_FLAGS_RELEASE
  CMAKE_OCaml_FLAGS_RELWITHDEBINFO
  CMAKE_OCaml_LINKER_FLAGS
  CMAKE_OCaml_LINKER_FLAGS_DEBUG
  CMAKE_OCaml_LINKER_FLAGS_MINSIZEREL
  CMAKE_OCaml_LINKER_FLAGS_RELEASE
  CMAKE_OCaml_LINKER_FLAGS_RELWITHDEBINFO
  )

macro (find_ocaml_package name)
  string (TOUPPER ${name} _name_upper)
  execute_process (
    COMMAND         ${CMAKE_OCaml_FIND} query ${name}
    OUTPUT_VARIABLE ${_name_upper}_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  set (${_name_upper}_LIBRARY ${${_name_upper}_INCLUDE_DIR}/${name})
  execute_process (
    COMMAND    ${CMAKE_OCaml_FIND} query -format "%v" ${name}
    OUTPUT_VARIABLE ${_name_upper}_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  include (FindPackageHandleStandardArgs)
  find_package_handle_standard_args (${name} DEFAULT_MSG
    ${_name_upper}_VERSION
    ${_name_upper}_INCLUDE_DIR
    ${_name_upper}_LIBRARY
    )
  mark_as_advanced (
    ${_name_upper}_INCLUDE_DIR
    ${_name_upper}_LIBRARY
    )
  if (${_name_upper}_FOUND)
    set (${_name_upper}_INCLUDE_DIRS ${${_name_upper}_INCLUDE_DIR})
    set (${_name_upper}_LIBRARIES ${${_name_upper}_LIBRARY})
  endif (${_name_upper}_FOUND)
endmacro (find_ocaml_package name)

# get_ocaml_dependencies (native source)
#   Generates several files which contains the dependencies for the file ${source}.
#   The CMake dependency file, ${source}.dep.cmake is generated in the directory
# ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/
#   The native argument is used for interface files. Indeed, the CMI file produced
# for an interface file is the same file but it could depend on CMO files or CMX files.
macro (get_ocaml_dependencies target source)
  set (_native ${OCAML_${target}_NATIVE})
  execute_process (
    COMMAND ${CMAKE_COMMAND} -D ocamldep=${CMAKE_OCaml_DEP}
                             -D filename=${source}
           -D workingdir=${CMAKE_CURRENT_SOURCE_DIR}
           -D include=${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ocaml.${target}.dir
           -D output=${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ocaml.${target}.dir
           -D native=${_native}
           -P ${CMAKE_USE_OCAML_FILE}
    )
endmacro (get_ocaml_dependencies)

# ocaml_add_object_target (target source objectname)
#   Compiles the Caml source ${source} to native or bytecode object.
#   The name of the object is written in the variable ${objectname}.
macro (ocaml_add_object_target target source objectname)
  set (_native ${OCAML_${target}_NATIVE})

  get_filename_component (_source_we   ${source} NAME_WE)
  get_filename_component (_source_name ${source} NAME)
  get_filename_component (_source_dir  ${source} PATH)

  set (_output_dir "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ocaml.${target}.dir")
  if (_source_dir)
    set (_output_dir "${_output_dir}/${_source_dir}")
  endif (_source_dir)

  if (_native)
    set (_object_ext cmx)
    set (_compiler   ${CMAKE_OCaml_OPT_COMPILER})
    set (_output
      ${_output_dir}/${_source_we}.cmx
      ${_output_dir}/${_source_we}.o
      )
  else (_native)
    set (_object_ext cmo)
    set (_compiler   ${CMAKE_OCaml_COMPILER})
    set (_output     ${_output_dir}/${_source_we}.cmo)
  endif (_native)

  set_property (GLOBAL PROPERTY ${_source_we}_IMPL_LOCATION ${_output_dir}/${_source_we}.${_object_ext})

  get_ocaml_dependencies (${target} ${source})

  include (${_output_dir}/Dependencies/${_source_name}.dep.cmake)

  set (_depends)
  set (_includes)
  foreach (dep ${${_source_we}_${_object_ext}_DEPENDS})
    get_filename_component (_dep_name_we ${dep} NAME_WE)
    get_filename_component (_dep_ext ${dep} EXT)
    if (_dep_ext STREQUAL ".cmi")
      get_property (_absolute_location GLOBAL PROPERTY ${_dep_name_we}_INTF_LOCATION)
    else (_dep_ext STREQUAL ".cmo" OR _dep_ext STREQUAL ".cmx")
      get_property (_absolute_location GLOBAL PROPERTY ${_dep_name_we}_IMPL_LOCATION)
    endif (_dep_ext STREQUAL ".cmi")
    list (APPEND _depends ${_absolute_location})
    get_filename_component (_absolute_path ${_absolute_location} PATH)
    list (APPEND _includes ${_absolute_path})
  endforeach (dep)

  get_directory_property (_include_dirs INCLUDE_DIRECTORIES)
  list (APPEND _includes ${_include_dirs})
  list (REMOVE_DUPLICATES _includes)

  set (_include_flags)
  foreach (_include ${_includes})
    list (APPEND _include_flags -I ${_include})
  endforeach (_include)

#  message (STATUS "${_output} depends on")
#  message (STATUS "${_depends}")

  add_custom_command (OUTPUT ${_output}
    COMMAND ${_compiler} ${CMAKE_OCaml_FLAGS}
                         ${CMAKE_OCaml_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}
       ${_include_flags}
       -o ${_output_dir}/${_source_we}.${_object_ext}
       -c ${source}
    COMMAND ${CMAKE_COMMAND} -D ocamldep=${CMAKE_OCaml_DEP}
                             -D filename=${source}
           -D workingdir=${CMAKE_CURRENT_SOURCE_DIR}
           -D include=${_output_dir}
           -D output=${_output_dir}
           -D native=${_native}
           -P ${CMAKE_USE_OCAML_FILE}
    MAIN_DEPENDENCY   ${source}
    DEPENDS           ${_depends}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT           "Building OCaml object ${_source_we}.${_object_ext}"
    )

  add_custom_target (${target}.${_source_we}.${_object_ext} DEPENDS ${_output})

  set (${objectname} "${_output_dir}/${_source_we}.${_object_ext}")

  get_directory_property (_clean ADDITIONAL_MAKE_CLEAN_FILES)
  list (APPEND _clean ${_output_dir}/${_source_we}.cmi)
  set_directory_properties (PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${_clean}")

endmacro (ocaml_add_object_target)

# ocaml_add_interface_object_target (target source)
#   Compiles the Caml interface ${source}.
macro (ocaml_add_interface_object_target target source)
  set (_native ${OCAML_${target}_NATIVE})

  get_filename_component (_source_we   ${source} NAME_WE)
  get_filename_component (_source_name ${source} NAME)
  get_filename_component (_source_dir  ${source} PATH)

  set (_output_dir "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ocaml.${target}.dir")
  if (_source_dir)
    set (_output_dir "${_output_dir}/${_source_dir}")
  endif (_source_dir)

  set (_output "${_output_dir}/${_source_we}.cmi")
  set_property (GLOBAL PROPERTY ${_source_we}_INTF_LOCATION ${_output})

  get_ocaml_dependencies (${target} ${source})

  if (_native)
    include (${_output_dir}/Dependencies/${_source_name}.native.dep.cmake)
  else (_native)
    include (${_output_dir}/Dependencies/${_source_name}.dep.cmake)
  endif (_native)

  set (_depends)
  foreach (dep ${${_source_we}_cmi_DEPENDS})
    get_filename_component (_dep_name_we ${dep} NAME_WE)
    get_filename_component (_dep_ext ${dep} EXT)
    if (_dep_ext STREQUAL ".cmi")
      get_property (_absolute_location GLOBAL PROPERTY ${_dep_name_we}_INTF_LOCATION)
    else (_dep_ext STREQUAL ".cmo" OR _dep_ext STREQUAL ".cmx")
      get_property (_absolute_location GLOBAL PROPERTY ${_dep_name_we}_IMPL_LOCATION)
    endif (_dep_ext STREQUAL ".cmi")
    list (APPEND _depends ${_absolute_location})
    get_filename_component (_absolute_path ${_absolute_location} PATH)
    list (APPEND _includes ${_absolute_path})
  endforeach (dep)

  get_directory_property (_include_dirs INCLUDE_DIRECTORIES)
  list (APPEND _includes ${_include_dirs})
  list (REMOVE_DUPLICATES _includes)

  set (_include_flags)
  foreach (_include ${_includes})
    list (APPEND _include_flags -I ${_include})
  endforeach (_include)

  add_custom_command (OUTPUT ${_output}
    COMMAND ${CMAKE_OCaml_COMPILER} ${CMAKE_OCaml_FLAGS}
                                    ${CMAKE_OCaml_FLAGS_${CMAKE_BUILD_TYPE_UPPER}}
            ${_include_flags}
            -o ${_output}
            -c ${source}
    COMMAND ${CMAKE_COMMAND} -D ocamldep=${CMAKE_OCaml_DEP}
                             -D filename=${source}
           -D workingdir=${CMAKE_CURRENT_SOURCE_DIR}
           -D include=${_output_dir}
           -D output=${_output_dir}
           -D native=${_native}
           -P ${CMAKE_USE_OCAML_FILE}
    MAIN_DEPENDENCY   ${source}
    DEPENDS           ${_depends}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT           "Building OCaml object ${_source_we}.cmi"
    )

  add_custom_target (${target}.${_source_we}.cmi DEPENDS ${_output})

endmacro (ocaml_add_interface_object_target)

# add_ocaml_objects (name sourcefiles)
#   Add rules to compile source files in native or bytecode.
#   Set the OCAML_${name}_OBJECTS and OCAML_${name}_NATIVE variables.
#   The real target is created by target_link_ocaml_libraries.
macro (add_ocaml_objects name)

  set (_native  ${CMAKE_OCaml_NATIVE})
  set (_sources ${ARGN})

  if (${ARGV1} STREQUAL "NATIVE")
    set  (_native TRUE)
    list (REMOVE_AT _sources 0)
  elseif (${ARGV1} STREQUAL "BYTECODE")
    set  (_native FALSE)
    list (REMOVE_AT _sources 0)
  endif (${ARGV1} STREQUAL "NATIVE")

  set (OCAML_${name}_NATIVE  ${_native})
  set (OCAML_${name}_OBJECTS)

  foreach (_source ${_sources})
    if (_source MATCHES "\\.ml$")
      ocaml_add_object_target (${name} ${_source} _object)
      list (APPEND OCAML_${name}_OBJECTS ${_object})
    elseif (_source MATCHES "\\.mli$")
      ocaml_add_interface_object_target (${name} ${_source})
    endif (_source MATCHES "\\.ml$")
  endforeach (_source)

endmacro (add_ocaml_objects)

# add_ocaml_executable (name sourcefiles)
#   See description above.
macro (add_ocaml_executable name)
  add_ocaml_objects (${name} ${ARGN})
  set (OCAML_${name}_KIND "EXECUTABLE")
endmacro (add_ocaml_executable)

# add_ocaml_library (name sourcefiles)
#   See description above.
macro (add_ocaml_library name)
  add_ocaml_objects (${name} ${ARGN})
  set (OCAML_${name}_KIND "LIBRARY")
endmacro (add_ocaml_library)

# target_link_ocaml_libraries (target libraries)
#   See description above.
macro (target_link_ocaml_libraries target)

  set (_deps)
  set (_tdeps)
  set (_libraries)

  foreach (library ${ARGN})
    if (IS_ABSOLUTE ${library})
      if (OCAML_${target}_NATIVE)
  list (APPEND _libraries ${library}.cmxa)
      else (OCAML_${target}_NATIVE)
  list (APPEND _libraries ${library}.cma)
      endif (OCAML_${target}_NATIVE)
    else (IS_ABSOLUTE ${library})
      get_target_property (_location  ocaml.${library} LOCATION)
      get_target_property (_ilocation ${library}       IMPORTED_LOCATION)
      if (_location) # It is a library of this project
  list (APPEND _deps ${_location})
  list (APPEND _tdeps ocaml.${library})
  get_target_property (_libs ocaml.${library} LINK_INTERFACE_LIBRARIES)
  list (APPEND _libraries ${_libs} ${_location})
      elseif (_ilocation) # It is a library imported from another project
  list (APPEND _deps ${_ilocation})
  get_target_property (_libs ${library} LINK_INTERFACE_LIBRARIES)
  list (APPEND _libraries ${_libs} ${_ilocation})
      else (_location) # It is a standard library
  if (OCAML_${target}_NATIVE)
    set (_location ${library}.cmxa)
  else (OCAML_${target}_NATIVE)
    set (_location ${library}.cma)
  endif (OCAML_${target}_NATIVE)
  list (APPEND _libraries ${_location})
      endif (_location)
    endif (IS_ABSOLUTE ${library})
  endforeach (library)

  if (OCAML_${target}_NATIVE)
    set (_compiler ${CMAKE_OCaml_OPT_COMPILER})
    set (_libext ".cmxa")
  else (OCAML_${target}_NATIVE)
    set (_compiler ${CMAKE_OCaml_COMPILER})
    set (_libext ".cma")
  endif (OCAML_${target}_NATIVE)

  set (_opt ${CMAKE_OCaml_FLAGS} ${CMAKE_OCaml_FLAGS_${CMAKE_BUILD_TYPE_UPPER}})

  if (${OCAML_${target}_KIND} STREQUAL "EXECUTABLE")
    set (_comment  "Linking OCaml executable ${target}")
    set (_ext)
    set (_location "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}")
    set (_libs     ${_libraries})
  elseif (${OCAML_${target}_KIND} STREQUAL "LIBRARY")
    set (_comment  "Linking OCaml library ${target}")
    set (_ext      "${_libext}")
    set (_location "${CMAKE_CURRENT_BINARY_DIR}/${target}${_libext}")
    set (_opt      ${_opt} -a)
    set (_libs)
  endif (${OCAML_${target}_KIND} STREQUAL "EXECUTABLE")

  add_custom_command (OUTPUT ${target}${_ext}
    COMMAND ${_compiler} ${_opt} -o ${target}${_ext} ${_libs} ${OCAML_${target}_OBJECTS}
    DEPENDS ${OCAML_${target}_OBJECTS} ${_deps}
    COMMENT "${_comment}"
    )

  add_custom_target (ocaml.${target} ALL DEPENDS ${target}${_ext})

  if (_tdeps)
    add_dependencies (ocaml.${target} ${_tdeps})
  endif (_tdeps)

  set_target_properties (ocaml.${target} PROPERTIES
    KIND                     ${OCAML_${target}_KIND}
    LOCATION                 ${_location}
    LINK_INTERFACE_LIBRARIES "${_libraries}"
    OUTPUT_NAME              ${target}${_ext}
    )

endmacro (target_link_ocaml_libraries)

macro (ocaml_parse_macro_arguments prefix arg_names)
  set (_arg_names ${arg_names})
  set (current_arg)
  foreach (arg ${ARGN})
    list (FIND _arg_names ${arg} idx)
    if (idx LESS 0)
      list (APPEND ${prefix}_${current_arg} ${arg})
    else (idx LESS 0)
      set (current_arg ${arg})
      set (${prefix}_${current_arg})
    endif (idx LESS 0)
  endforeach (arg)
endmacro (ocaml_parse_macro_arguments)

macro (add_ocaml_target target)
  ocaml_parse_macro_arguments (${target} "SOURCES;HEADERS;INCLUDES;LIBRARIES;MAIN" ${ARGN})
  include_directories (${${target}_INCLUDES})
  if (${target}_MAIN)
    add_ocaml_executable (${target} ${${target}_HEADERS} ${${target}_SOURCES} ${${target}_MAIN})
  else (${target}_MAIN)
    add_ocaml_library (${target} ${${target}_HEADERS} ${${target}_SOURCES})
  endif (${target}_MAIN)
  target_link_ocaml_libraries (${target} ${${target}_LIBRARIES})
endmacro (add_ocaml_target)

# install_ocaml_targets (executables DESTINATION destination)
#   See description above.
macro (install_ocaml_targets)
  set (_argv ${ARGV})

  list (GET _argv -2 _key)
  list (GET _argv -1 _destination)

  if (NOT ${_key} STREQUAL "DESTINATION")
    message (SEND_ERROR "Incorrect syntax for install_ocaml_executable. No destination specified.")
  endif (NOT ${_key} STREQUAL "DESTINATION")

  list (REMOVE_AT _argv -2 -1)

  foreach (_target ${_argv})
    get_target_property (_kind     ocaml.${_target} KIND)
    get_target_property (_location ocaml.${_target} LOCATION)
    if (${_kind} STREQUAL "EXECUTABLE")
      install (PROGRAMS ${_location} DESTINATION ${_destination})
    elseif (${_kind} STREQUAL "LIBRARY")
      install (FILES ${_location} DESTINATION ${_destination})
    endif (${_kind} STREQUAL "EXECUTABLE")
    set_target_properties (ocaml.${_target} PROPERTIES INSTALL_LOCATION ${_destination})
  endforeach (_target)

endmacro (install_ocaml_targets)

# install_ocaml_interfaces (...)
#   See description above.
macro (install_ocaml_interfaces target)
  set (_argv ${ARGN})

  list (GET _argv -2 _key)
  list (GET _argv -1 _destination)

  if (NOT ${_key} STREQUAL "DESTINATION")
    message (SEND_ERROR "Incorrect syntax for install_ocaml_interfaces. No destination specified.")
  endif (NOT ${_key} STREQUAL "DESTINATION")

  list (REMOVE_AT _argv -2 -1)

  if (OCAML_${target}_NATIVE)
    set (_ext ".cmx")
  else (OCAML_${target}_NATIVE)
    set (_ext ".cmi")
  endif (OCAML_${target}_NATIVE)

  set (_interfaces)
  foreach (_interface ${_argv})
    list (APPEND _interfaces "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ocaml.${target}.dir/${_interface}${_ext}")
  endforeach (_interface)

  install (FILES ${_interfaces} DESTINATION ${_destination})

endmacro (install_ocaml_interfaces)

# install_ocaml_exports (...)
#   See description above.
macro (install_ocaml_exports)
  set (_argv ${ARGV})

  list (GET _argv -4 _dest_keyword)
  list (GET _argv -3 _destination)
  list (GET _argv -2 _file_keyword)
  list (GET _argv -1 _file)

  if (NOT ${_dest_keyword} STREQUAL "DESTINATION" OR NOT ${_file_keyword} STREQUAL "FILE")
    message (SEND_ERROR "Incorrect syntax for install_ocaml_targets. No destination or file specified.")
  endif (NOT ${_dest_keyword} STREQUAL "DESTINATION" OR NOT ${_file_keyword} STREQUAL "FILE")

  list (REMOVE_AT _argv -4 -3 -2 -1)

  set (_file "${CMAKE_CURRENT_BINARY_DIR}/${_file}")

  file (WRITE "${_file}")
  file (APPEND "${_file}"  "get_filename_component (_self_dir   \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)\n")

  set (_tmp)
  set (_temp "${_destination}")
  while (_temp)
    get_filename_component (_temp "${_temp}" PATH)
    set (_tmp "${_tmp}/..")
  endwhile (_temp)

  file (APPEND "${_file}" "get_filename_component (_prefix_dir \"\${_self_dir}${_tmp}\" ABSOLUTE)\n\n")

  foreach (_target ${_argv})
    get_target_property (_kind            ocaml.${_target} KIND)
    get_target_property (_location        ocaml.${_target} INSTALL_LOCATION)
    get_target_property (_link_interfaces ocaml.${_target} LINK_INTERFACE_LIBRARIES)
    get_target_property (_name            ocaml.${_target} OUTPUT_NAME)
    if (${_kind} STREQUAL "EXECUTABLE")
      file (APPEND "${_file}" "add_executable (${_target} IMPORTED)\n")
      file (APPEND "${_file}" "set_target_properties (${_target} PROPERTIES\n")
      file (APPEND "${_file}" "  IMPORTED_LOCATION \"\${_prefix_dir}/${_location}/${_name}\"\n")
      file (APPEND "${_file}" "  )\n\n")
    elseif (${_kind} STREQUAL "LIBRARY")
      file (APPEND "${_file}" "add_library (${_target} UNKNOWN IMPORTED)\n")
      file (APPEND "${_file}" "set_target_properties (${_target} PROPERTIES\n")
      file (APPEND "${_file}" "  KIND                     \"LIBRARY\"\n")
      file (APPEND "${_file}" "  IMPORTED_LOCATION        \"\${_prefix_dir}/${_location}/${_name}\"\n")
      file (APPEND "${_file}" "  LINK_INTERFACE_LIBRARIES \"${_link_interfaces}\"\n")
      file (APPEND "${_file}" "  OUTPUT_NAME              ${_name}\n")
      file (APPEND "${_file}" "  )\n\n")
    endif (${_kind} STREQUAL "EXECUTABLE")
  endforeach (_target)

  install (FILES "${_file}" DESTINATION "${_destination}")

endmacro (install_ocaml_exports)

# gen_ocaml_lexers (outfilesname sources)
#   See description above.
macro (gen_ocaml_lexers outfilesname)
  set (outfilesname)
  foreach (source ${ARGN})
    get_filename_component (_name_we ${source} NAME_WE)
    set (_output ${CMAKE_CURRENT_BINARY_DIR}/${_name_we}.ml)
    if (NOT EXISTS ${_output})
      execute_process (
  COMMAND           ${CMAKE_OCaml_LEX} -o ${_output} -ml ${source}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
    endif (NOT EXISTS ${_output})
    add_custom_command (OUTPUT ${_output}
      COMMAND           ${CMAKE_OCaml_LEX} -o ${_output} -ml ${source}
      MAIN_DEPENDENCY   ${source}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )
    list (APPEND ${outfilesname} ${_output})
    add_custom_target (ocaml.${_name_we}.ml DEPENDS ${_output})
  endforeach (source)
endmacro (gen_ocaml_lexers)

# gen_ocaml_parsers (outfilesname sources)
#   See description above.
macro (gen_ocaml_parsers outfilesname)
  set (outfilesname)
  foreach (source ${ARGN})
    get_filename_component (_name_we ${source} NAME_WE)
    set (_output ${CMAKE_CURRENT_BINARY_DIR}/${_name_we}.mli ${CMAKE_CURRENT_BINARY_DIR}/${_name_we}.ml)
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_name_we}.ml)
      execute_process (
  COMMAND           ${CMAKE_OCaml_YACC} -b ${CMAKE_CURRENT_BINARY_DIR}/${_name_we} ${source}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
    endif (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${_name_we}.ml)
    add_custom_command (OUTPUT ${_output}
      COMMAND           ${CMAKE_OCaml_YACC} -b ${CMAKE_CURRENT_BINARY_DIR}/${_name_we} ${source}
      MAIN_DEPENDENCY   ${source}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      )
    list (APPEND ${outfilesname} ${_output})
    add_custom_target (ocaml.${_name_we}.ml DEPENDS ${_output})
  endforeach (source)
endmacro (gen_ocaml_parsers)
