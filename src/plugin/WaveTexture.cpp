#include "WaveTexture.h"
#include "AudioBackend.h"
#include "PulseAudioBackend.h"

#include <QPainter>
#include <qlogging.h>
#include <qpoint.h>

WaveTexture::WaveTexture(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_textureImage(0, 0, QImage::Format_RGBA8888) {
}

/*** QML Properties ***/

AudioBackend *WaveTexture::audioBackend() const { return this->m_audioBackend; }

void WaveTexture::setAudioBackend(AudioBackend *audioBackend) {
  if (audioBackend == this->m_audioBackend)
    return;

  this->m_audioBackend = audioBackend;

  connect(audioBackend->backend(), &PulseAudioBackend::buffersUpdated, this,
          &WaveTexture::updateTexture);

  Q_EMIT this->audioBackendChanged();
}

/*** Drawing ***/

void WaveTexture::updateTexture() {
  qDebug() << "Updating the texture...";

  // Draw the texture in the stored image.
  this->drawTextureImage();

  // Resize the widget based on the texture.
  this->setWidth(this->m_textureImage.width());
  this->setHeight(this->m_textureImage.height());

  // Force the widget to repaint.
  this->update(QRect(0, 0, this->width(), this->height()));
}

void WaveTexture::drawTextureImage() {
  PulseAudioBackend *backend = this->m_audioBackend->backend();

  std::vector<uint8_t> leftBuffer = backend->leftBuffer();
  std::vector<uint8_t> rightBuffer = backend->rightBuffer();

  // The width of the image, which is also the number of samples in the buffer
  // (1 sample = 1 pixel).
  int width = leftBuffer.size();

  // Resize the image to the size of the buffers.
  this->m_textureImage = QImage(width, 1, QImage::Format_RGBA8888);

  // Since the image is 1 pixel tall, it's buffer is just the first line's
  // buffer.
  uchar *imageBuffer = this->m_textureImage.scanLine(0);

  for (int i = 0; i < width; i++) {
    // The red channel is the left channel.
    imageBuffer[0] = leftBuffer[i];
    // The green channel is the right channel.
    imageBuffer[1] = rightBuffer[i];
    // The blue channel is not used, so leave it to 0.
    imageBuffer[2] = 0;
    // The alpha channel is not used, but we set it to 255 so we can visually
    // debug the texture if needed.
    imageBuffer[3] = 255;

    // Advance to the next pixel.
    imageBuffer += 4;
  }
}

void WaveTexture::paint(QPainter *painter) {
  // Paint the texture image on the widget.
  painter->drawImage(QPoint(0, 0), this->m_textureImage);
}
