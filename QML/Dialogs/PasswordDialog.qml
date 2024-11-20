import QtQuick 2.15
import QtQuick.Controls 2.15

Item {

    id: passwordDialog

    property bool keyboard_active: false
    property int keyboard_height: 0

    property bool isPasswordVisible: false

    signal confirmClicked(string password)

    visible: false
    width: parent.width
    height: parent.height

    Rectangle {
        id: passwordRct

        width: 300
        height: 175
        color: "white"
        radius: 5
        border.color: "black"
        border.width: 1

        z: passwordDialog.z
        y: keyboard_active ? (parent.height - keyboard_height - height - 10) : (parent.height - height) / 2
        anchors.horizontalCenter: parent.horizontalCenter


        // Animazione per rendere il movimento pi� fluido
        /*Behavior on y {
        NumberAnimation {
            duration: 5000
            easing.type: Easing.InOutQuad
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
                placeholderText: "Inserisci password"
                echoMode: isPasswordVisible ? TextInput.Normal : TextInput.Password
                inputMethodHints: Qt.ImhSensitiveData
                anchors.horizontalCenter: parent.horizontalCenter
                onAccepted: {
                    console.log("Enter pressed");
                    focus = false

                }

            }

            CheckBox {
                id: showPasswordCheckBox
                text: "Mostra Password"
                focusPolicy: Qt.NoFocus
                font.pixelSize: 16
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                checked: isPasswordVisible
                onClicked: isPasswordVisible = !isPasswordVisible;
            }


            Button {
                text: "Conferma"
                focusPolicy: Qt.NoFocus
                anchors.horizontalCenter: parent.horizontalCenter
                onReleased: {
                    passwordDialog.visible = false;

                    console.log("Password salvata")
                    passwordDialog.confirmClicked(passwordField.text);

                    passwordField.focus = false;
                    passwordField.text = "";
                    isPasswordVisible = false;

                }

            }
        }



    }

    Rectangle {
        id: overlay
        anchors.fill: parent
        color: "black"
        opacity: 0.7  // Regola l'opacit� per ottenere l'effetto di oscuramento
        z: (passwordDialog.z - 1)

        MouseArea {
            id: dialogBackgroundArea
            anchors.fill: parent // Copre l'intero schermo
            onClicked: {

                if (keyboard_active){
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



        }
    }
}
