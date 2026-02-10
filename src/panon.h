#pragma once

#include <QObject>
#include <QQmlEngine>

class Panon : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  static Panon *create(QQmlEngine *, QJSEngine *);

private:
  Panon();
};
