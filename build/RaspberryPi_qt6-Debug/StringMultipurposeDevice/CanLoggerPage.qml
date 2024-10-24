import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: canLoggerPage
    width: 800
    height: 480

    // Aggiungiamo una propriet� per ricevere lo StackView dal file principale
    //property var stackView
    // Aggiungi una propriet� per ricevere il riferimento a mainPage
    //property var mainPage
    //property real mainPageX
    //property real canLoggerPageX // Posizione iniziale di CanLoggerPage fuori dallo schermo
    property var pageManager

    // SwipeArea per rilevare lo swipe verso destra
    MouseArea {

        anchors.fill: parent
        property real startX: 0
        property bool moved: false  // Tiene traccia se c'� stato un movimento significativo
        //z: 1

        //propagateComposedEvents: true  // Lascia che i pulsanti sotto gestiscano i click


        // Inizia a tracciare il tocco/click
        onPressed: function(mouse) {
            startX = mouse.x
            moved = false  // Resetta lo stato del movimento
        }

        // Durante il movimento del cursore o dito
        onPositionChanged: function(mouse) {
            var deltaX = mouse.x - startX
            if (Math.abs(deltaX) > 50) {  // Se il movimento supera i 10px
                moved = true  // Flag che indica che c'� stato un movimento
            }
        }

        // Quando il tocco/click viene rilasciato
        onReleased: function(mouse) {
            var deltaX = mouse.x - startX
            if (moved && deltaX > 100) {  // Se c'� stato un movimento e lo swipe è verso destra
                pageManager.mainPageX = 0
                pageManager.canLoggerPageX = 800

            }
        }

        /*// Non blocca l'interazione con i pulsanti o il TextArea se non c'è stato un movimento significativo
        onClicked: function(mouse) {
            if (!moved) {
                mouse.accepted = false  // Rifiuta l'evento in modo che passi ai componenti sottostanti
                // Lascia passare l'evento ai componenti interattivi sottostanti
                //console.log("Click normale, propagato ai pulsanti sottostanti.")
            }
        }*/
    }

    // Sezione superiore per selezione CAN
    RowLayout {
        anchors.fill: parent


        // Colonna sinistra: pulsanti di controllo
        ColumnLayout {
            Layout.margins: 20
            width: 250
            spacing: 20

            // Sezione inferiore per salvare su USB
            RowLayout {
                spacing: 20

                // Pulsante CAN0
                Button {
                    id: can0Button
                    text: "CAN0"
                    checkable: true
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 75
                    SwipeButtonArea {
                        target: can0Button  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                }

                // Pulsante CAN1
                Button {
                    id: can1Button
                    text: "CAN1"
                    checkable: true
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 75
                    SwipeButtonArea {
                        target: can1Button  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                }
            }

            // Sezione centrale per controllo registrazione
            ColumnLayout {
                Layout.alignment: Qt.AlignHCenter  // Centro orizzontalmente i pulsanti
                spacing: 20

                // Pulsante Start Rec
                Button {
                    id: startButton
                    text: "Start Rec"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 100


                    // SwipeButtonArea riutilizzabile
                    SwipeButtonArea {
                        target: startButton  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                    // Non blocca l'interazione con i pulsanti o il TextArea se non c'è stato un movimento significativo
                    onClicked: {
                        console.log("Registrazione avviata")
                    }



                }

                // Pulsante Stop Rec
                Button {
                    id: stopButton
                    text: "Stop Rec"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 100
                    // SwipeButtonArea riutilizzabile
                    SwipeButtonArea {
                        target: stopButton  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                    onClicked: {
                        // Ferma la registrazione CAN
                        console.log("Registrazione fermata")
                    }
                }

            }

            // Sezione inferiore per salvare su USB
            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignBottom  // Allinea i pulsanti in basso

                Button {
                    id: saveButton
                    text: "Salva"
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 100
                    // SwipeButtonArea riutilizzabile
                    SwipeButtonArea {
                        target: saveButton  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                    onClicked: {
                        // Salva il file su USB
                        console.log("Salvataggio su USB")
                    }
                }

                Button {
                    id: deleteButton
                    text: "Elimina"
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 100
                    SwipeButtonArea {
                        target: deleteButton  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                    onClicked: {
                        // Salva il file su USB
                        console.log("Eliminazione registrazione")
                    }
                }
            }
        }


        // Colonna destra: candump visu
        ColumnLayout {
            Layout.margins: 20
            width: 550
            spacing: 10

            // Pulsante CAN0
            Button {
                id: candumpButton
                text: "Visu candump"
                checkable: true
                Layout.preferredWidth: 500
                Layout.preferredHeight: 30
                SwipeButtonArea {
                    target: candumpButton  // Passiamo il riferimento del pulsante
                    currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                    previousPage: pageManager.mainPageX
                }
            }


            // Sezione inferiore: TextArea all'interno di un Rectangle con bordo
            Rectangle {
                Layout.fillHeight: true  // Riempie tutto lo spazio verticale rimanente
                Layout.preferredWidth: 500
                color: "transparent"  // Sfondo trasparente per vedere solo il bordo
                border.color: "black"  // Colore del bordo
                border.width: 0.5  // Larghezza del bordo
                radius: 5  // Rende gli angoli leggermente arrotondati (facoltativo)

                // Incapsula il TextArea nel Rectangle per avere il bordo
                TextArea {
                    id: candumpArea
                    anchors.fill: parent
                    readOnly: true
                    wrapMode: Text.NoWrap
                    text: "Candump Output\n"
                    padding: 10  // Margine interno per dare un po' di spazio al testo
                    SwipeButtonArea {
                        target: candumpArea  // Passiamo il riferimento del pulsante
                        currentPage: pageManager.canLoggerPageX  // Imposta la pagina attuale
                        previousPage: pageManager.mainPageX
                    }
                }
            }
        }
    }
}
