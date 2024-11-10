import QtQuick 2.15
import com.wifiManager 1.0
import "colors.js" as Colors
import "NetworkList.qml"

Item {

    id: wifiScanDialog

    // Dialogo manuale creato all'interno del contenitore principale
    Rectangle {
        id: networkDialog
        visible: false
        width: 400
        height: wifiManager.busy ? 75 : Math.min(listView.contentHeight + 10, maxVisibleItems * 40 + 10)
        color: "white"
        radius: 2
        anchors.centerIn: parent
        border.color: "black"
        border.width: 1
        z: 10  // Porta il dialogo in primo piano

        /*onVisibleChanged: {
                    if (networkDialog.visible) {
                        updateTimer.start();  // Avvia il timer quando il dialogo diventa visibile
                    } else {
                        updateTimer.stop();  // Ferma il timer quando il dialogo viene chiuso
                    }
                }*/

        onVisibleChanged: {
            if (!networkDialog.visible) {
                wifiManager.stopNetworkScan();  // Ferma la scansione quando il dialogo viene chiuso
            }
        }



        // Busy Indicator che gira durante la ricerca
        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: wifiManager.busy  // Il BusyIndicator gira quando wifiManager � occupato
            visible: wifiManager.busy  // Visibile solo quando � occupato
        }

        Column {
            width: parent.width  // Lascia un margine per evitare che il contenuto tocchi i bordi
            spacing: 10  // Spazio tra gli elementi nella colonna
            anchors.horizontalCenter: parent.horizontalCenter  // Centra la colonna orizzontalmente rispetto al dialogo
            anchors {
                fill: parent
                margins: 5  // Aggiungi un margine per evitare che il contenuto tocchi i bordi
            }


            NetworkList {
                        anchors.fill: parent
                        maxVisibleItems: 5
                    }

            onNetworkSelected: {
                    console.log("Rete selezionata:", ssid);
                    selectedNetwork = {
                        "ssid": ssid,
                        "lock": requiresPassword,
                        "isSaved": networkKnown
                    };
                    networkName.text = selectedNetwork.ssid;
                    // Add more logic like password check here
                }
        }


    }

    Button {
        id: updateButton
        text: "Aggiorna"
        anchors.top: networkDialog.bottom
        anchors.horizontalCenter: networkDialog.horizontalCenter
        anchors.topMargin: 10
        visible: !wifiManager.busy && networkDialog.visible
        z: 10
        onClicked: {
            startNetworkScan()
        }

    }

}
