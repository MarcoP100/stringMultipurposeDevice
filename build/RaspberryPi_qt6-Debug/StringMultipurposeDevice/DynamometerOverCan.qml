import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.VirtualKeyboard
import QtQuick.VirtualKeyboard.Settings
//import CustomComponents 1.0

Item {
    id: dynaOnCanPage
    width: 800
    height: 480


    property Flickable flickableRef
    property bool popupOpen: false
    property int maxVisibleItems: 5  // Numero massimo di righe visibili senza scorrimento
    property bool isPasswordVisible: false

    // Dizionario per memorizzare le password delle reti Wi-Fi in base al SSID
    property var passwordDictionary: ({})
    property string selectedNetwork: "" // Rete selezionata


    SwipeButtonArea {
        target: dynaOnCanPage  // Passiamo il riferimento del pulsante
        flickRef: flickableRef  // Passiamo il riferimento del Flickable
    }

    // Indica che sono disponibili reti
    Connections {
        target: wifiManager
        function onAvailableNetworksChanged(networks) {

            console.log("Reti WiFi trovate:", networks);
            updateNetworkList(networks);


        }
    }
    ListModel {
        id: listModel
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
        listModel.clear();
        //busyIndicator.running = true;  // Avvia l'indicatore di caricamento
        wifiManager.getAvailableNetworks();  // Avvia la ricerca delle reti
        //busyIndicator.running = false;

    }

    /*Component.onCompleted: {
            // Esegui una ricerca delle reti all'apertura della pagina
            wifiManager.getAvailableNetworks();
        }*/

    function updateNetworkList(networks) {
        //console.log("Reti trovate in QML:", networks);
        // Mappa per evitare duplicati e per mantenere traccia degli elementi attuali

        var existingNetworks = {};
        for (var j = 0; j < listModel.count; j++) {
            existingNetworks[listModel.get(j).text] = true;
        }

        // Aggiungi solo le reti che non sono gi� presenti nel modello
        for (var i = 0; i < networks.length; i++) {
            if (!existingNetworks[networks[i]]) {
                listModel.append({"text": networks[i]});
            }
        }

        // Rimuovi le reti che non sono pi� presenti
        for (var k = listModel.count - 1; k >= 0; k--) {
            if (networks.indexOf(listModel.get(k).text) === -1) {
                listModel.remove(k);
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

    // Placeholder per determinare se una rete ha bisogno di una password
    function needsPassword(ssid) {
        // Logica per determinare se la rete richiede una password (es. reti salvate)
        return true;  // Supponiamo per ora che ogni rete ne abbia bisogno
    }

    function isNetworkSelected() {
        if (selectedNetwork == "")
            return false;  // Supponiamo per ora che ogni rete ne abbia bisogno
        else
            return true;
    }

    function addNetworkPassword(ssid) {
        // Se il dizionario non contiene la rete, aggiungila
        if (!(ssid in passwordDictionary)) {
            passwordDictionary[ssid] = "";
        }
    }

    /*// Funzione di connessione alla rete WiFi
        function connectToNetwork(ssid) {
            if (ssid in passwordDictionary && passwordDictionary[ssid] !== "") {
                console.log("Password trovata per:", ssid);
                // Tenta di connetterti con la password associata
                wifiManager.connectToNetwork(ssid, passwordDictionary[ssid]);
            } else {
                console.log("Password necessaria per:", ssid);
                passwordDialog.visible = true;
            }
        }*/

    // Salva una nuova password quando viene confermata
    function savePassword(ssid, password) {
        passwordDictionary[ssid] = password;
        console.log("Password salvata per:", ssid);
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
                    model: listModel
                    focus: true  // Assicurati che la ListView mantenga il focus
                    interactive: true  // Mantieni la ListView interattiva
                    visible: !wifiManager.busy
                    delegate: ItemDelegate {
                        width: listView.width
                        height: 40  // Altezza di ogni elemento per calcolare la dimensione complessiva

                        text: model.text
                        onClicked: {
                            networkButton.text = model.text;  // Aggiorna il pulsante con la rete selezionata
                            selectedNetwork = model.text;
                            addNetworkPassword(selectedNetwork);
                            networkDialog.visible = false;  // Chiudi il dialogo dopo la selezione
                        }
                    }
                }

            }

            /*// Pulsante per aggiornare la lista delle reti WiFi
            Button {
                id: refreshButton
                text: "Aggiorna"
                width: parent.width * 0.5
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    startNetworkScan();
                }
            }*/



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



    Rectangle {

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

        Button {
            id: networkButton
            width: 200
            height: 30
            text: "Seleziona una rete"
            anchors.top: wifiText.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            // Quando si preme il pulsante, apre il popup
            onClicked: {
                console.log("Pulsante cliccato, apertura overlay o dialog");
                networkDialog.visible = true;

                startNetworkScan();

            }
        }






        Timer {
            id: updateTimer
            interval: 4000  // Aggiorna ogni 5000 ms (5 secondi)
            repeat: true
            running: false
            onTriggered: {
                wifiManager.getAvailableNetworks();
            }
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
                // Controllo se la rete richiede la password
                if (needsPassword(selectedNetwork)) {
                    if (passwordDictionary[selectedNetwork] && passwordDictionary[selectedNetwork] !== "") {
                        console.log("Password trovata")
                        // Tenta di connettersi utilizzando la password inserita
                        wifiManager.connectToNetwork(selectedNetwork, passwordField.text);
                    } else {
                        // Se non c'� password, mostra il popup per richiederla
                        console.log("Password necessaria")
                        passwordDialog.visible = true;
                    }

                } else {
                    // Chiamata diretta a C++ per connettersi senza password
                    wifiManager.connectToNetwork(selectedNetwork, "")
                    updateStatus("connecting");
                }
            }


        }

        // Indicatore di stato (LED)
        Rectangle {
            id: statusLed
            width: 20
            height: 20
            radius: 10
            color: "red"
            anchors.top: networkButton.bottom
            anchors.left: wifiConnection.right
            anchors.topMargin: 10
            anchors.leftMargin: 20
            //anchors.verticalCenter: wifiNetworks.verticalCenter
        }
    }

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
        // Ancoraggio dinamico al centro, ma si sposta verso l'alto quando la tastiera � visibile
        /*anchors {
            verticalCenter: keyboardVisible ? undefined : parent.verticalCenter
            bottom: keyboardVisible ? keyboard.top : undefined
            bottomMargin: !keyboardVisible ? 10 : 0  // Margine opzionale

        }*/

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

        /*MouseArea {
            id: textFieldMouseArea
            anchors.fill: parent
            onClicked: {
                if (passwordField.active) {
                    passwordField.forceActiveFocus();
                }
            }
        }*/

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
                onClicked: {
                    savePassword(selectedNetwork, passwordField.text);

                    passwordField.focus = false;
                    passwordDialog.visible = false;
                    isPasswordVisible = false;
                    passwordField.text = "";

                    console.log("Password salvata per SSID:", selectedNetwork, "Password:", passwordDictionary[selectedNetwork])

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


