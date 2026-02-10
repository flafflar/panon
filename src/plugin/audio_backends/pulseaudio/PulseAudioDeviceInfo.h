#pragma once

#include "AudioDeviceInfo.h"

#include <pulse/introspect.h>

class PulseAudioDeviceInfo : public AudioDeviceInfo {
public:
  PulseAudioDeviceInfo(const pa_sink_info *info)
      : m_type(AudioDeviceType::Sink),
        m_identifier(QString::fromUtf8(info->name)), m_pulseIndex(info->index),
        m_name(QString::fromUtf8(info->description)) {}

  PulseAudioDeviceInfo(const pa_source_info *info)
      : m_type(AudioDeviceType::Source),
        m_identifier(QString::fromUtf8(info->name)), m_pulseIndex(info->index),
        m_name(QString::fromUtf8(info->description)) {}

  PulseAudioDeviceInfo(const PulseAudioDeviceInfo &info)
      : m_type(info.m_type), m_identifier(info.m_identifier),
        m_pulseIndex(info.m_pulseIndex), m_name(info.m_name) {}

  virtual ~PulseAudioDeviceInfo() override = default;

  virtual AudioDeviceType type() const override { return this->m_type; }

  virtual QString identifier() const override { return this->m_identifier; }

  virtual QString name() const override { return this->m_name; }

  virtual bool isSame(AudioDeviceInfo &other) const override {
    // Try to downcast to this class.
    PulseAudioDeviceInfo *info = dynamic_cast<PulseAudioDeviceInfo *>(&other);
    // If the downcast fails, then the two devices are definitely not the same.
    if (info == nullptr)
      return false;

    return this->m_pulseIndex == info->m_pulseIndex;
  }

  uint32_t pulseIndex() const { return this->m_pulseIndex; }

private:
  // Whether the device is a source or sink.
  AudioDeviceType m_type;

  // The Pulse name of the device.
  QString m_identifier;

  // The identifier index for the Pulse device.
  uint32_t m_pulseIndex;

  // The user-friendly name of the device.
  QString m_name;
};
