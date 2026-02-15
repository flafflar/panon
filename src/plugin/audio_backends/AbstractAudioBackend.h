#pragma once

#include <span>

#include <QObject>

#include "AudioBufferProcessor.h"
#include "AudioDeviceInfo.h"

typedef AudioBufferProcessorOutputs AudioBuffers;

class AbstractAudioBackend : public QObject {
  Q_OBJECT
  Q_PROPERTY(
      QList<AudioDeviceInfoQML *> devices READ devices NOTIFY devicesChanged)
  Q_PROPERTY(AudioDeviceInfoQML *currentDevice READ currentDevice WRITE
                 setCurrentDevice)
  Q_PROPERTY(int fps READ fps WRITE setFps)

public:
  // TODO: Const?
  virtual QList<AudioDeviceInfoQML *> devices() = 0;

  virtual AudioDeviceInfoQML *currentDevice() const = 0;

  virtual int fps() const = 0;

  AudioBuffers audioBuffers() { return this->processor.getOutputs(); };

public Q_SLOTS:
  virtual void setCurrentDevice(AudioDeviceInfoQML *device) = 0;

  virtual void setFps(int fps) = 0;

Q_SIGNALS:
  void devicesChanged();

  void buffersUpdated();

protected:
  AudioBufferProcessor processor;
};
