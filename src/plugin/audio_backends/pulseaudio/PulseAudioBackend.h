#pragma once

#include <QList>
#include <QString>
#include <QThread>
#include <memory>

#include "AudioDeviceInfo.h"
#include "AudioWorker.h"

class PulseAudioBackend : public QObject {
  Q_OBJECT

  AudioWorker worker;
  QThread workerThread;

public:
  PulseAudioBackend();

  QList<AudioDeviceInfoQML *> devices();

  AudioDeviceInfoQML *currentDevice() { return this->m_currentDevice; };

  int fps() const { return this->m_fps; };

  std::vector<float> leftBuffer() const { return this->worker.getLeftBuffer(); }
  std::vector<float> rightBuffer() const {
    return this->worker.getRightBuffer();
  }

public Q_SLOTS:

  void setCurrentDevice(AudioDeviceInfoQML *device) {
    std::shared_ptr<AudioDeviceInfo> dev = *device;
    this->worker.setCurrentDevice(
        std::dynamic_pointer_cast<PulseAudioDeviceInfo>(dev));
  }

  void setFps(int fps) {
    this->m_fps = fps;
    this->worker.setFps(fps);
  }

private Q_SLOTS:
  void updateDevices();

Q_SIGNALS:
  void devicesChanged();

  void buffersUpdated();

private:
  QList<AudioDeviceInfoQML *> m_devices;

  // The current device selected by the user.
  AudioDeviceInfoQML *m_currentDevice;

  int m_fps;
};
