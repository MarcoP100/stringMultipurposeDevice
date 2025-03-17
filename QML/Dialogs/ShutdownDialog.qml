import QtQuick 2.15
import QtQuick.Controls 2.15
import "../colors.js" as Colors
import "../Components" as MyComponents


Item {

    id: shutdownDialog

    width: parent.width
    height: parent.height

    signal mouseAreaClicked()
    signal shutdownClicked()
    signal rebootClicked()

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"
        opacity: 0.7  // Regola l'opacit� per ottenere l'effetto di oscuramento
        z: 1

        MouseArea {
            id: dialogBackgroundArea
            anchors.fill: parent // Copre l'intero schermo
            onClicked: shutdownDialog.mouseAreaClicked();
        }
    }


    // Dialogo manuale creato all'interno del contenitore principale
    Rectangle {
        id: shutdownRect
        visible: true
        width: 300
        height: 200
        color: "white"
        radius: 2
        anchors.centerIn: parent
        border.color: "black"
        border.width: 1
        z: 2

        Column {
            width: parent.width - 40 // Lascia un margine per evitare che il contenuto tocchi i bordi
            spacing: 15  // Spazio tra gli elementi nella colonna
            anchors.centerIn: parent

            Button {
                text: "Riavvia"
                width: parent.width * 0.8
                height: 50
                font.pixelSize: 18
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: shutdownDialog.rebootClicked()
            }

            Button {
                text: "Spegni"
                width: parent.width * 0.8
                height: 50
                font.pixelSize: 18
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: shutdownDialog.shutdownClicked()
            }
        }

    }



}
