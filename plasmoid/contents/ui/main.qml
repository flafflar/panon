import QtQuick 2.0
import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore

PlasmoidItem {
    CreateVenv{}

    readonly property var cfg:plasmoid.configuration

    preferredRepresentation: Plasmoid.compactRepresentation

    compactRepresentation: Spectrum{}

    fullRepresentation: Spectrum{}

    toolTipItem: cfg.hideTooltip?tooltipitem:null

    Plasmoid.backgroundHints: PlasmaCore.Types.DefaultBackground | PlasmaCore.Types.ConfigurableBackground

    Item{id:tooltipitem}

}
