import QtQuick 2.15
import "../Components" as MyComponents
import "../networkUtils.js" as NetUtils
import "../colors.js" as Colors

/*Item {
    // Indicatore di stato (LED)
    id: wifiIconItem

    signal longPressed()
    signal ssidClicked()
    signal iconClicked()
    property int circleRadius: 20
    property string networkText: "Selezionare rete"
    property string circleBorderColor: Colors.DARK_GREY_COLOR
    property string iconStatusImg: "qrc:/wifi_off.svg"
    property bool restartTimerClosure: false
    property bool stopTimerClosure: false
    property bool expandIcon: false

    onRestartTimerClosureChanged: {
                if (restartTimerClosure) {
                    console.log("Propriet� diventata true, riavvio il Timer");
                    hideTimer.restart();  // Riavvia il Timer
                    restartTimerClosure = false;  // Riporta la propriet� a false
                }
            }
    onStopTimerClosureChanged: {
        if (stopTimerClosure) {
            hideTimer.stop();
        }
    }
*/

Rectangle {
    id: wifiIcon

    signal longPressed()
    signal ssidClicked()
    signal iconClicked()
    property int circleRadius: 20
    property string networkText: "Selezionare rete"
    property string circleBorderColor: Colors.DARK_GREY_COLOR
    property string iconStatusImg: "qrc:/wifi_off.svg"
    property bool restartTimerClosure: false
    property bool stopTimerClosure: false
    property bool expandIcon: false



    width: circleRadius * 2
    height: circleRadius * 2
    radius: circleRadius
    color: "lightblue"
    border.color: "black" // Colore del bordo
    border.width: 1  // Spessore del bordo

    anchors {

        left: parent.left
        top: parent.top

    }

    state: ""
    // Animazione per trasformare l'icona in pulsante
    states: [
        State {
            name: "expanded"
            when: expandIcon
            PropertyChanges {
                target: wifiIcon
                width: ((circleRadius * 2) + networkName.width + (circleRadius * 2) - 10) // Calcola la larghezza massima
                height: circleRadius * 2
                radius: circleRadius
                anchors.left: parent.left // Mantieni il wifiIcon ancorato a sinistra

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

    onRestartTimerClosureChanged: {
        if (restartTimerClosure) {
            console.log("Propriet� diventata true, riavvio il Timer");
            hideTimer.restart();  // Riavvia il Timer
            restartTimerClosure = false;  // Riporta la propriet� a false
        }
    }
    onStopTimerClosureChanged: {
        if (stopTimerClosure) {
            hideTimer.stop();
        }
    }
    MyComponents.WiFiStatusIcon{
        id: wifiStatusIcon
        anchors.left: parent.left // Assicura che sia ancorato a sinistra
        anchors.verticalCenter: parent.verticalCenter // Mantieni la posizione verticale centrata
        wifiStatusColor: "white"
        borderColor: circleBorderColor
        iconSource: iconStatusImg
        z: parent.z + 1
        circleRadius: circleRadius
        visible: true

        onSingleClicked: {

            wifiIcon.iconClicked()
        }

        onLongPressed: {
            wifiIcon.longPressed();
        }

    }

    // Nome della rete WiFi, che appare solo quando espanso
    Text {
        id: networkName
        text: networkText
        anchors.verticalCenter: wifiIcon.verticalCenter
        anchors.left: wifiStatusIcon.right
        anchors.leftMargin: 10 // Margine tra l'icona e il testo
        width: Math.min(implicitWidth, 500) // Limita la larghezza al massimo a 200 oppure alla larghezza del rettangolo - margine
        elide: Text.ElideRight // Troncamento con "..." se il testo supera la larghezza
        opacity: wifiIcon.state === "expanded" ? 1.0 : 0.0 // Inizia invisibile
        color: "black"
    }

    MouseArea {
        id: networkMouseArea
        anchors.fill: parent // Copre l'intero schermo
        visible: true
        onClicked: {
            if (wifiIcon.state === "expanded") {
                ssidClicked(); // Emetti il segnale solo quando � espanso
            }
        }

    }

    // Timer per nascondere il nome della rete
    Timer {
        id: hideTimer
        interval: 5000  // Durata in millisecondi (es. 5000 = 5 secondi)
        repeat: false   // Non ripetere il timer
        onTriggered: {
            wifiIcon.expandIcon = false;  // Nascondi il nome della rete
        }

    }

}

//}
