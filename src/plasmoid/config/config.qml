import QtQuick 2.0
import org.kde.plasma.configuration 2.0

ConfigModel {
    ConfigCategory {
         name: i18nc("@title","Back-end")
         icon: 'preferences-desktop-sound'
         source: 'config/ConfigAudioBackend.qml'
    }
}
