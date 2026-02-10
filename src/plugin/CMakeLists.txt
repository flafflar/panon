add_library(QtAudioBackend STATIC
  audio_backends/QtAudioBackend.h
  audio_backends/QtAudioBackend.cpp
)

target_link_libraries(QtAudioBackend
  PUBLIC
    Qt6::Core
    Qt6::Multimedia
)

add_subdirectory(audio_backends)

add_library(PanonPlugin SHARED
  PanonPlugin.cpp
  ShaderWidget.cpp
  AudioBackend.cpp
  WaveViewer.cpp
  WaveTexture.cpp
)

target_link_libraries(PanonPlugin
  PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Qml
    Qt6::Quick
    QtAudioBackend
    PulseAudioBackend
)

# Place the output library in a namespaced directory so QML can load it
# directly from the build directory.
set_target_properties(PanonPlugin PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/xyz/flafflar/panon
)

# Also copy the qmldir file to the namespaced directory in the build directory
# so QML can load the plugin directly.
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/xyz/flafflar/panon/qmldir
  COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
    ${CMAKE_BINARY_DIR}/xyz/flafflar/panon/qmldir
  DEPENDS
    qmldir
)

add_custom_target(qmldir ALL
  DEPENDS
    PanonPlugin
    ${CMAKE_BINARY_DIR}/xyz/flafflar/panon/qmldir
)

# A custom target for commands that depend on the plugin being installed in the
# build directory.
add_custom_target(plugin_installed
  DEPENDS
    PanonPlugin
    qmldir
)

install(DIRECTORY
  DESTINATION ${KDE_INSTALL_QMLDIR}/xyz
  DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ             GROUP_EXECUTE
    WORLD_READ             WORLD_EXECUTE
)
install(DIRECTORY
  DESTINATION ${KDE_INSTALL_QMLDIR}/xyz/flafflar
  DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ             GROUP_EXECUTE
    WORLD_READ             WORLD_EXECUTE
)
install(DIRECTORY
  DESTINATION ${KDE_INSTALL_QMLDIR}/xyz/flafflar/panon
  DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ             GROUP_EXECUTE
    WORLD_READ             WORLD_EXECUTE
)

install(
  TARGETS PanonPlugin
  DESTINATION ${KDE_INSTALL_QMLDIR}/xyz/flafflar/panon
  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ             GROUP_EXECUTE
    WORLD_READ             WORLD_EXECUTE
)

install(
  FILES qmldir
  DESTINATION ${KDE_INSTALL_QMLDIR}/xyz/flafflar/panon
  PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ             GROUP_EXECUTE
    WORLD_READ             WORLD_EXECUTE
)
