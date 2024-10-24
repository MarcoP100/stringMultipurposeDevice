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


        // Propriet� centralizzata per gestire le posizioni delle pagine
        QtObject {
            id: pageManager
            property real mainPageX: 0
            property real canLoggerPageX: 800  // Posizione iniziale fuori schermo
        }

        Loader {
            id: mainPageLoader
            source: "MainPage.qml"  // Carica il file MainPage.qml
            //anchors.fill: parent

            x: pageManager.mainPageX  // Collega alla propriet� globale
            onLoaded: {
                mainPageLoader.item.pageManager = pageManager  // Passa il riferimento a pageManager
                console.log("MainPage caricata - mainPageX:", pageManager.mainPageX)  // Debug

            }
        }

        /*StackView {
            id: stackView
            anchors.fill: parent

            initialItem: Qt.createComponent("MainPage.qml").createObject(stackView, { "stackView": stackView })

            Component.onCompleted: {
                var mainPageItem = stackView.currentItem  // Riferimento a mainPage

                // Carica CanLoggerPage e passa il riferimento a mainPage
                var canLoggerComponent = Qt.createComponent("CanLoggerPage.qml");
                var canLoggerPage = canLoggerComponent.createObject(stackView, {
                                                                        "stackView": stackView,
                                                                        "mainPage": mainPageItem  // Passa mainPage come previousPage
                                                                    });

                if (canLoggerPage !== null) {
                    stackView.push(canLoggerPage);  // Aggiungi la nuova pagina allo StackView
                } else {
                    console.log("Errore nel caricamento di CanLoggerPage.qml");
                }
            }*/



        Loader {
            id: canLoggerPageLoader
            source: "CanLoggerPage.qml"  // Carica il file MainPage.qml
            //anchors.fill: parent

            x: pageManager.canLoggerPageX  // Collega alla propriet� globale
            onLoaded: {
                canLoggerPageLoader.item.pageManager = pageManager  // Passa il riferimento a pageManager
                console.log("CanLoggerPage caricata - canLoggerPageX:", pageManager.canLoggerPageX)  // Debug

            }

        }




    }

}
