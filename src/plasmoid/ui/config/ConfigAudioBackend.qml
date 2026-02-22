import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCM

import xyz.flafflar.panon

KCM.SimpleKCM {

  property alias cfg_fps: fps.value
  property int cfg_fpsDefault

  property string cfg_device
  property string cfg_deviceDefault

  property alias cfg_spectrumVolumeMode: spectrumVolumeMode.currentValue
  property int cfg_spectrumVolumeModeDefault

  property alias cfg_spectrumLogFloor: spectrumLogFloor.value
  property double cfg_spectrumLogFloorDefault

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
      Kirigami.FormData.label: "Rate:"

      SpinBox {
        id: fps

        stepSize: 1
        from: 1
        to: 120

        textFromValue: (value) => value + " Hz"
      }

      Kirigami.ContextualHelpButton {
        toolTipText: "How many times per second we get audio data from the OS. \
Higher values mean the waveform updates more frequently, but it's shorter in \
width. Lower values mean the waveform is slower, but it's also longer in width."
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

    Kirigami.Separator {
      Kirigami.FormData.isSection: true
      Kirigami.FormData.label: "Spectrum settings"
    }

    ComboBox {
      id: spectrumVolumeMode

      Kirigami.FormData.label: "Volume mode:"

      textRole: "key"
      valueRole: "value"
      model: ListModel {
        ListElement {
          key: "Linear"
          value: SpectrumVolumeMode.Linear
        }
        ListElement {
          key: "Logarithmic (dB)"
          value: SpectrumVolumeMode.Logarithmic
        }
      }
    }

    RowLayout {
      Kirigami.FormData.label: "Minimum volume:"

      visible: spectrumVolumeMode.currentValue == SpectrumVolumeMode.Logarithmic

      SpinBox {
        id: spectrumLogFloor

        from: -120
        to: 0
        stepSize: 1

        textFromValue: (value) => value + " dB"
      }
    }
  }
}

