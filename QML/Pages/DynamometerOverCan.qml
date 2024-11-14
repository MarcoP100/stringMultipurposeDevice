import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.VirtualKeyboard
import QtQuick.VirtualKeyboard.Settings
import com.wifiManager 1.0
import "../colors.js" as Colors
import "../Components" as MyComponents
import "../Dialogs" as MyDialogs
import "../networkUtils.js" as NetUtils


Item {
    id: dynaOnCanPage
    width: 800
    height: 480

    property Flickable flickableRef

    Connections {
        target: wifiScanDialog.updateButton
        function onUpdateList() {
            console.log("Pulsante cliccato! Gestione dal livello superiore con Connections.");
            NetUtils.startNetworkScan();
        }
    }

    Connections {
        target: WiFiManager
        function onScanCompleted(networks) {

            for (var i = 0; i < networks.length; i++) {
                console.log("SSID:", networks[i].ssid, "Richiede password:", networks[i].requiresPassword, "Salvata:", networks[i].networkKnown);
            }
            NetUtils.updateNetworkList(networks, wifiListModel);

        }
    }


    ListModel {
        id: wifiListModel
    }


    MyDialogs.WifiScanDialog {
        id: wifiScanDialog
        anchors.centerIn: parent
        maxVisibleItems: 5
        wifiManagerBusy: WiFiManager.busy
        wifiListModel: wifiListModel
        visible: false

    }



}


