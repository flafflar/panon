# Even though this is required, we mark it as optional because it takes
# multiple steps to find it successfully.
find_package(FFTW3f QUIET)

if(NOT ${FFTW3f_FOUND})
  # Try to find the regular FFTW3 library. In some distros (Archlinux, possibly
  # others), the FFTw3fConfig.cmake file is in the /usr/lib/cmake/fftw3
  # directory, instead of the correct /usr/lib/cmake/fftw3f directory. So, we
  # try to find FFTW3 and give its path as a hint to FFTW3f.
  find_package(FFTW3 QUIET)
  if(${FFTW3_FOUND})
    find_package(FFTW3f HINTS ${FFTW3_DIR})
  endif()
endif()

if(NOT ${FFTW3f_FOUND})
  # Try to find the library with pkgconfig.
  find_package(PkgConfig QUIET)
  if(${PkgConfig_FOUND})
    pkg_check_modules(FFTW3f fftw3f)
  endif()
  if(${FFTW3f_FOUND})
    # Trick CMake into thinking it has found FFTW3f, so it doesn't display it
    # in the list of not found packages.
    get_property(PACKAGES_NOT_FOUND GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(REMOVE_ITEM PACKAGES_NOT_FOUND FFTW3f)
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${PACKAGES_NOT_FOUND})
    get_property(PACKAGES_FOUND GLOBAL PROPERTY PACKAGES_FOUND)
    list(APPEND PACKAGES_FOUND FFTW3f)
    set_property(GLOBAL PROPERTY PACKAGES_FOUND ${PACKAGES_FOUND})
  endif()
endif()

if(NOT ${FFTW3f_FOUND})
  # Force trigger the failure message to the user.
  find_package(FFTW3f REQUIRED)
endif()

add_library(AudioBackendCommon STATIC
  AudioDeviceInfo.h
  AudioBufferProcessor.cpp
  AbstractAudioBackend.cpp
)

target_link_libraries(AudioBackendCommon
  PUBLIC
    Qt6::Core
  PRIVATE
    ${FFTW3f_LIBRARIES}
)

add_subdirectory(pulseaudio)
