import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.VirtualKeyboard
import QtQuick.VirtualKeyboard.Settings
import com.wifiManager 1.0
import "colors.js" as Colors
import "../Components" as MyComponents

Item {
    id: dynaOnCanPage
    width: 800
    height: 480


    property Flickable flickableRef
    property bool popupOpen: false
    property int maxVisibleItems: 5  // Numero massimo di righe visibili senza scorrimento
    property bool isPasswordVisible: false

    // Dizionario per memorizzare le password delle reti Wi-Fi in base al SSID
    //property var passwordDictionary: ({})
    property var selectedNetwork: { "ssid": "", "lock": false, "isSaved": false }
    property bool firstSaved: true

    property bool wifiVisible: false


    MyComponents.SwipeButtonArea {
        target: dynaOnCanPage  // Passiamo il riferimento del pulsante
        flickRef: flickableRef  // Passiamo il riferimento del Flickable
    }

    // Indica che sono disponibili reti
    /*Connections {
        target: wifiManager
        function onAvailableNetworksChanged(networks) {

            for (var i = 0; i < networks.length; i++) {
                console.log("SSID:", networks[i].ssid, "Richiede password:", networks[i].requiresPassword, "Salvata:", networks[i].networkKnown);
            }
            updateNetworkList(networks);

            if (firstSaved) {
                console.log("Reti salvate lette, avvio la verifica di connessione...");
                if (wifiManager.isConnected){
                    console.log("Connessione attiva.");
                    wifiStatusCircle.border.color = Colors.GREEN_COLOR
                    wifiStatusIcon.source = "qrc:/wifi_on.svg"

                    var connectedSSID = wifiManager.getConnectedSSID();
                    console.log("connectedSSID:", connectedSSID);
                    for (i = 0; i < networks.length; i++) {
                        if (connectedSSID === networks[i].ssid) {
                            selectedNetwork = {
                                "ssid": networks[i].ssid,
                                "lock": networks[i].requiresPassword,
                                "isSaved": networks[i].networkKnown
                            };
                            networkName.text = selectedNetwork.ssid;
                        }
                    }

                }

            }

            // Imposta la variabile `firstTime` a false per evitare che venga ripetuto
            firstSaved = false;
        }

        function onIsConnectedChanged() {
            if (wifiManager.isConnected) {
                wifiStatusCircle.border.color = Colors.GREEN_COLOR
                wifiStatusIcon.source = "qrc:/wifi_on.svg"
                wifiIcon.state = ""

                var connectedSSID = wifiManager.getConnectedSSID();
                console.log("Viariazione connessione");
                for (i = 0; i < networks.length; i++) {
                    if (connectedSSID === networks[i].ssid) {
                        selectedNetwork = {
                            "ssid": networks[i].ssid,
                            "lock": networks[i].requiresPassword,
                            "isSaved": networks[i].networkKnown
                        };
                        networkName.text = selectedNetwork.ssid;
                    }
                }

            } else {
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
            }
        }


        function onConnectionStatusChanged(status) {


            console.log("Status:", status)
            switch (status) {
            case WiFiManager.ErrorProcessStart:
                console.log("Errore nell'avvio del processo.");
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Timeout:
                console.log("La connessione ha superato il timeout.");
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Connecting:
                console.log("Connessione in corso...");
                wifiStatusCircle.border.color = Colors.YELLOW_COLOR
                wifiStatusIcon.source = "qrc:/wifi_connecting.svg"
                break;
            case WiFiManager.Connected:
                console.log("Connessione avvenuta con successo.");
                wifiStatusCircle.border.color = Colors.GREEN_COLOR
                wifiStatusIcon.source = "qrc:/wifi_on.svg"
                wifiIcon.state = ""
                break;
            case WiFiManager.WrongPassword:
                console.log("Password errata. Riprovare.");
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
                passwordDialog.visible = true;
                break;
            case WiFiManager.ConnectionFailed:
                console.log("Connessione fallita.");
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.Disconnected:
                console.log("Disconnesso.");
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
                break;
            case WiFiManager.ErrorDisconnection:
                console.log("Disconnessione fallita.");
                if (wifiManager.isConnected){
                    wifiStatusCircle.border.color = Colors.GREEN_COLOR
                    wifiStatusIcon.source = "qrc:/wifi_on.svg"

                }else{
                    wifiStatusCircle.border.color = Colors.RED_COLOR
                    wifiStatusIcon.source = "qrc:/wifi_off.svg"
                }

                break;



            default:
                console.log("Stato della connessione sconosciuto.");
                wifiStatusCircle.border.color = Colors.RED_COLOR
                wifiStatusIcon.source = "qrc:/wifi_off.svg"
            }
        }
    }

*/

    ListModel {
        id: wifiListModel
    }

    /*WiFiManager {
        id: wifiManager
        onAvailableNetworksChanged: {
            /*if (!wifiManager.busy) {
                if (networks.length > 0) {
                    updateNetworkList(networks);
                } else {
                    console.log("Nessuna rete trovata o dati non validi.");
                }
            }
            console.log("Scansione finita");
        }
    }*/

    // Funzione per avviare la scansione delle reti e mostrare l'indicatore di caricamento
    function startNetworkScan() {
        wifiListModel.clear();
        //busyIndicator.running = true;  // Avvia l'indicatore di caricamento
        wifiManager.scanNetworks();  // Avvia la ricerca delle reti
        //busyIndicator.running = false;

    }

    Component.onCompleted: {
        // Esegui una ricerca delle reti all'apertura della pagina
        wifiManager.scanNetworks();
    }

    function updateNetworkList(networks) {
        // Mappa per evitare duplicati e per mantenere traccia degli elementi attuali

        var existingNetworks = {};
        for (var j = 0; j < wifiListModel.count; j++) {
            existingNetworks[wifiListModel.get(j).ssid] = true;
        }

        // Aggiungi solo le reti che non sono gi� presenti nel modello
        for (var i = 0; i < networks.length; i++) {
            if (!existingNetworks[networks[i].ssid]) {
                wifiListModel.append({
                                         ssid: networks[i].ssid,
                                         requiresPassword: networks[i].requiresPassword,
                                         networkKnown: networks[i].networkKnown
                                     });
            }
        }

        // Rimuovi le reti che non sono pi� presenti
        for (var k = wifiListModel.count - 1; k >= 0; k--) {
            var ssid = wifiListModel.get(k).ssid;
            var found = false;
            for (var m = 0; m < networks.length; m++) {
                if (networks[m].ssid === ssid) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                wifiListModel.remove(k);
            }
        }
    }

    function updateStatus(status) {
        if (status === "connecting") {
            statusText.text = "Stato: Connessione in corso..."
            statusLed.color = "yellow"
        } else if (status === "connected") {
            statusText.text = "Stato: Connesso"
            statusLed.color = "green"
        } else {
            statusText.text = "Stato: Non connesso"
            statusLed.color = "red"
        }
    }



    function isNetworkSelected() {
        if (selectedNetwork.ssid === "")
            return false;  // Supponiamo per ora che ogni rete ne abbia bisogno
        else
            return true;
    }

    /* function addNetworkPassword(ssid) {
        // Se il dizionario non contiene la rete, aggiungila
        if (!(ssid in passwordDictionary)) {
            passwordDictionary[ssid] = "";
        }
    }


    // Salva una nuova password quando viene confermata
    function savePassword(ssid, password) {
        passwordDictionary[ssid] = password;
        console.log("Password salvata per:", ssid);
    }*/

    function needsPassword(){
        // Controllo se la rete richiede la password
        if (selectedNetwork.lock) {
            if (selectedNetwork.isSaved) {
                console.log("Password conosciuta")
                // Tenta di connettersi utilizzando la password inserita
                wifiManager.connectToNetwork(selectedNetwork.ssid, "");
            } else {
                // Se non c'� password, mostra il popup per richiederla
                console.log("Password necessaria")
                passwordDialog.visible = true;
            }

        } else {
            // Chiamata diretta a C++ per connettersi senza password
            wifiManager.connectToNetwork(selectedNetwork.ssid, "")
            updateStatus("connecting");
        }
    }

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

            Flickable {

                id: flickableArea
                width: parent.width
                height: Math.min(listView.contentHeight, maxVisibleItems * 40) // Altezza dinamica o massimo consentito
                contentHeight: listView.contentHeight
                clip: true  // Limita il contenuto visibile all'altezza dell'elemento

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
                                    selectedNetwork = {
                                        "ssid": model.ssid,
                                        "lock": model.requiresPassword,
                                        "isSaved": model.networkKnown
                                    };
                                    networkName.text = selectedNetwork.ssid;
                                    //addNetworkPassword(selectedNetwork.ssid);
                                    networkDialog.visible = false;  // Chiudi il dialogo dopo la selezione
                                    needsPassword();
                                }
                            }
                        }

                    }

                }
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

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"
        opacity: 0.7  // Regola l'opacit� per ottenere l'effetto di oscuramento
        visible: dialogBackgroundArea.visible  // Rendi visibile solo quando serve
        z: 4
    }

    MouseArea {
        id: dialogBackgroundArea
        anchors.fill: parent // Copre l'intero schermo
        visible: networkDialog.visible || passwordDialog.visible || keyboard.visible// Visibile solo quando il dialogo � aperto
        onClicked: {
            if (networkDialog.visible){
                networkDialog.visible = false; // Chiudi il dialogo cliccando fuori dal riquadro
                wifiManager.stopNetworkScan();
            } else if (keyboard.visible){
                console.log("Chiusura tastiera")
                passwordField.focus = false;
            } else if (passwordDialog.visible){
                passwordField.focus = false;
                passwordDialog.visible = false;
                passwordField.text = ""; // Resetta la password temporanea se non si conferma
                isPasswordVisible = false;
                console.log("Nessuna modifica alla password")

            }
        }
        z: 5 // Posiziona quest'area sotto il dialogo, ma sopra il contenuto del background



    }

    MouseArea {
        id: wifiLabelMouseArea
        anchors.fill: parent // Copre l'intero schermo
        visible: wifiIcon.state === "expanded"// Visibile solo quando il dialogo � aperto
        onClicked: {
            wifiIcon.state = ""

        }
        z: 4 // Posiziona quest'area sotto il dialogo, ma sopra il contenuto del background
    }


    /*Rectangle {

        width: 400
        height: 200


        // Titolo della sezione
        Text {
            id: wifiText
            text: "Connessione WiFi"
            font.pixelSize: 14
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10

        }




        // Pulsante per connettersi
        Button {
            id: wifiConnection
            text: "Connetti"
            width: parent.width * 0.5
            anchors.top: networkButton.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            visible: isNetworkSelected()
            SwipeButtonArea {
                target: wifiConnection  // Passiamo il riferimento del pulsante
                flickRef: flickableRef  // Passiamo il riferimento del Flickable
            }
            onClicked: {
                needsPassword();
            }


        }
    }*/
    // Indicatore di stato (LED)
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


        //anchors.verticalCenter: wifiNetworks.verticalCenter


        Rectangle {
            id: wifiStatusCircle
            width: 40
            height: 40
            radius: 20
            color: "white"
            border.color: Colors.DARK_GREY_COLOR  // Colore del bordo
            border.width: 1  // Spessore del bordo
            z: 21
            anchors {


                leftMargin: 10
                topMargin: 10

            }

            Image {
                id: wifiStatusIcon
                anchors.centerIn: wifiStatusCircle
                source: "qrc:/wifi_off_bk.svg"
                visible: true

            }

            MouseArea {
                id: wifiMouseArea
                anchors.fill: parent

                onClicked: {

                    console.log("Icona cliccata brevemente");

                    // Mostra o nasconde il pulsante per la rete
                    //networkButton.visible = !networkButton.visible;
                    wifiIcon.state = wifiIcon.state === "" ? "expanded" : ""
                }

                onPressAndHold:{
                    if (wifiManager.isConnected) {
                        // Avvia disconnessione
                        console.log("Disconnessione in corso...");
                        wifiManager.disconnectNetwork();
                    } else {
                        if (selectedNetwork.ssid !== ""){
                            // Avvia connessione
                            console.log("Connessione in corso...");
                            needsPassword();
                        }
                    }
                }


                onReleased: {
                    console.log("Mouse rilasciato");
                    longPressTimer.stop(); // Ferma il timer per evitare falsi trigger
                }

                Timer {
                    id: longPressTimer
                    interval: 3000 // 3 secondi
                    repeat: false
                    onTriggered: wifiMouseArea.onPressAndHold
                }

                onPressed: {
                    longPressTimer.start(); // Avvia il timer per rilevare la pressione prolungata
                }
            }
        }

        Rectangle {
            id: tcpStatusCircle
            width: 40
            height: 40
            radius: 20
            color: "white"
            border.color: Colors.DARK_GREY_COLOR  // Colore del bordo
            border.width: 1  // Spessore del bordo
            z: 21
            anchors {

                left: wifiIcon.right
                leftMargin: 10
                topMargin: 10

            }
            //visible : wifiManager.isConnected && wifiManager.currentSsid === "Dynamometer_EXP32C6"
            Image {
                id: tcpStatusIcon
                anchors.centerIn: tcpStatusCircle
                source: "qrc:/wifi_off_bk.svg"
                visible: true

            }

            /*MouseArea {
                id: tcpMouseArea
                anchors.fill: parent

                onClicked: {

                     if (tcpClient.isConnected()) {
                        // Avvia disconnessione
                        console.log("Disconnessione in corso...");
                        tcpClient.disconnectTcp();
                    } else {

                         // Avvia connessione
                         console.log("Connessione in corso...");
                         tcpClient.connectTcp();

                    }
                }



            }*/
        }
    }

    /*Rectangle {
        id: networkButton
        width: 200
        height: 40
        radius: height / 2  // Arrotonda i lati corti
        color: "lightblue"

        MouseArea {
            id: networkMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("Pulsante cliccato, apertura overlay o dialog");
                networkDialog.visible = true;

                startNetworkScan();

            }
        }

        Text {
            anchors.centerIn: parent
            text: "Seleziona una rete"
            color: "white"
        }
    }*/



    /*Button {
        id: networkButton
        width: 200
        height: 30
        z: 10
        text: "Seleziona una rete"
        anchors.left: wifiStatusCircle.right
        anchors.leftMargin: 10
        visible: false // Inizialmente nascosto
        // Quando si preme il pulsante, apre il popup
        onClicked: {
            console.log("Pulsante cliccato, apertura overlay o dialog");
            networkDialog.visible = true;

            startNetworkScan();

        }
    }*/


    Rectangle {
        id: passwordDialog
        visible: false
        width: 300
        height: 175
        color: "white"
        radius: 2
        //anchors.centerIn: parent
        border.color: "black"
        border.width: 1

        z: 10  // Porta il dialogo in primo piano


        // Propriet� per determinare l'altezza della tastiera
        property bool keyboardVisible: keyboard.active  // Aggiungi una propriet� per tenere traccia dello stato della tastiera

        y: keyboard.active ? (parent.height - keyboard.height - height - 10) : (parent.height - height) / 2

        anchors.horizontalCenter: parent.horizontalCenter


        // Aggiorna la visibilit� quando cambia lo stato della tastiera
        onKeyboardVisibleChanged: {
            console.log(keyboard.height);
            /* console.log("cambio keyboard visible");
            anchors.verticalCenter = keyboardVisible ? undefined : parent.verticalCenter;
            anchors.bottom = keyboardVisible ? keyboard.top : undefined;
            anchors.bottomMargin = keyboardVisible ? 10 : 0;
            console.log(keyboardVisible, keyboard.top, anchors.bottom);*/

        }

        // Animazione per rendere il movimento pi� fluido
        Behavior on anchors.verticalCenter {
            NumberAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }


        Column {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter  // Centra la colonna orizzontalmente rispetto al dialogo
            anchors {
                fill: parent
                margins: 5  // Aggiungi un margine per evitare che il contenuto tocchi i bordi
            }



            Text {
                text: "Inserisci Password"
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 16
            }


            TextField {
                id: passwordField
                width: parent.width * 0.9
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Inserisci password"
                echoMode: isPasswordVisible ? TextInput.Normal : TextInput.Password
                inputMethodHints: Qt.ImhSensitiveData
                onAccepted: {
                    console.log("Enter pressed");
                    focus = false

                }
                // Nascondi la tastiera quando si perde il focus
                /*onEditingFinished: {
                    console.log("Editing terminato, nascondo la tastiera.");

                    focus = false
                }*/


            }

            CheckBox {
                id: showPasswordCheckBox
                text: "Mostra Password"
                focusPolicy: Qt.NoFocus
                font.pixelSize: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                checked: isPasswordVisible
                onClicked: {

                    isPasswordVisible = !isPasswordVisible;

                }
            }


            Button {
                text: "Conferma"
                anchors.horizontalCenter: parent.horizontalCenter
                property string tmpPassword: ""
                onClicked: {
                    tmpPassword = passwordField.text;
                    passwordField.focus = false;
                    passwordDialog.visible = false;
                    isPasswordVisible = false;
                    passwordField.text = "";

                    console.log("Password salvata per SSID:", selectedNetwork.ssid, "Password:", tmpPassword)

                    wifiManager.connectToNetwork(selectedNetwork.ssid, tmpPassword);

                }
            }



        }
    }

    // Tastiera virtuale
    InputPanel {
        id: keyboard
        visible: active
        z: 10
        width: parent.width
        anchors.bottom: parent.bottom
        //anchors.fill: parent


    }


}


