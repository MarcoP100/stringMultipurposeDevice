import QtQuick 2.15
import "../colors.js" as Colors

    Rectangle {

        id: tcpStatusCircle

        signal longPressed()

        // Propriet� configurabili dall'esterno
        property color tcpStatusColor: "white"
        property color borderColor: Colors.DARK_GREY_COLOR
        property string iconSource: "qrc:/tcp_disable_gy.svg"
        property int widthIcon: 20


        width: widthIcon
        height: widthIcon
        radius: 5
        color: tcpStatusColor
        border.color: borderColor    // Colore del bordo
        border.width: 1  // Spessore del bordo

        Image {
            id: tcpStatusIcon
            anchors.centerIn: tcpStatusCircle
            source: iconSource

        }

        MouseArea {
            id: tcpMouseArea
            anchors.fill: parent


            /*onPressAndHold:{
                console.log("Icona premuta a lungo");
                tcpStatusCircle.longPressed();  // Emissione del segnale `longPressed`
            }*/

            /*Timer {
                id: longPressTimer
                interval: 10// 3 secondi
                repeat: false
                onTriggered: tcpMouseArea.onPressAndHold
            }*/

            onPressed: {
                //longPressTimer.start(); // Avvia il timer per rilevare la pressione prolungata
            }

            onReleased: {
                //console.log("Mouse rilasciato");
                //longPressTimer.stop(); // Ferma il timer per evitare falsi trigger
                //tcpMouseArea.onPressAndHold
                tcpStatusCircle.longPressed();
            }
        }
    }

