#include "WaveTexture.h"

#include <QPainter>

#include "AudioBackend.h"
#include "PulseAudioBackend.h"

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
  // Draw the texture in the stored image.
  this->drawTextureImage();

  // Resize the widget based on the texture.
  this->setWidth(this->m_textureImage.width());
  this->setHeight(this->m_textureImage.height());

  // Force the widget to repaint.
  this->update(QRect(0, 0, this->width(), this->height()));
}

/**
 * Converts an audio sample (that is in the range [-1, 1]) to a pixel color
 * (that is in the range [0, 255]).
 *
 * This function uses the same conversion that PulseAudio uses to convert
 * float32 audio samples to u8 audio samples. This means that the range of the
 * output is not exactly [0, 255], because -1.0 maps to 1 instead of 0. 0 can
 * still appear though, in case the sample is less than -1.0. See the table
 * below for some example values.
 *
 * +--------+--------+
 * | sample | output |
 * +--------+--------+
 * |  -1.0  |     1  |
 * |   0.0  |   128  |
 * |   1.0  |   255  |
 * +--------+--------+
 *
 * @param sample The sample in the range [-1, 1] to convert. The sample is
 * allowed to be outside of the range [-1, 1], but it will be clamped if this is
 * the case.
 *
 * @return The sample converted in the [0, 255] range.
 */
uchar sampleToColor(const float sample) {
  float value = (sample * 127.0) + 128.0;

  // Clamp the value to [0, 255]
  if (value > 255.0) [[unlikely]] {
    value = 255.0;
  } else if (value < 0.0) [[unlikely]] {
    value = 0.0;
  }

  return std::rint(value);
}

void WaveTexture::drawTextureImage() {
  AbstractAudioBackend *backend = this->m_audioBackend->backend();

  AudioBuffers buffers = backend->audioBuffers();

  // The width of the image, which is also the number of samples in the buffer
  // (1 sample = 1 pixel).
  int width = buffers.left.size();

  // Resize the image to the size of the buffers.
  this->m_textureImage = QImage(width, 1, QImage::Format_RGBA8888);

  // Since the image is 1 pixel tall, it's buffer is just the first line's
  // buffer.
  uchar *imageBuffer = this->m_textureImage.scanLine(0);

  for (int i = 0; i < width; i++) {
    // The red channel is the left channel.
    imageBuffer[0] = sampleToColor(buffers.left[i]);
    // The green channel is the right channel.
    imageBuffer[1] = sampleToColor(buffers.right[i]);
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
