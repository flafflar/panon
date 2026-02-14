add_subdirectory(plugin)

add_plasmoid(xyz.flafflar.panon
  METADATA plasmoid/metadata.json
  OUTPUT_TARGET plasmoid_target
)

add_subdirectory(plasmoid)

add_subdirectory(shaders)

find_program(PLASMOIDVIEWER NAMES plasmoidviewer OPTIONAL)

if(PLASMOIDVIEWER MATCHES "NOTFOUND$")
  message(WARNING "Could not find the plasmoidviewer executable. The launch\
  command that is used to test the plasmoid will not be available.")
else()
  add_custom_target(launch
    DEPENDS
      plugin_installed
      ${plasmoid_target}
    COMMAND ${CMAKE_COMMAND}
      -E env
      QML_IMPORT_PATH=${CMAKE_BINARY_DIR}
      ${PLASMOIDVIEWER}
      --applet ${CMAKE_BINARY_DIR}/plasmoids/xyz.flafflar.panon
  )
endif()
