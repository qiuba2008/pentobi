import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import QtWebView 1.1
import pentobi 1.0

Window {
    property string language: {
        var s = Qt.locale().name
        var pos = s.indexOf("_")
        if (pos >= 0)
            s = s.substr(0, pos)
        if (s !== "C" && s !== "de")
            s = "C"
        return s
    }
    property url startUrl: helpFileExtractor.extract(language)

    // Instead of initializing webView.url with startUrl, we provide an init
    // function that needs to be called after show() to work around an issue
    // with the initial zoom factor of WebView sometimes very large on Android
    // (last tested with Qt 5.9.2)
    function init() { webView.url = startUrl }

    width: isAndroid ? Screen.desktopAvailableWidth : Math.min(Screen.pixelDensity * 150, Screen.desktopAvailableWidth)
    height: isAndroid ? Screen.desktopAvailableHeight : Math.min(Screen.pixelDensity * 180, Screen.desktopAvailableHeight)
    title: qsTr("Pentobi Help")
    color: theme.backgroundColor
    flags: Qt.Dialog

    // We'd like to hide the window instead of closing it but it doesn't work on
    // Android to make it visible again (problem with WebView? Last tested with
    // Qt 5.8-rc), so for now we destroy it and recreate it when needed.
    onClosing: helpWindowLoader.source = ""

    WebView {
        id: webView

        anchors.fill: parent
    }
    HelpFileExtractor { id: helpFileExtractor }
}
