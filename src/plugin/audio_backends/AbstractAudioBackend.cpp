#include "AbstractAudioBackend.h"

SpectrumVolumeMode::Mode AbstractAudioBackend::spectrumVolumeMode() const {
  return this->processor.getSpectrumVolumeMode();
}

void AbstractAudioBackend::setSpectrumVolumeMode(
    SpectrumVolumeMode::Mode mode) {
  this->processor.setSpectrumVolumeMode(mode);
}

float AbstractAudioBackend::spectrumLogFloor() const {
  return this->processor.getSpectrumLogFloor();
}

void AbstractAudioBackend::setSpectrumLogFloor(float floor) {
  this->processor.setSpectrumLogFloor(floor);
}
