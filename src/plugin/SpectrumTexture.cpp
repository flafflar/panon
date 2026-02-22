#include "SpectrumTexture.h"

#include <QPainter>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <numbers>
#include <qassert.h>
#include <qlogging.h>

#include "AudioBackend.h"

SpectrumTexture::SpectrumTexture(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_textureImage(0, 0, QImage::Format_RGBA8888) {
}

/*** QML Properties ***/

AudioBackend *SpectrumTexture::audioBackend() const {
  return this->m_audioBackend;
}

void SpectrumTexture::setAudioBackend(AudioBackend *audioBackend) {
  if (audioBackend == this->m_audioBackend)
    return;

  this->m_audioBackend = audioBackend;

  connect(audioBackend->backend(), &AbstractAudioBackend::buffersUpdated, this,
          &SpectrumTexture::updateTexture);

  Q_EMIT this->audioBackendChanged();
}

/*** Drawing ***/

void SpectrumTexture::updateTexture() {
  // Draw the texture in the stored image.
  this->drawTextureImage();

  // Resize the widget based on the texture.
  this->setWidth(this->m_textureImage.width());
  this->setHeight(this->m_textureImage.height());

  // Force the widget to repaint.
  this->update(QRect(0, 0, this->width(), this->height()));
}

void SpectrumTexture::drawTextureImage() {
  AbstractAudioBackend *backend = this->m_audioBackend->backend();

  AudioBuffers buffers = backend->audioBuffers();

  // The width of the image, which is also the number of samples in the buffer
  // (1 sample = 1 pixel).
  int width = buffers.spectrum.size() * 0.4;

  // Resize the image to the size of the buffers.
  this->m_textureImage = QImage(width, 1, QImage::Format_RGBA8888);

  // Since the image is 1 pixel tall, it's buffer is just the first line's
  // buffer.
  uchar *imageBuffer = this->m_textureImage.scanLine(0);

  for (int i = 0; i < width; i++) {
    std::complex<float> normalized =
        buffers.fft[i] / (float)buffers.fft.size() * 255.0f;
    imageBuffer[0] = normalized.real();
    // The green channel is the right channel.
    imageBuffer[1] = normalized.imag();
    // The blue channel is not used, so leave it to 0.
    imageBuffer[2] = std::clamp(buffers.spectrum[i] * 255.0f, 0.0f, 255.0f);
    // std::clamp(amplitude * 255.0f * 3.0f, 0.0f, 255.0f);
    // The alpha channel is not used, but we set it to 255 so we can visually
    // debug the texture if needed.
    imageBuffer[3] = 255;

    // Advance to the next pixel.
    imageBuffer += 4;
  }
}

void SpectrumTexture::paint(QPainter *painter) {
  // Paint the texture image on the widget.
  painter->drawImage(QPoint(0, 0), this->m_textureImage);
}
