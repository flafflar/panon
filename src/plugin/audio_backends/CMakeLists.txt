add_library(AudioBackendCommon STATIC
  AudioDeviceInfo.h
  AudioBufferProcessor.cpp
  AbstractAudioBackend.cpp
)

target_link_libraries(AudioBackendCommon
  PUBLIC
    Qt6::Core
)

add_subdirectory(pulseaudio)
