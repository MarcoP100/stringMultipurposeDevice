import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Components" as MyComponents

Item {
    id: canLoggerPage
    width: 800
    height: 480

    property Flickable flickableRef
    
    MyComponents.SwipeButtonArea {
        target: canLoggerPage  // Passiamo il riferimento del pulsante
        flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                    MyComponents.SwipeButtonArea {
                        target: can0Button  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
                    }
                }

                // Pulsante CAN1
                Button {
                    id: can1Button
                    text: "CAN1"
                    checkable: true
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 75
                    MyComponents.SwipeButtonArea {
                        target: can1Button  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                    MyComponents.SwipeButtonArea {
                        target: startButton  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                    MyComponents.SwipeButtonArea {
                        target: stopButton  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                    MyComponents.SwipeButtonArea {
                        target: saveButton  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                    MyComponents.SwipeButtonArea {
                        target: deleteButton  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                MyComponents.SwipeButtonArea {
                    target: candumpButton  // Passiamo il riferimento del pulsante
                    flickRef: flickableRef  // Passiamo il riferimento del Flickable
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
                    MyComponents.SwipeButtonArea {
                        target: candumpArea  // Passiamo il riferimento del pulsante
                        flickRef: flickableRef  // Passiamo il riferimento del Flickable
                    }
                }
            }
        }
    }
}
