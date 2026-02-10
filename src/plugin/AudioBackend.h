#pragma once

#include <QObject>
#include <QQmlEngine>

#include "AudioDeviceInfo.h"
#include "PulseAudioBackend.h"

class AudioBackend : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QQmlListProperty<AudioDeviceInfoQML> devices READ devices NOTIFY
                 devicesChanged)

  Q_PROPERTY(AudioDeviceInfoQML *currentDevice READ currentDevice WRITE
                 setCurrentDevice)

  Q_PROPERTY(int fps READ fps WRITE setFps)

  PulseAudioBackend *m_backend;

public:
  static AudioBackend *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

  AudioBackend();

  PulseAudioBackend *backend() const { return this->m_backend; }

  QQmlListProperty<AudioDeviceInfoQML> devices();

  AudioDeviceInfoQML *currentDevice() const;

  int fps() const;

public Q_SLOTS:

  void setCurrentDevice(AudioDeviceInfoQML *device);

  void setFps(int fps);

Q_SIGNALS:
  void devicesChanged();
};
