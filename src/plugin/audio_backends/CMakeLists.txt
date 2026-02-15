find_package(FFTW3 REQUIRED)

add_library(AudioBackendCommon STATIC
  AudioDeviceInfo.h
  AudioBufferProcessor.cpp
  AbstractAudioBackend.cpp
)

target_link_libraries(AudioBackendCommon
  PUBLIC
    Qt6::Core
  PRIVATE
    fftw3f
)

add_subdirectory(pulseaudio)
