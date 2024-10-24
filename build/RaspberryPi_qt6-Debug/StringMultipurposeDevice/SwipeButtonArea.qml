// SwipeButtonArea.qml
import QtQuick 2.15

MouseArea {
    // Riferimento al pulsante
    property var target
    //property var stackView
    property real currentPage  // Riferimento alla pagina attuale
    property real previousPage

    property real startX: 0
    property real startY: 0
    property bool moved: false
    property bool insideArea: true  // Tiene traccia se il puntatore � ancora dentro il pulsante

    anchors.fill: parent

    // Inizia a tracciare il tocco/click
    onPressed: function(mouse) {
        startX = mouse.x
        startY = mouse.y
        insideArea = true
        moved = false  // Resetta lo stato del movimento
        if (target && target.hasOwnProperty("down")) {
            target.down = true  // Simula lo stato premuto per i Button
        }
        //console.log("Inizio swipe - currentPage:", currentPage, "previousPage:", previousPage)  // Log di debug

    }

    // Durante il movimento del cursore o dito
    onPositionChanged: function(mouse) {
        var deltaX = mouse.x - startX
        var deltaY = mouse.y - startY
        if (Math.abs(deltaX) > 50) {  // Se il movimento supera i 50px
            moved = true  // Flag che indica che c'� stato un movimento
            if (target && target.hasOwnProperty("down")) {
                target.down = false  // Rilascia il pulsante se esce dall'area
            }
        }
        // Verifica se il puntatore � ancora all'interno dell'area del pulsante
        if (mouse.x < 0 || mouse.x > width || mouse.y < 0 || mouse.y > height) {
            insideArea = false
            if (target && target.hasOwnProperty("down")) {
                target.down = false  // Rilascia il pulsante se esce dall'area
            }
        }
        // Spostamento della pagina
        if (deltaX > 0) {
            // Sposta la pagina attuale e la pagina precedente
            pageManager.canLoggerPageX = deltaX
            pageManager.mainPageX = deltaX - 800
            //console.log("Spostamento - currentPage:", currentPage, "previousPage:", previousPage)  // Log di debug

        }
    }

    // Quando il tocco/click viene rilasciato
    onReleased: function(mouse) {
        if (target && target.hasOwnProperty("down")) {
            target.down = false  // Rilascia il pulsante per i Button
        }
        var deltaX = mouse.x - startX
        if (moved && deltaX > 100) {  // Se c'� stato un movimento e lo swipe � verso destra
            pageManager.mainPageX = 0 // Torna alla pagina precedente
            pageManager.canLoggerPageX = 800
        }else {
            // Ripristina la posizione iniziale
            pageManager.canLoggerPageX = 0
            pageManager.mainPageX = -800

        }
        //console.log("Rilasciato - currentPage:", currentPage, "previousPage:", previousPage)  // Log di debug


    }

    // Non blocca l'interazione con il pulsante se non c'� stato un movimento significativo
    onClicked: {
        if (insideArea && !moved) {
            // Se � checkable, cambia lo stato checked (solo per i Button)
            if (target && target.checkable !== undefined) {
                target.checked = !target.checked
            }
            // Trigger di onClicked se esiste
            if (target && target.clicked !== undefined) {
                target.clicked()
            }

        }
    }
}
