#include "panon.h"

Panon::Panon() : QObject(nullptr) {}

Panon *Panon::create(QQmlEngine *, QJSEngine *) {
  static Panon instance;
  QQmlEngine::setObjectOwnership(&instance, QQmlEngine::CppOwnership);
  return &instance;
}
