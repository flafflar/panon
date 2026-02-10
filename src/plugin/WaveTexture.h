#pragma once

#include <QImage>
#include <QQuickPaintedItem>

#include "AudioBackend.h"

// Renders the waveform data from an audio device into a texture that is
// provided to the visualization shader.
class WaveTexture : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(AudioBackend *audioBackend READ audioBackend WRITE setAudioBackend
                 NOTIFY audioBackendChanged)

public:
  WaveTexture(QQuickItem *parent = nullptr);

  AudioBackend *audioBackend() const;

  void paint(QPainter *painter) override;

public Q_SLOTS:
  void setAudioBackend(AudioBackend *audioBackend);

Q_SIGNALS:
  void audioBackendChanged();

private:
  AudioBackend *m_audioBackend;

  // The image on which we draw the texture.
  QImage m_textureImage;

  // Takes the waveform data from the audio backend and draws them on
  // m_textureImage;
  void drawTextureImage();

private Q_SLOTS:
  void updateTexture();
};
