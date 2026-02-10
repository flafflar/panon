#include "QtAudioBackend.h"

#include <QAudioDevice>
#include <iostream>

QtAudioBackend::QtAudioBackend() : mediaDevices(new QMediaDevices(this)) {}

QList<QString> QtAudioBackend::listDevices() {
  for (QAudioDevice &device : this->mediaDevices->audioInputs()) {
    std::cout << device.description().toStdString() << std::endl;
  }

  return {QLatin1String("Device 1"), QLatin1String("Device 2")};
}
