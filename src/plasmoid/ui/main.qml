import QtQuick
import QtQuick.Layouts
import org.kde.plasma.plasmoid
import org.kde.plasma.components as PC3
import xyz.flafflar.panon

PlasmoidItem {
  id: root

  fullRepresentation: RowLayout {
    anchors.fill: parent

    Layout.preferredWidth: 700
    Layout.maximumWidth: 700

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
      visible: false
    }

    ShaderEffectSource {
      id: waveTextureSource
      sourceItem: waveTexture
      width: waveTexture.width
      height: waveTexture.height
      visible: false
    }

    ShaderEffect {
      id: mainShader

      Layout.fillWidth: true
      Layout.fillHeight: true

      property variant iResolution: Qt.vector3d(mainShader.width, mainShader.height, 1)
      property variant iChannel0: waveTextureSource

      fragmentShader: "../shaders/wave.frag.qsb"
    }
  } 
}

