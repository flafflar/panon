#pragma once

#include <QQuickPaintedItem>
#include <qtmetamacros.h>

#include "AudioBackend.h"

// Displays the audio wave coming from the audio backend. Used for debugging.
class WaveViewer : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(AudioBackend *audioBackend READ audioBackend WRITE setAudioBackend
                 NOTIFY audioBackendChanged)
  Q_PROPERTY(QColor waveColor READ waveColor WRITE setWaveColor NOTIFY
                 waveColorChanged)

public:
  WaveViewer(QQuickItem *parent = nullptr);

  void paint(QPainter *painter) override;

  AudioBackend *audioBackend() const;

  QColor waveColor() const;

public Q_SLOTS:
  void setAudioBackend(AudioBackend *audioBackend);

  void setWaveColor(QColor color);

  void redrawWave();

Q_SIGNALS:
  void audioBackendChanged();

  void waveColorChanged();

private:
  AudioBackend *m_audioBackend = nullptr;

  // The color of the wave.
  QColor m_waveColor;
};
