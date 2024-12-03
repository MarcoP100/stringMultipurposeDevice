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


    Text {
        id: valueText
        text: dynaValue.toFixed(1) // Mostra il valore con una cifra decimale
        anchors.centerIn: parent
        font.pixelSize: 130
        color: dynaStatus === "E" ? "red" : dynaStatus === "S" ? "black" : dynaStatus === "N" ? "green" : "blue"
    }

}
