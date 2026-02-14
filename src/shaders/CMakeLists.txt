#[[ Should I do it like this? I'll keep it here and decide later.
add_custom_target(shaders)
qt6_add_shaders(shaders "shaders"
  FILES
    wave.frag
)

get_property(shader_files TARGET shaders PROPERTY _qt_resource_source_files)

plasmoid_add_files(xyz.flafflar.panon
  PREFIX /shaders
  FILES ${shader_files}
)
#]]

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wave.frag.qsb
  COMMAND Qt6::qsb
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.frag
    --qt6
    -o ${CMAKE_CURRENT_BINARY_DIR}/wave.frag.qsb
  DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.frag
)

plasmoid_add_files(xyz.flafflar.panon
  PREFIX /shaders
  FILES ${CMAKE_CURRENT_BINARY_DIR}/wave.frag.qsb
)
