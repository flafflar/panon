#include "PulseAudioBackend.h"
#include "AudioWorker.h"

#include <memory>

#include <QDebug>
#include <QObject>
#include <qlogging.h>

PulseAudioBackend::PulseAudioBackend()
    : worker(this->processor), m_currentDevice(nullptr) {
  this->worker.moveToThread(&this->workerThread);

  // Start the worker when the thread launches.
  QObject::connect(&this->workerThread, &QThread::started, &this->worker,
                   &AudioWorker::run);

  // Update the device list every time the devices get updated.
  connect(&this->worker, &AudioWorker::devicesChanged, this,
          &PulseAudioBackend::updateDevices);

  connect(&this->worker, &AudioWorker::buffersUpdated, this,
          &PulseAudioBackend::buffersUpdated);

  this->workerThread.start();
}

QList<AudioDeviceInfoQML *> PulseAudioBackend::devices() {
  return this->m_devices;
}

void PulseAudioBackend::updateDevices() {
  // Whether there was an actual meaningful change in the devices.
  // We might get this message even when some non-meaningful change happened,
  // like a property we don't track. In that case, we don't want to emit a
  // signal that the devices changed, in order to prevent unneccesary updates.
  bool didUpdate = false;

  // Clear the devices list.
  // TODO: Fix memory leak.
  this->m_devices.clear();

  // Insert the new devices in the list.
  for (auto &info : this->worker.devices()) {
    AudioDeviceInfoQML *infoQml = new AudioDeviceInfoQML(info);
    this->m_devices.append(infoQml);
  }
  auto a = this->m_devices[0];

  Q_EMIT this->devicesChanged();
}
