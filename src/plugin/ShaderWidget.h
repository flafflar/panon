#pragma once

#include <QColor>
#include <QtQuick/QQuickPaintedItem>

class ShaderWidget : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor FINAL)
  QML_ELEMENT

public:
  ShaderWidget(QQuickItem *parent = nullptr);

  QColor color() const;
  void setColor(const QColor &color);

  void paint(QPainter *painter) override;

private:
  QColor m_color;
};
