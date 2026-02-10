#include "panonwidget.h"
#include <qcolor.h>

PanonWidget::PanonWidget(QQuickItem *parent) : QQuickPaintedItem(parent) {}

QColor PanonWidget::color() const { return this->m_color; }

void PanonWidget::setColor(const QColor &color) { this->m_color = color; }
