import QtQuick 2.15

Rectangle {

    id: shutdownRect

    signal singleClicked()

    // Propriet� configurabili dall'esterno
    property alias iconSource: shutdownIcon.source
    property int width_height: 50
    property color innerColor: 'white'
    property color borderColor: 'black'


    width: width_height
    height: width_height
    radius: 5
    color: innerColor
    border.color: borderColor   // Colore del bordo
    border.width: 3  // Spessore del bordo

    Image {
        id: shutdownIcon
        anchors.fill: parent
        anchors.margins: 5
        fillMode: Image.PreserveAspectFit  // Adatta l'icona senza distorsioni

    }

    MouseArea {
        id: shutdownMouseArea
        anchors.fill: parent
        onPressed: shutdownRect.opacity = 0.7  // Effetto di pressione
        onReleased: shutdownRect.opacity = 1.0
        onClicked: shutdownRect.singleClicked();  // Emissione del segnale `clicked`

    }
}
