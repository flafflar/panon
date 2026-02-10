#pragma once

#include <QList>
#include <QMediaDevices>
#include <QString>
#include <qmediadevices.h>

class QtAudioBackend : public QObject {
  Q_OBJECT

public:
  QtAudioBackend();

  QList<QString> listDevices();

private:
  QMediaDevices *mediaDevices;
};
