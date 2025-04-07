import QtQuick 2.15
import "../colors.js" as Colors

    Rectangle {

        id: wifiStatusCircle

        signal singleClicked()
        signal longPressed()

        // Propriet� configurabili dall'esterno
        property color wifiStatusColor: "white"
        property color borderColor: Colors.DARK_GREY_COLOR
        property string iconSource: "qrc:/wifi_off_bk.svg"
        property int widthIcon: 20


        width: widthIcon
        height: widthIcon
        radius: 5
        color: wifiStatusColor
        border.color: borderColor    // Colore del bordo
        border.width: 1  // Spessore del bordo
        anchors.left: parent.left // Assicura che sia ancorato a sinistra
        anchors.verticalCenter: parent.verticalCenter // Centra verticalmente

        Image {
            id: wifiStatusIcon
            anchors.centerIn: wifiStatusCircle
            source: iconSource

        }

        MouseArea {
            id: wifiMouseArea
            anchors.fill: parent

            onClicked: {

                console.log("Icona cliccata brevemente");
                wifiStatusCircle.singleClicked();  // Emissione del segnale `clicked`

            }

            onPressAndHold:{
                console.log("Icona premuta a lungo");
                wifiStatusCircle.longPressed();  // Emissione del segnale `longPressed`
            }

            Timer {
                id: longPressTimer
                interval: 3000 // 3 secondi
                repeat: false
                onTriggered: wifiMouseArea.onPressAndHold
            }

            onPressed: {
                longPressTimer.start(); // Avvia il timer per rilevare la pressione prolungata
            }

            onReleased: {
                console.log("Mouse rilasciato");
                longPressTimer.stop(); // Ferma il timer per evitare falsi trigger
            }
        }
    }

