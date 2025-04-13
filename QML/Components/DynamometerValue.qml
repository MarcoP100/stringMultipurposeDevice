import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: valueDisplay

    property color displayColor: {
        if (dynamometerData.dynState === "X")
            return "blue";  // fallback fisso se non lampeggia
        if (dynamometerData.dynState === "S")
            return "black";
        if (dynamometerData.dynState === "N")
            return "green";
        if (dynamometerData.dynState === "E")
            return "red";
        return "gray";
    }

    width: 790
    height: 370
    radius: 4
    color: "lightblue"
    border.color: "black"
    border.width: 2

    FontLoader {
        id: dinCondensedFont
        source: "qrc:/fonts/DSEG7Classic-Regular.ttf"
    }

    Text {
        id: backText
        text: "888888" // Mostra il valore con una cifra decimale
        anchors.centerIn: parent
        font.family: dinCondensedFont.name
        font.pixelSize: 150
        color: "black"
        opacity: 0.03
    }


    Text {
        id: valueText
        text: dynamometerData.dynValue // Mostra il valore con una cifra decimale
        anchors.centerIn: parent
        font.family: dinCondensedFont.name
        font.pixelSize: 150
        color: displayColor

        SequentialAnimation on color {
            running: dynamometerData.dynState === "X"
            loops: Animation.Infinite
            ColorAnimation { from: "blue"; to: "transparent"; duration: 1000 }
            ColorAnimation { from: "transparent"; to: "blue"; duration: 1000 }
        }
    }

    Text {
        id: umText
        text: "daN" // Mostra il valore con una cifra decimale
        x: 620
        y: 10
        font.family: "Arial"
        font.bold: true
        font.italic: true
        font.pixelSize: 60
        color: "black"
    }

    Text {
        id: lostText
        text: "Pacchetti persi: " + dynamometerData.packetsLost
        font.pixelSize: 20
        color: "red"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 16
    }


}
