import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.kirigami 2.3 as Kirigami
import org.kde.kcmutils as KCM

import xyz.flafflar.panon

KCM.SimpleKCM {

  property alias cfg_fps: fps.value
  property string cfg_device

  AudioBackend {
    id: audioBackend

    fps: cfg_fps
  }

  onCfg_deviceChanged: {
    let device = audioBackend.devices.find((device) => device.identifier == cfg_device);
    if (device) audioBackend.currentDevice = device;
  }

  Kirigami.FormLayout {

    RowLayout {
      Kirigami.FormData.label: "Input device:"

      ComboBox {
        id: deviceComboBox
        model: audioBackend.devices
        textRole: showIdentifiers.checkState ? "identifier" : "name"

        onActivated: (index) => {
          cfg_device = audioBackend.devices[index].identifier
        }

        onModelChanged: {
          let index = audioBackend.devices.findIndex((device) => device.identifier == cfg_device);
          deviceComboBox.currentIndex = index;
        }
      }
    }

    CheckBox {
      id: showIdentifiers
      text: "Show raw device names"
    }

    RowLayout {
      Kirigami.FormData.label: "FPS:"

      SpinBox {
        id: fps

        stepSize: 1
        from: 1
        to: 120
      }
    }

    RowLayout {
      Kirigami.FormData.label: "Preview wave:"

      WaveViewer {
        Layout.fillWidth: true
        height: 64
        waveColor: "white"
        audioBackend: audioBackend
      }
    }
  }
}

