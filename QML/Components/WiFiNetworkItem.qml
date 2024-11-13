import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: wifiNetworkItem
    property int wifiItemHeight: 40

    width: parent.width
    height: wifiItemHeight

    signal networkSelected(string ssid, bool requiresPassword, bool networkKnown)

    // Definisci una nuova property per la gestione dello stato di "requiresPassword"
    property bool requiresPassword: false

    // Definisci anche altre propriet� per rendere il componente pi� riutilizzabile
    property string ssid: ""
    property bool networkKnown: false


    Rectangle {
        id: networkBackground
        width: parent.width
        height: parent.height
        color: wifiSelectedArea.pressed ? "#d0d0d0" : "#f0f0f0"
        border.color: "#cccccc"
        border.width: 1

        Text {
            id: netSsid
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: ssid
        }

        Image {
            id: netLock
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            source: requiresPassword ? "qrc:/wifiLock.svg" : "qrc:/wifiUnLock.svg"
            visible: true
        }

        Image {
            id: netKnown
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: netLock.left
            anchors.rightMargin: 10
            source: networkKnown ? "qrc:/key_on.svg" : "qrc:/key_no.svg"
            visible: requiresPassword
        }

        MouseArea {
            id: wifiSelectedArea
            anchors.fill: parent
            onClicked: {
                networkSelected(ssid, requiresPassword, networkKnown)
            }
        }
    }


}
