import QtQuick 2.0
import QtQuick.Dialogs 1.2
import "Main.js" as Logic

FileDialog {
    title: qsTr("Open")
    nameFilters: [ qsTr("Blokus games (*.blksgf)"), qsTr("All files (*)") ]
    folder: root.folder != "" ? root.folder : (isAndroid ? "file:///sdcard" : shortcuts.desktop)
    onAccepted: {
        root.folder = folder
        Logic.openFileUrl()
    }
    onVisibleChanged: if (! visible) gameDisplay.forceActiveFocus() // QTBUG-48456
}
