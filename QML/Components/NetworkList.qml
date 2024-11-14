import QtQuick 2.15
import QtQuick.Controls 2.15

Flickable {

    id: flickableArea

    property var listModel
    property int maxVisibleItems
    property bool listVisible
    property int flickableAreaWidth

    property alias listViewContentHeight: listView.contentHeight;

    signal networkSelected(string ssid, bool requiresPassword, bool networkKnown)

    width: flickableAreaWidth
    height: Math.min(listViewContentHeight, maxVisibleItems * 40) // Altezza dinamica o massimo consentito
    contentHeight: listViewContentHeight
    clip: true  // Limita il contenuto visibile all'altezza dell'elemento
    // Imposta il comportamento per fermarsi ai limiti
    boundsBehavior: Flickable.StopAtBounds
    interactive: false
    flickableDirection: Flickable.VerticalFlick

    ListView {
        id: listView

        width: parent.width
        height: contentHeight  // Altezza dinamica per mostrare tutto il contenuto
        model: listModel
        focus: true  // Assicurati che la ListView mantenga il focus
        interactive: true  // Mantieni la ListView interattiva
        visible: listVisible
        clip: true
        delegate: WiFiNetworkItem {

            wifiItemWidth: listView.width
            wifiItemHeight: 40
            ssid: model.ssid
            requiresPassword: model.requiresPassword
            networkKnown: model.networkKnown
            onNetworkSelected: function(selectedSsid, selectedRequiresPassword, selectedNetworkKnown) {
                // Passa il segnale al livello superior
                flickableArea.networkSelected(selectedSsid, selectedRequiresPassword, selectedNetworkKnown);
            }
        }
    }

}
