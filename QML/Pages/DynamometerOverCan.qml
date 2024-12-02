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

    property string tcpStatusImg: "qrc:/tcp_disable_gy.svg"
    property string tcpStatusColor: Colors.DARK_GREY_COLOR

    Component.onCompleted: {
        if (WiFiManager) {
            WiFiManager.checkCurrentConnectionStatus();
        }
    }


    Connections {
        target: wifiScanDialog.updateButton
        function onUpdateList() {
            //console.log("Pulsante cliccato! Gestione dal livello superiore con Connections.");
            NetUtils.startNetworkScan();
        }
    }

    Connections {
        target: WiFiManager
        function onScanCompleted(networks) {

            //for (var i = 0; i < networks.length; i++) {
            //    console.log("SSID:", networks[i].ssid, "Richiede password:", networks[i].requiresPassword, "Salvata:", networks[i].networkKnown);
            //}
            NetUtils.updateNetworkList(networks, wifiListModel);

        }

        function onSsidChanged(ssid) {
            //console.log("Nome della rete aggiornato:", ssid);
            if (ssid !== ""){
                networkName = ssid;
            }
        }


        function onWifiStatusChanged(status){

            //console.log("Status:", status)
            wiFiStatusBar.expandIcon = true;
            if (!wifiScanDialog.visible){
               wiFiStatusBar.stopTimerClosure = false;
            }
            setProperty(wiFiStatusBar, "restartTimerClosure", true, wiFiStatusBar.stopTimerClosure)

            switch (status) {

            case WiFiManager.Timeout:
                //console.log("La connessione ha superato il timeout.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Connecting:
                //console.log("Connessione in corso...");
                wifiStatusColor = Colors.YELLOW_COLOR
                wifiIconStatusImg = "qrc:/wifi_connecting.svg"
                wiFiStatusBar.stopTimerClosure = true;
                break;
            case WiFiManager.Connected:
                //console.log("Connessione avvenuta con successo.");
                wifiStatusColor = Colors.GREEN_COLOR
                wifiIconStatusImg = "qrc:/wifi_on.svg"
                break;
            case WiFiManager.WrongPassword:
                //console.log("Password errata. Riprovare.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.ConnectionFailed:
                //console.log("Connessione fallita.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Disconnected:
                //console.log("Disconnesso.");
                wifiStatusColor = Colors.DARK_GREY_COLOR
                wifiIconStatusImg = "qrc:/wifi_off_bk.svg"
                break;
            case WiFiManager.StatusUnknown:
                //console.log("Stato Sconosciuto.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
                break;


            default:
                console.log("Stato della connessione sconosciuto.");
                wifiStatusColor = Colors.RED_COLOR
                wifiIconStatusImg = "qrc:/wifi_off.svg"
            }
        }

        function onReasonChangeState(newState, reason){
            //console.log("Nuovo stato:", newState, "Motivo:", reason);
            if (newState === WiFiManager.NM_DEVICE_STATE_FAILED && reason === WiFiManager.NM_DEVICE_STATE_REASON_NO_SECRETS) {
                console.log("Apertura popup password");
                passwordDialog.visible = true;
            }

        }
    }


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
            //console.log("Pressione lunga icona wifi");
            switch (wifiStatusColor) {
            case Colors.RED_COLOR:
            case Colors.DARK_GREY_COLOR:
            case Colors.YELLOW_COLOR:
                break;

            case Colors.GREEN_COLOR:

                if (WiFiManager) {
                    WiFiManager.disconnectNetwork();
                }
                break;

            default:
                //console.log("Stato non riconosciuto");
                break;
            }
        }

        onSsidClicked: {
            //console.log("Pressione mouse area wifi");
            //if (wifiStatusColor !== Colors.GREEN_COLOR){
            wifiScanDialog.visible = true;
            NetUtils.startNetworkScan();
            //}

        }

        onIconClicked: {
            setProperty(wiFiStatusBar, "expandIcon", !wiFiStatusBar.expandIcon, wiFiStatusBar.stopTimerClosure)
            setProperty(wiFiStatusBar, "restartTimerClosure", true, wiFiStatusBar.stopTimerClosure)
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

        }

        onNetworkSelected:function(selectedSsid, selectedRequiresPassword, selectedNetworkKnown){

            //console.log("Rete selezionata:", selectedSsid);
            // Gestione della selezione della rete
            selectedNetwork = {
                "ssid": selectedSsid,
                "lock": selectedRequiresPassword,
                "isSaved": selectedNetworkKnown
            };
            //networkName = selectedNetwork.ssid;
            wifiScanDialog.visible = false;  // Chiudi il dialogo dopo la selezione
            let result = NetUtils.checkPasswordLock(selectedNetwork);
            if (result.shouldConnect) {
                if (WiFiManager) {
                    WiFiManager.connectToNetwork(selectedNetwork.ssid, "", false);
                }
            } else {
                passwordDialog.visible = true;
                passwordDialog.ssid = selectedNetwork.ssid;
            }

        }
        
        onVisibleChanged: {
            if (visible){
                wiFiStatusBar.stopTimerClosure = true
            }else{
                wiFiStatusBar.stopTimerClosure = false
                setProperty(wiFiStatusBar, "restartTimerClosure", true, wiFiStatusBar.stopTimerClosure)
                //console.log("Scan chiuso, restart timer");
            }
        }


    }


    MyDialogs.PasswordDialog {
        id: passwordDialog
        keyboard_active: keyboard.active
        keyboard_height: keyboard.height
        ssid: selectedNetwork.ssid
        z: 10

        onConfirmClicked:function(password, savePassword){

            if (WiFiManager) {
                WiFiManager.connectToNetwork(selectedNetwork.ssid, password, savePassword);
            }
        }
        onVisibleChanged:{
            if (visible)
                wifiScanDialog.visible = false;
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

    MyComponents.TcpStatusIcon{
        id: tcpStatusIcon
        anchors {

            left: wiFiStatusBar.right
            top: parent.top
            leftMargin: 10
            topMargin: 10

        }
        tcpStatusColor: "white"
        borderColor: tcpStatusColor
        iconSource: tcpStatusImg
        circleRadius: 20
        visible: true

        onLongPressed: {
            tcpClient.connectToESP32("192.168.4.1", 8080);
        }

    }
    
    // funzioni
    function setProperty(object, propertyName, value, lock) {
        if (lock) {
            console.log("Modifica non consentita: propriet� bloccata!");
            return;
        }
        if (object && propertyName in object) {
                object[propertyName] = value;  // Modifica dinamica della propriet�
                //console.log(propertyName, "modificata a:", value, lock);
            } else {
                console.log("Errore: la propriet�", propertyName, "non esiste nell'oggetto.");
            }
    }
}


