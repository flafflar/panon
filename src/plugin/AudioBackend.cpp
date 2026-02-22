#include "AudioBackend.h"
#include "AudioDeviceInfo.h"
#include "PulseAudioBackend.h"
#include <qobject.h>
#include <qqmllist.h>

AudioBackend *AudioBackend::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) {
  Q_UNUSED(qmlEngine);
  Q_UNUSED(jsEngine);

  return new AudioBackend();
}

AudioBackend::AudioBackend() : QObject(nullptr) {
  this->m_backend = new PulseAudioBackend();

  connect(this->m_backend, &PulseAudioBackend::devicesChanged, this,
          &AudioBackend::devicesChanged);
}
QQmlListProperty<AudioDeviceInfoQML> AudioBackend::devices() {
  return QQmlListProperty<AudioDeviceInfoQML>(
      this, nullptr,
      [](auto *list) -> qsizetype {
        AudioBackend *self = qobject_cast<AudioBackend *>(list->object);
        return self->m_backend->devices().count();
      },
      [](auto *list, auto i) -> AudioDeviceInfoQML * {
        AudioBackend *self = qobject_cast<AudioBackend *>(list->object);
        return self->m_backend->devices().at(i);
      });
}

AudioDeviceInfoQML *AudioBackend::currentDevice() const {
  // TODO: Who deletes this object??
  return this->m_backend->currentDevice();
}

void AudioBackend::setCurrentDevice(AudioDeviceInfoQML *device) {
  this->m_backend->setCurrentDevice(device);
}

int AudioBackend::fps() const { return this->m_backend->fps(); }

void AudioBackend::setFps(int fps) { this->m_backend->setFps(fps); }

SpectrumVolumeMode::Mode AudioBackend::spectrumVolumeMode() const {
  return this->m_backend->spectrumVolumeMode();
}

void AudioBackend::setSpectrumVolumeMode(SpectrumVolumeMode::Mode mode) {
  this->m_backend->setSpectrumVolumeMode(mode);
}

float AudioBackend::spectrumLogFloor() const {
  return this->m_backend->spectrumLogFloor();
}

void AudioBackend::setSpectrumLogFloor(float floor) {
  this->m_backend->setSpectrumLogFloor(floor);
}
