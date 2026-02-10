find_package(PulseAudio REQUIRED)

add_library(PulseAudioBackend STATIC
  PulseAudioBackend.h
  PulseAudioBackend.cpp
  AudioWorker.cpp
  PulseAudioDeviceInfo.h
)

target_link_libraries(PulseAudioBackend
  PUBLIC
    Qt6::Core
    AudioBackendCommon
  PRIVATE
    pulse
)
