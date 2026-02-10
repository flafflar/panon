import QtQuick
import QtQuick.Layouts
import org.kde.plasma.plasmoid
import org.kde.plasma.components as PC3
import xyz.flafflar.panon

PlasmoidItem {
  id: root


  fullRepresentation: RowLayout {
    anchors.fill: parent

    AudioBackend {
      id: audioBackend

      currentDevice: {
        // Find the device specified in the configuration.
        let device = audioBackend.devices.find((device) => device.identifier == plasmoid.configuration.device)
        // If the device was found, use the device, else use the first device available.
        device ? device : audioBackend.devices[0]
      }

      fps: plasmoid.configuration.fps
    }

    WaveTexture {
      id: waveTexture
      audioBackend: audioBackend
    }

    WaveViewer {
      id: waveViewer
      Layout.fillWidth: true
      Layout.fillHeight: true

      waveColor: "white"

      audioBackend: audioBackend
    }
  } 
}

