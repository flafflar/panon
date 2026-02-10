#pragma once

#include <QDebug>
#include <QObject>
#include <memory>

enum class AudioDeviceType { Sink, Source };

inline QString audioDeviceTypeToString(AudioDeviceType type) {
  switch (type) {
  case AudioDeviceType::Sink:
    return QStringLiteral("sink");

  case AudioDeviceType::Source:
    return QStringLiteral("source");

  default:
    qFatal() << "Huh? Wha?";
    return QStringLiteral("unreachable");
  }
}

class AudioDeviceInfo {
public:
  virtual ~AudioDeviceInfo() = default;

  virtual AudioDeviceType type() const = 0;

  // A unique string that identifies this device to the respective backend.
  //
  // This should be unique across different sessions, since it is stored as a
  // setting.
  virtual QString identifier() const = 0;

  virtual QString name() const = 0;

  // Compares this device info with another and checks if they contain the same
  // fields. This does not actually check if the devices are the same. It just
  // checks the publicly exposed fields.
  bool isEquivalent(AudioDeviceInfo &other) const {
    return this->type() == other.type() && this->name() == other.name();
  }

  // Compares this device info with another and checks if they refer to the same
  // device. This only checks if the internal device is the same, the values of
  // the fields might be different.
  virtual bool isSame(AudioDeviceInfo &other) const = 0;
};

// The same class as AudioDeviceInfo, but defined as a QObject to be exposed to
// QML.
class AudioDeviceInfoQML : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString type READ type CONSTANT)
  Q_PROPERTY(QString identifier READ identifier CONSTANT)
  Q_PROPERTY(QString name READ name CONSTANT)

public:
  AudioDeviceInfoQML(std::shared_ptr<AudioDeviceInfo> info) : m_info(info) {}

  QString type() { return audioDeviceTypeToString(this->m_info->type()); }

  QString identifier() { return this->m_info->identifier(); }

  QString name() { return this->m_info->name(); }

  // TODO: Check if this is sound.
  operator std::shared_ptr<AudioDeviceInfo>() { return this->m_info; }

private:
  std::shared_ptr<AudioDeviceInfo> m_info;
};
