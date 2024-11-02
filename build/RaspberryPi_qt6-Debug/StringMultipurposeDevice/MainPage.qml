import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: mainPage
    width: 800
    height: 480

    property Flickable flickableRef
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
                console.log("Vai a Pagina 1")
                // Sposta il Flickable alla pagina 2
                if (pageManager){
                    pageManager.canLoggerPageX = 800
                    pageManager.dynaOnCanPageX = 1600
                }
                if (flickableRef) {
                    flickableRef.contentX = 800
                }
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
                if (pageManager){
                    pageManager.canLoggerPageX = 1600
                    pageManager.dynaOnCanPageX = 800
                }
                if (flickableRef) {
                    flickableRef.contentX = 800
                }
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
