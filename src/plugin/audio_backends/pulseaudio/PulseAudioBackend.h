#pragma once

#include <QList>
#include <QString>
#include <QThread>
#include <memory>

#include "AbstractAudioBackend.h"
#include "AudioDeviceInfo.h"
#include "AudioWorker.h"

class PulseAudioBackend : public AbstractAudioBackend {
  Q_OBJECT

  AudioWorker worker;
  QThread workerThread;

public:
  PulseAudioBackend();

  virtual QList<AudioDeviceInfoQML *> devices() override;

  virtual AudioDeviceInfoQML *currentDevice() const override {
    return this->m_currentDevice;
  };

  virtual int fps() const override { return this->m_fps; };

public Q_SLOTS:

  virtual void setCurrentDevice(AudioDeviceInfoQML *device) override {
    std::shared_ptr<AudioDeviceInfo> dev = *device;
    this->worker.setCurrentDevice(
        std::dynamic_pointer_cast<PulseAudioDeviceInfo>(dev));
  }

  virtual void setFps(int fps) override {
    this->m_fps = fps;
    this->worker.setFps(fps);
  }

private Q_SLOTS:
  void updateDevices();

Q_SIGNALS:

private:
  QList<AudioDeviceInfoQML *> m_devices;

  // The current device selected by the user.
  AudioDeviceInfoQML *m_currentDevice;

  int m_fps;
};
