import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: valueDisplay

    property string dynaStatus: "E" // Stato iniziale: Nuovo
    property real dynaValue: 0     // Valore iniziale: 0

    width: 780
    height: 350
    radius: 10
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
        opacity: 0.1
    }


    Text {
        id: valueText
        text: dynaValue.toFixed(0) // Mostra il valore con una cifra decimale
        anchors.centerIn: parent
        font.family: dinCondensedFont.name
        font.pixelSize: 150
        color: dynaStatus === "E" ? "red" : dynaStatus === "S" ? "black" : dynaStatus === "N" ? "green" : "blue"
    }



}
