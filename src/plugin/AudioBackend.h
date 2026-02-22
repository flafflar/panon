#pragma once

#include <QObject>
#include <QQmlEngine>
#include <qlogging.h>

#include "AbstractAudioBackend.h"
#include "AudioBackendSettings.h"
#include "AudioDeviceInfo.h"

class AudioBackend : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QQmlListProperty<AudioDeviceInfoQML> devices READ devices NOTIFY
                 devicesChanged)

  Q_PROPERTY(AudioDeviceInfoQML *currentDevice READ currentDevice WRITE
                 setCurrentDevice)

  Q_PROPERTY(int fps READ fps WRITE setFps)

  Q_PROPERTY(SpectrumVolumeMode::Mode spectrumVolumeMode READ spectrumVolumeMode
                 WRITE setSpectrumVolumeMode)

  Q_PROPERTY(
      float spectrumLogFloor READ spectrumLogFloor WRITE setSpectrumLogFloor)

  AbstractAudioBackend *m_backend;

public:
  static AudioBackend *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

  AudioBackend();

  AbstractAudioBackend *backend() const { return this->m_backend; }

  QQmlListProperty<AudioDeviceInfoQML> devices();

  AudioDeviceInfoQML *currentDevice() const;

  int fps() const;

  SpectrumVolumeMode::Mode spectrumVolumeMode() const;

  float spectrumLogFloor() const;

public Q_SLOTS:

  void setCurrentDevice(AudioDeviceInfoQML *device);

  void setFps(int fps);

  void setSpectrumVolumeMode(SpectrumVolumeMode::Mode mode);

  void setSpectrumLogFloor(float floor);

Q_SIGNALS:
  void devicesChanged();
};
