//-----------------------------------------------------------------------------
/** @file pentobi_qml/qml/ScoreElement2.qml
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

import QtQuick 2.0

Item {
    id: root

    property alias color1: point1.color
    property alias color2: point2.color
    property bool isFinal: false
    property int value
    property real pointSize

    Rectangle {
        id: point1

        width: pointSize
        height: width
        radius: width / 2
        anchors.verticalCenter: root.verticalCenter
    }
    Rectangle {
        id: point2

        width: pointSize
        height: width
        radius: width / 2
        anchors.left: point1.right
        anchors.verticalCenter: root.verticalCenter
    }
    Text {
        text: value
        color: "#6D6367"
        width: root.width - point1.width - point2.width - anchors.leftMargin
        anchors {
            left: point2.right
            leftMargin: 0.3 * point1.width
            verticalCenter: root.verticalCenter
        }
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
        font.underline: isFinal
        font.pixelSize: 1.7 * point1.width
    }
}