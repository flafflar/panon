#include "ShaderWidget.h"
#include <QLabel>
#include <QPainter>

ShaderWidget::ShaderWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_color("white") {}

QColor ShaderWidget::color() const { return this->m_color; }

void ShaderWidget::setColor(const QColor &color) { this->m_color = color; }

void ShaderWidget::paint(QPainter *painter) {
  painter->fillRect(0, 0, this->width(), this->height(), this->m_color);

  QLabel label(QLatin1String("Hello"));
  label.render(painter);
}
