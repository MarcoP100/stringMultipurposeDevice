import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: qsTr("StringMultipurposeDevice")

    Rectangle {
        id: mainContainer
        anchors.fill: parent
        color: "lightgrey"

        // Rotazione di 90 gradi
        transform: Rotation {
            origin.x: mainContainer.width / 2
            origin.y: mainContainer.height / 2
            angle: 180
        }

        Flickable {
            id: flickable
            width: 800
            height: 480
            contentWidth: width * 2
            contentHeight: height
            flickableDirection: Flickable.HorizontalFlick
            boundsBehavior: Flickable.StopAtBounds
            interactive: true  // Disattiva il flick inizialmente
            clip: true

            onContentXChanged: {
                // console.log("flickable contentX changed:", contentX)
            }


            Loader {
                id: mainPageLoader
                source: "MainPage.qml"  // Carica il file MainPage.qml
                //anchors.fill: parent
                width: 800
                height: parent.height
                x: 0
                onLoaded: {
                    mainPageLoader.item.flickableRef = flickable  // Passa il riferimento di flickable
                    flickable.interactive = false  // Attiva il flick solo quando si arriva in pagina 2
                }

            }

            Loader {
                id: canLoggerPageLoader
                source: "CanLoggerPage.qml"  // Carica il file MainPage.qml
                //anchors.fill: parent
                width: 800
                height: parent.height
                x: 800  // Posiziona questa pagina a destra della prima
                onLoaded: {
                    canLoggerPageLoader.item.flickableRef = flickable  // Passa il riferimento di flickable
                    flickable.interactive = true  // Attiva il flick solo quando si arriva in pagina 2
                }

            }

        }

    }
}
