#pragma once

#include <QObject>
#include <QQmlEngine>

class SpectrumVolumeMode : public QObject {
  Q_OBJECT
  QML_ELEMENT

public:
  /**
   * IMPORTANT: The numeric values of this enum should NOT be changed, because
   * they will break backward compatibility!
   */
  enum Mode { Linear = 0, Logarithmic = 1 };
  Q_ENUM(Mode)

  Mode mode() const;
};
