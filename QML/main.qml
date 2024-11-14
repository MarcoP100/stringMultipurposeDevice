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

        QtObject {
            id: pageManager
            property real canLoggerPageX: 1600
            property real dynaOnCanPageX: 800
        }

        Flickable {
            id: flickable
            width: 800
            height: 480
            contentWidth: width * 3
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
                source: "Pages/MainPage.qml"  // Carica il file MainPage.qml
                //anchors.fill: parent
                width: 800
                height: parent.height
                x: 0
                onLoaded: {
                    mainPageLoader.item.flickableRef = flickable  // Passa il riferimento di flickable
                    mainPageLoader.item.pageManager = pageManager
                    flickable.interactive = false  // Attiva il flick solo quando si arriva in pagina 2
                }

            }

            Loader {
                id: canLoggerPageLoader
                source: "Pages/CanLoggerPage.qml"  // Carica il file MainPage.qml
                //anchors.fill: parent
                width: 800
                height: parent.height
                x: pageManager.canLoggerPageX  // Posiziona questa pagina a destra della prima
                onLoaded: {
                    canLoggerPageLoader.item.flickableRef = flickable  // Passa il riferimento di flickable
                    flickable.interactive = true  // Attiva il flick solo quando si arriva in pagina 2
                }

            }


            Loader {
                id: dynaOnCanPageLoader
                source: "Pages/DynamometerOverCan.qml"  // Carica il file MainPage.qml
                //anchors.fill: parent
                width: 800
                height: parent.height
                x: pageManager.dynaOnCanPageX  // Posiziona questa pagina a destra della prima
                onLoaded: {
                    dynaOnCanPageLoader.item.flickableRef = flickable  // Passa il riferimento di flickable
                    flickable.interactive = true  // Attiva il flick solo quando si arriva in pagina 2
                }

            }

        }

    }
}
