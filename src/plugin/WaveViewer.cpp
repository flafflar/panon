#include "WaveViewer.h"

#include <QPainter>
#include <qline.h>
#include <qlogging.h>
#include <qvectornd.h>

#include "PulseAudioBackend.h"

WaveViewer::WaveViewer(QQuickItem *parent) : QQuickPaintedItem(parent) {}

AudioBackend *WaveViewer::audioBackend() const { return this->m_audioBackend; }

void WaveViewer::setAudioBackend(AudioBackend *audioBackend) {
  // Do nothing if there is no actual change in the object.
  if (this->m_audioBackend == audioBackend)
    return;

  // Disconnect all signals from the old audio backend.
  this->m_audioBackend->disconnect(this);

  this->m_audioBackend = audioBackend;

  // TODO: Reconnect this when it changes.
  connect(audioBackend->backend(), &PulseAudioBackend::buffersUpdated, this,
          &WaveViewer::redrawWave);

  Q_EMIT this->audioBackendChanged();
}

QColor WaveViewer::waveColor() const { return this->m_waveColor; }

void WaveViewer::setWaveColor(QColor color) {
  if (this->m_waveColor != color) {
    this->m_waveColor = color;
    Q_EMIT this->waveColorChanged();
  }
}

void WaveViewer::redrawWave() {
  // Redraw the whole widget area.
  this->update(QRect(0, 0, this->width(), this->height()));
}

void WaveViewer::paint(QPainter *painter) {
  qreal width = this->width();
  qreal height = this->height();
  // The vertical center of the widget.
  qreal vcenter = height / 2.0;

  // First, draw a one pixel line at exactly the center.
  painter->setPen(QPen(this->m_waveColor, 1));
  painter->drawLine(QLineF(0, vcenter, width, vcenter));

  // If we don't have a source for the wave data, don't draw anything else.
  if (this->m_audioBackend == nullptr)
    return;

  PulseAudioBackend *backend = this->m_audioBackend->backend();

  std::vector<float> leftBuffer = backend->leftBuffer();
  std::vector<float> rightBuffer = backend->rightBuffer();

  /*
  painter->fillRect(QRectF(0, 0, this->width(), this->height()),
                    this->m_waveColor);
  */

  // The width in pixels of one sample.
  qreal dx = width / leftBuffer.size();

  for (size_t i = 0; i < leftBuffer.size(); i++) {
    // Take the average of the two channels.
    qreal sample = (leftBuffer[i] + rightBuffer[i]) / 2.0;

    QRectF rect;
    if (sample > 0.0) {
      rect = QRectF(i * dx, vcenter - sample / 2.0, dx, height * sample / 2.0);
    } else {
      rect = QRectF(i * dx, vcenter + sample / 2.0, dx, height * sample / 2.0);
    }

    painter->fillRect(rect, this->m_waveColor);
  }
}
