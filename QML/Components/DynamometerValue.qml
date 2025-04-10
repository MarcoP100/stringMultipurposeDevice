import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: valueDisplay

    property string dynaStatus: "X" // Stato iniziale: Nuovo
    property string dynaValue: "------"     // Valore iniziale: 0

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
        text: dynaValue // Mostra il valore con una cifra decimale
        anchors.centerIn: parent
        font.family: dinCondensedFont.name
        font.pixelSize: 150
        color: dynaStatus === "E" ? "red" : dynaStatus === "S" ? "black" : dynaStatus === "N" ? "green" : "blue"
    }

    /*Text {
        id: umText
        text: "daN" // Mostra il valore con una cifra decimale
        x: 620
        y: 10
        font.family: "Arial"
        font.bold: true
        font.italic: true
        font.pixelSize: 60
        color: "black"
    }*/

}
