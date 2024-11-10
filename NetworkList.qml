import QtQuick 2.15
import QtQuick.Controls 2.15


Flickable {

    id: flickableArea
    width: parent.width
    height: Math.min(listView.contentHeight, maxVisibleItems * 40) // Altezza dinamica o massimo consentito
    contentHeight: listView.contentHeight
    clip: true  // Limita il contenuto visibile all'altezza dell'elemento

    signal networkSelected(string ssid, bool requiresPassword, bool networkKnown)

    ListView {
        id: listView
        width: parent.width
        height: contentHeight  // Altezza dinamica per mostrare tutto il contenuto
        model: wifiListModel
        focus: true  // Assicurati che la ListView mantenga il focus
        interactive: true  // Mantieni la ListView interattiva
        visible: !wifiManager.busy

        delegate: ItemDelegate {
            width: listView.width
            height: 40  // Altezza di ogni elemento per calcolare la dimensione complessiva

            Rectangle {
                width: parent.width
                height: 40
                color: wifiSelectedArea.pressed ? "#d0d0d0" : "#f0f0f0"
                border.color: "#cccccc"
                border.width: 1

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text:  model.ssid

                }



                Image {
                    id: netLock
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    source: requiresPassword ? "qrc:/wifiLock.svg" : "qrc:/wifiUnLock.svg"
                    visible: true // Mostra l'icona del lucchetto se � richiesta una password

                }

                Image {
                    id: netKnown
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: netLock.left
                    anchors.rightMargin: 10
                    source: networkKnown ? "qrc:/key_on.svg" : "qrc:/key_no.svg"
                    visible: requiresPassword // Mostra l'icona del lucchetto se � richiesta una password

                }

                MouseArea {
                    id: wifiSelectedArea
                    anchors.fill: parent
                    onClicked: {
                        console.log("Rete selezionata:", ssid);
                        // Gestione della selezione della rete
                        networkSelected(model.ssid, model.requiresPassword, model.networkKnown);
                        /*selectedNetwork = {
                            "ssid": model.ssid,
                            "lock": model.requiresPassword,
                            "isSaved": model.networkKnown
                        };
                        networkName.text = selectedNetwork.ssid;
                        //addNetworkPassword(selectedNetwork.ssid);
                        networkDialog.visible = false;  // Chiudi il dialogo dopo la selezione
                        needsPassword();*/
                    }
                }
            }

        }

    }
}
