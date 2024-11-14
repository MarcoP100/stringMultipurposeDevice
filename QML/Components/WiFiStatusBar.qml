import QtQuick 2.15
import "../Components" as MyComponents
import "../networkUtils.js" as NetUtils

Item {
    // Indicatore di stato (LED)
    id: wifiIconItem

    signal longPressed()


    Rectangle {
        id: wifiIcon
        width: 40
        height: 40
        radius: 20
        color: "lightblue"
        border.color: "black" // Colore del bordo
        border.width: 1  // Spessore del bordo
        z: 20
        anchors {

            left: dynaOnCanPage.left
            top: dynaOnCanPage.top
            leftMargin: 10
            topMargin: 10

        }
        state: ""
        // Animazione per trasformare l'icona in pulsante
        states: [
            State {
                name: "expanded"
                PropertyChanges {
                    target: wifiIcon
                    width: (40 + networkName.width + 30) // Calcola la larghezza massima
                    height: 40
                    radius: height / 2
                }
                PropertyChanges {
                    target: networkName
                    opacity: 1.0
                }
            }
        ]
        transitions: Transition {
            from: ""
            to: "expanded"
            reversible: true
            SequentialAnimation {
                PropertyAnimation {
                    target: wifiIcon
                    properties: "width, height"
                    duration: 400
                    easing.type: Easing.OutBack
                }
                PropertyAnimation {
                    target: wifiIcon
                    property: "radius"
                    duration: 200
                }
                PropertyAnimation {
                    target: networkName
                    property: "opacity"
                    duration: 300
                }
            }

        }

        MyComponents.WiFiStatusIcon{
            id: wifiStatusIcon
            anchors.centerIn: parent
            wifiStatusColor: "white"
            borderColor: Colors.DARK_GREY_COLOR
            iconSource: "qrc:/wifi_off_bk.svg"
            z: 20
            circleRadius: 20
            visible: true

            onSingleClicked{

                wifiIcon.state = wifiIcon.state === "" ? "expanded" : ""

            }

            onLongPressed{
                wifiIconItem.longPressed();
            }

            }


                // Nome della rete WiFi, che appare solo quando espanso
                Text {
                    id: networkName
                    text: "Selezionare rete"
                    anchors.verticalCenter: wifiIcon.verticalCenter
                    anchors.left: wifiStatusCircle.right
                    anchors.leftMargin: 10 // Margine tra l'icona e il testo
                    width: Math.min(implicitWidth, 500) // Limita la larghezza al massimo a 200 oppure alla larghezza del rettangolo - margine
                    elide: Text.ElideRight // Troncamento con "..." se il testo supera la larghezza
                    opacity: wifiIcon.state === "expanded" ? 1.0 : 0.0 // Inizia invisibile
                    color: "black"
                }

                MouseArea {
                    id: networkMouseArea
                    anchors.fill: parent // Copre l'intero schermo
                    visible: wifiIcon.state === "expanded"// Visibile solo quando il dialogo � aperto
                    onClicked: {
                        networkDialog.visible = true;
                        startNetworkScan();
                    }

                }

            }

        }
