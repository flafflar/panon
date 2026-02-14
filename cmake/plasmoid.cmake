# We need extra-cmake-modules to get the KDEInstallDirs module.
find_package(ECM "6.0.0" REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)

find_package(KF6 REQUIRED
  COMPONENTS
    # We need this for the plasma_install_package() function.
    Config
)

define_property(TARGET PROPERTY "PLASMOID_CONTENT_TARGETS"
  BRIEF_DOCS "Internal - do not use"
)

function(add_plasmoid NAME)
  cmake_parse_arguments(
    PARSE_ARGV 1
    args
    ""
    "METADATA;OUTPUT_TARGET"
    ""
  )

  if(NOT DEFINED args_METADATA)
    message(FATAL_ERROR "You must provide a metadata.json file")
  endif()

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/plasmoids/${NAME}/metadata.json
    COMMAND ${CMAKE_COMMAND}
      -E make_directory
      ${CMAKE_BINARY_DIR}/plasmoids/${NAME}/contents
    COMMAND ${CMAKE_COMMAND}
      -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/${args_METADATA}
      ${CMAKE_BINARY_DIR}/plasmoids/${NAME}/metadata.json
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/${args_METADATA}
  )

  add_custom_target(plasmoid_${NAME}
    ALL
    DEPENDS
      ${CMAKE_BINARY_DIR}/plasmoids/${NAME}/metadata.json
      $<TARGET_PROPERTY:plasmoid_${NAME},PLASMOID_CONTENT_TARGETS>
  )

  plasma_install_package(
    ${CMAKE_BINARY_DIR}/plasmoids/${NAME}
    ${NAME}
    plasmoids
  )

  if(DEFINED args_OUTPUT_TARGET)
    set(${args_OUTPUT_TARGET} plasmoid_${NAME} PARENT_SCOPE)
  endif()
endfunction()

function(__plasmoid_add_file NAME PREFIX FILE)
  if(NOT PREFIX MATCHES "^/")
    message(FATAL_ERROR "PREFIX should start with a slash")
  endif()

  # Convert FILE into an absolute path.
  cmake_path(ABSOLUTE_PATH FILE BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  # We have verified above that PREFIX starts with a slash, so it's safe to
  # concatenate the paths like this.
  set(OUT_PATH ${CMAKE_BINARY_DIR}/plasmoids/${NAME}/contents${PREFIX})

  # Get the filename of the file in a variable called FILENAME.
  cmake_path(GET FILE FILENAME FILENAME)
  # Append the filename to OUT_PATH to create the final output path of the file.
  cmake_path(APPEND OUT_PATH ${FILENAME})
  # Get the directory path of the output path in OUT_DIR.
  cmake_path(GET OUT_PATH PARENT_PATH OUT_DIR)

  add_custom_command(
    OUTPUT ${OUT_PATH}
    # First, make sure the directory exists, or else the copy command might
    # fail.
    COMMAND ${CMAKE_COMMAND}
      -E make_directory
      ${OUT_DIR}
    # Copy the file.
    COMMAND ${CMAKE_COMMAND}
      -E copy_if_different
      ${FILE}
      ${OUT_PATH}
    DEPENDS
      ${FILE}
  )

  # Create a target name by taking the output path relative to CMAKE_BINARY_DIR
  # and replacing slashes (which are not allowed in target names) with
  # underscores.
  cmake_path(RELATIVE_PATH
    OUT_PATH
    BASE_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE TARGET_NAME
  )
  string(REPLACE "/" "_" TARGET_NAME ${TARGET_NAME})

  add_custom_target(${TARGET_NAME} DEPENDS ${OUT_PATH})

  # Add the target to the list of dependencies of the main plasmoid target.
  set_property(TARGET plasmoid_${NAME}
    APPEND PROPERTY PLASMOID_CONTENT_TARGETS ${TARGET_NAME}
  )
endfunction()

function(plasmoid_add_files NAME)
  cmake_parse_arguments(
    PARSE_ARGV 1
    args
    ""
    "PREFIX"
    "FILES"
  )

  foreach(FILE IN LISTS args_FILES)
    __plasmoid_add_file(${NAME} ${args_PREFIX} ${FILE})
  endforeach()
endfunction()
