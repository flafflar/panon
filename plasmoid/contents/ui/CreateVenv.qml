import org.kde.plasma.core 2.0 as PlasmaCore
import "utils.js" as Utils
/*
 * Create virtual environment for auto-installing dependencies.
 */
PlasmaCore.DataSource {
    engine: 'executable'
    readonly property string createVenv:Utils.create_venv()
    connectedSources: [createVenv]
    onNewData:{
        // Show back-end errors.
        console.log(data.stdout)
        console.log(data.stderr)
    }
}
