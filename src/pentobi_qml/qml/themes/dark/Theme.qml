import QtQuick 2.0

QtObject {
    property color messageTextColor: fontColorPosInfo
    property color messageBackgroundColor: "#333"
    property color backgroundColor: "#131313"
    property color fontColorScore: "#C8C1BE"
    property color fontColorCoordinates: "#A09B99"
    property color fontColorPosInfo: "#C8C1BE"
    property color colorBlue: "#0077D2"
    property color colorYellow: "#EBCD23"
    property color colorRed: "#E63E2C"
    property color colorGreen: "#00C000"
    property color colorPurple: "#a12CCF"
    property color colorOrange: "#F09217"
    property color colorStartingPoint: "#82777E"
    property color backgroundButtonPressed: Qt.lighter(backgroundColor, 3)
    property color selectionColor: "steelblue"
    property color selectedTextColor: "#EEE"
    property color toolBarTextColor: fontColorPosInfo
    property color analyzeBackgroundColor: "#222"
    property color analyzeLineColor: "#888"
    property color analyzeMiddleLineColor: "#555"
    property real pieceListOpacity: 0.94
    property real toPlayColorLighter: 1.7

    function getImage(name) {
        if (startsWith(name, "frame-")
                || startsWith(name, "junction-")
                || startsWith(name, "linesegment-")
                || startsWith(name, "piece-manipulator")
                || startsWith(name, "quarter-square-")
                || startsWith(name, "square-")
                || startsWith(name, "triangle-"))
            return "themes/light/" + name + ".svg"
        return "themes/dark/" + name + ".svg"
    }

    // Helper function for Qt <5.8, which doesn't support String.startsWith()
    function startsWith(s, s1) { return s.substring(0, s1.length) === s1 }
}
