#pragma once

#include <QColor>
#include <QtQuick/QQuickPaintedItem>

class PanonWidget : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor FINAL)
  QML_ELEMENT

public:
  PanonWidget(QQuickItem *parent = nullptr);

  QColor color() const;
  void setColor(const QColor &color);

private:
  QColor m_color;
};
