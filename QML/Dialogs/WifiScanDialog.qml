import QtQuick 2.15
import QtQuick.Controls 2.15
import "../colors.js" as Colors
import "../Components" as MyComponents


Item {

    id: wifiScanDialog

    width: parent.width
    height: parent.height

    signal mouseAreaClicked()
    signal networkSelected(string ssid, bool requiresPassword, bool networkKnown)

    property bool wifiManagerBusy: false
    property var wifiListModel
    property int maxVisibleItems: 1
   
    // Rende il pulsante `updateButton` accessibile da altri componenti
    property alias updateButton: updateButton

    // Dialogo manuale creato all'interno del contenitore principale
    Rectangle {
        id: networkDialog
        visible: true
        width: 400
        height: wifiManagerBusy ? 70 : Math.min(networkList.listViewContentHeight + 10, maxVisibleItems * 40 + 10)
        color: "white"
        radius: 2
        anchors.centerIn: parent
        border.color: "black"
        border.width: 1

        /*onVisibleChanged: {
            if (!networkDialog.visible) {
                wifiManager.stopNetworkScan();  // Ferma la scansione quando il dialogo viene chiuso
            }
        }*/

        // Busy Indicator che gira durante la ricerca
        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: wifiManagerBusy  // Il BusyIndicator gira quando wifiManager � occupato
            visible: wifiManagerBusy  // Visibile solo quando � occupato
        }

        Column {
            width: parent.width  // Lascia un margine per evitare che il contenuto tocchi i bordi
            spacing: 10  // Spazio tra gli elementi nella colonna
            anchors {
                fill: parent
                margins: 5  // Aggiungi un margine per evitare che il contenuto tocchi i bordi
            }

            MyComponents.NetworkList {
                id: networkList
                maxVisibleItemsList: maxVisibleItems
                flickableAreaWidth: parent.width
                // Inserisci un modello di esempio per il test
                listModel: wifiListModel
                listVisible: !wifiManagerBusy

                onNetworkSelected: function(selectedSsid, selectedRequiresPassword, selectedNetworkKnown) {
                    console.log("Rete selezionata:", selectedSsid);
                    wifiScanDialog.networkSelected(selectedSsid, selectedRequiresPassword, selectedNetworkKnown);
                }
            }
        }

        Button {
            id: updateButton

            // Definisci un segnale personalizzato
            signal updateList()


            width: 50
            height: 50
            anchors.top: networkDialog.bottom
            anchors.horizontalCenter: networkDialog.horizontalCenter
            anchors.topMargin: 10
            visible: networkDialog.visible
            enabled: !wifiManagerBusy
            contentItem: Image {
                source: wifiManagerBusy ? "qrc:/refresh_gy_30.svg" :  "qrc:/refresh.svg"
                anchors.centerIn: parent
            }

            onClicked: {
                updateList();
            }

        }
    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"
        opacity: 0.7  // Regola l'opacit� per ottenere l'effetto di oscuramento
        z: (networkDialog.z - 1)

        MouseArea {
            id: dialogBackgroundArea
            anchors.fill: parent // Copre l'intero schermo
            onClicked: {

                    wifiScanDialog.mouseAreaClicked();

            }



        }
    }

}
