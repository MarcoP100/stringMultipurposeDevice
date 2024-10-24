import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: mainPage
    width: 800
    height: 480

    // Aggiungiamo una propriet� per ricevere lo StackView dal file principale
    //property var stackView
    // Propriet� per ricevere il riferimento a pageManager dal file principale
       property var pageManager

    GridLayout {
        columns: 2
        anchors.centerIn: parent
        rowSpacing: 10  // Spaziatura verticale tra le righe
        columnSpacing: 10  // Spaziatura orizzontale tra le colonne

        Button {
            text: "Can Logger"
            Layout.preferredWidth: 380  // Dimensione fissa in larghezza
            Layout.preferredHeight: 220  // Dimensione fissa in altezza
            font.pixelSize: 24  // Dimensione fissa del testo
            //anchors.margins: 10
            onClicked: {
                // Naviga a Pagina 1
                //canDrag = false  // Blocca il drag se clicchi qui
                console.log("Vai a Pagina 1")
                // Sposta manualmente entrambe le pagine
                // Modifica le propriet� di posizione globali
                            pageManager.mainPageX = -800
                            pageManager.canLoggerPageX = 0


                //stackView.push("CanLoggerPage.qml")  // Naviga alla sottopagina
                //if (!canLoggerPage) {
                    //var canLoggerComponent = Qt.createComponent("CanLoggerPage.qml");
                    //canLoggerPage = canLoggerComponent.createObject(parent, { "mainPage": mainPage })
                    //canLoggerPage.x = parent.width  // Posiziona fuori dallo schermo a destra
                    //}

                    // Avvia la transizione spostando la mainPage e portando in scena CanLoggerPage
                    //mainPage.x = -mainPage.width  // Sposta mainPage fuori dallo schermo a sinistra
                    //canLoggerPage.x = 0  // Porta CanLoggerPage in scena

                    /*if (canLoggerPage !== null) {
                    stackView.push(canLoggerPage);  // Naviga verso CanLoggerPage
                } else {
                    console.log("Errore nel caricamento di CanLoggerPage.qml");
                }*/
                //}
            }
            }

            Button {
                text: "Dinamometro su can"
                Layout.preferredWidth: 380  // Dimensione fissa in larghezza
                Layout.preferredHeight: 220  // Dimensione fissa in altezza
                font.pixelSize: 24  // Dimensione fissa del testo
                onClicked: {
                    // Naviga a Pagina 2
                    console.log("Vai a Pagina 2")
                    //stackView.push("CanDynamometerPage.qml")  // Naviga alla sottopagina
                }
            }

        Button {
            text: "In costruzione 1"
            Layout.preferredWidth: 380  // Dimensione fissa in larghezza
            Layout.preferredHeight: 220  // Dimensione fissa in altezza
            font.pixelSize: 24  // Dimensione fissa del testo
            onClicked: {
                // Naviga a Pagina 3
                console.log("Vai a Pagina 3")
            }
        }

        Button {
            text: "In costruzione 2"
            Layout.preferredWidth: 380  // Dimensione fissa in larghezza
            Layout.preferredHeight: 220  // Dimensione fissa in altezza
            font.pixelSize: 24  // Dimensione fissa del testo
            onClicked: {
                // Naviga a Pagina 4
                console.log("Vai a Pagina 4")
            }
        }
    }

}
