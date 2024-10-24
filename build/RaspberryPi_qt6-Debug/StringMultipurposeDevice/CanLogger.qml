import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    width: 800
    height: 480

    Column {
        anchors.centerIn: parent

        Button {
            text: "Pulsante 1 nella sottopagina"
            Layout.preferredWidth: 300
            Layout.preferredHeight: 100
        }

        Button {
            text: "Pulsante 2 nella sottopagina"
            Layout.preferredWidth: 300
            Layout.preferredHeight: 100
        }

        Button {
            text: "Torna indietro"
            Layout.preferredWidth: 300
            Layout.preferredHeight: 100
            onClicked: {
                stackView.pop()  // Torna alla pagina principale
            }
        }
    }
}

