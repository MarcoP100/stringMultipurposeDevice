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
    property var selectedNetwork: { "ssid": "", "lock": false, "isSaved": false }
    property string wifiStatusColor: Colors.DARK_GREY_COLOR
    property string wifiIconStatusImg: "qrc:/wifi_off_bk.svg"
    property string networkName: "Selezionare rete"

    Component.onCompleted: {
        if (WiFiManager) {
            WiFiManager.checkCurrentConnectionStatus();
        }
    }


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

        function onSsidChanged(ssid) {
            console.log("Nome della rete aggiornato:", ssid);
            if (ssid !== ""){
                networkName = ssid;
            }
        }


        function onWifiStatusChanged(status){

            console.log("Status:", status)
            switch (status) {

            case WiFiManager.Timeout:
                console.log("La connessione ha superato il timeout.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Connecting:
                console.log("Connessione in corso...");
                wifiStatusColor = Colors.YELLOW_COLOR
                wifiIconStatusImg = "qrc:/wifi_connecting.svg"
                break;
            case WiFiManager.Connected:
                console.log("Connessione avvenuta con successo.");
                wifiStatusColor = Colors.GREEN_COLOR
                wifiIconStatusImg = "qrc:/wifi_on.svg"
                wiFiStatusBar.iconState = ""
                break;
            case WiFiManager.WrongPassword:
                console.log("Password errata. Riprovare.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                passwordDialog.visible = true;
                break;
            case WiFiManager.ConnectionFailed:
                console.log("Connessione fallita.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Disconnected:
                console.log("Disconnesso.");
                wifiStatusColor = Colors.DARK_GREY_COLOR
                wifiIconStatusImg = "qrc:/wifi_off_bk.svg"
                break;
            case WiFiManager.StatusUnknown:
                console.log("Stato Sconosciuto.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;


            default:
                console.log("Stato della connessione sconosciuto.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
            }
        }

    }


    /*Connections {
        target: wifiScanDialog
        function onNetworkSelected(ssid, requiresPassword, networkKnown) {
            selectedNetwork = {
                "ssid": ssid,
                "lock": requiresPassword,
                "isSaved": networkKnown
            };
            wifiScanDialog.visible = false;
            let result = NetUtils.checkPasswordLock(selectedNetwork);
            if (result.shouldConnect) {
                if (WiFiManager) {
                    WiFiManager.connectToNetwork(selectedNetwork.ssid, "");
                }
            }
            passwordDialog.visible = true;
        }
    }*/


    ListModel {
        id: wifiListModel
    }


    MyComponents.WiFiStatusBar {
        id: wiFiStatusBar

        circleBorderColor: wifiStatusColor
        iconStatusImg: wifiIconStatusImg
        circleRadius : 20
        networkText: networkName === "" ? "Selezionare rete" : networkName

        anchors {

            left: parent.left
            top: parent.top
            leftMargin: 10
            topMargin: 10

        }

        onLongPressed: {
            console.log("Pressione lunga icona wifi");
        }

        onSsidClicked: {
            console.log("Pressione mouse area wifi");
            //if (wifiStatusColor !== Colors.GREEN_COLOR){
                wifiScanDialog.visible = true;
                NetUtils.startNetworkScan();
            //}

        }

    }


    MyDialogs.WifiScanDialog {
        id: wifiScanDialog
        anchors.centerIn: parent
        maxVisibleItems: 5
        wifiManagerBusy: WiFiManager.busy
        wifiListModel: wifiListModel
        visible: false
        z: 10

        onMouseAreaClicked:{
            wifiScanDialog.visible = false;
            passwordDialog.visible = true
        }

        onNetworkSelected:function(selectedSsid, selectedRequiresPassword, selectedNetworkKnown){

            console.log("Rete selezionata:", selectedSsid);
            // Gestione della selezione della rete
            selectedNetwork = {
                "ssid": selectedSsid,
                "lock": selectedRequiresPassword,
                "isSaved": selectedNetworkKnown
            };
            networkName = selectedNetwork.ssid;
            wifiScanDialog.visible = false;  // Chiudi il dialogo dopo la selezione
            let result = NetUtils.checkPasswordLock(selectedNetwork);
            if (result.shouldConnect) {
                if (WiFiManager) {
                    WiFiManager.connectToNetwork(selectedNetwork.ssid, "");
                }
            } else {
                passwordDialog.visible = true;
            }

        }


    }


    MyDialogs.PasswordDialog {
        id: passwordDialog
        keyboard_active: keyboard.active
        keyboard_height: keyboard.height
        z: 10

        onConfirmClicked:{
            ;
        }
    }


    // Tastiera virtuale
    InputPanel {
        id: keyboard
        visible: active
        z: 10
        width: parent.width
        anchors.bottom: parent.bottom

    }
}


