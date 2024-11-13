// SwipeButtonArea.qml
import QtQuick 2.15

MouseArea {
    // Riferimento al pulsante
    property var target
    property Flickable flickRef
    property real startMovX: 0
    property real startMouseX: 0
    property real startY: 0
    property bool moved: false

    anchors.fill: parent
    propagateComposedEvents: true  // Lascia passare l'evento anche agli elementi sottostanti

    // Inizia a tracciare il tocco/click
    onPressed: function(mouse) {
        startMovX = flickRef.contentX
        startMouseX = mouse.x
        //startY = mouse.y
        moved = false  // Resetta lo stato del movimento
        if (target && target.hasOwnProperty("down")) {
            target.down = true  // Simula lo stato premuto per i Button
            //console.log("Pulsante premuto - down = true")
        }
        //console.log("Inizio swipe - currentPage:", currentPage, "previousPage:", previousPage)  // Log di debug

    }

    // Durante il movimento del cursore o dito
    onPositionChanged: function(mouse) {
        var deltaMouseX = mouse.x - startMouseX
        var deltaMovX = flickRef.contentX - startMovX
        //var deltaY = mouse.y - startY
        if (Math.abs(deltaMovX) > 10) {  // Se il movimento supera i 50px
            moved = true  // Flag che indica che c'� stato un movimento
            if (target && target.hasOwnProperty("down")) {
                target.down = false  // Rilascia il pulsante se esce dall'area
                //console.log("Movimento significativo, pulsante rilasciato - down = false")
            }
        }
        if (flickRef.contentX <= 800) {
            flickRef.contentX -= deltaMouseX  // Muove il contenuto del Flickable
        }

    }

    // Quando il tocco/click viene rilasciato
    onReleased: function(mouse) {

        if (target && target.hasOwnProperty("down")) {
            target.down = false  // Rilascia il pulsante per i Button
            //console.log("Pulsante rilasciato - down = false")
        }
        var deltaMovX = flickRef.contentX - startMovX
        if (flickRef) {

            // Se lo swipe � stato significativo, decidiamo dove spostare il contenuto
            if (deltaMovX < -200) {
                flickRef.contentX = 0  // Torna a pagina 1
            } else {
                flickRef.contentX = 800  // Resta su pagina 2
            }
        }

        //console.log("Rilasciato - currentPage:", currentPage, "previousPage:", previousPage)  // Log di debug

    }

    // Non blocca l'interazione con il pulsante se non c'� stato un movimento significativo
    onClicked: {
        if (!moved) {
            // Se � checkable, cambia lo stato checked (solo per i Button)
            if (target && target.checkable === true) {
                target.checked = !target.checked
            }
            // Trigger di onClicked se esiste
            if (target && target.clicked !== undefined) {
                target.clicked()

            }


        }
    }
}
