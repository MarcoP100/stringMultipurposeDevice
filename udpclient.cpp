#include "udpclient.h"
#include <QDebug>

#define UDP_PORT 50000  // Deve corrispondere a quello usato sull'ESP
#define ESP_ADDRESS "192.168.4.1"
#define ESP_SSID_NAME "Dynamometer_ESP32C6"


udpClient::udpClient(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    bool success = udpSocket->bind(QHostAddress::Any, UDP_PORT);
    if (!success) {
        qDebug() << "Errore bind UDP:" << udpSocket->errorString();
    }

    connect(udpSocket, &QUdpSocket::readyRead, this, &udpClient::processPendingDatagrams);


    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(5000);  // 5s di timeout
    timeoutTimer->setSingleShot(true);

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        //qDebug() << "Timeout: Nessun dato ricevuto da 5s!";
        emit udpConnectionTimeout();
    });


    sendTimer = new QTimer(this);
    sendTimer->setInterval(1000);  // 5s di timeout
    sendTimer->setSingleShot(true);

    connect(sendTimer, &QTimer::timeout, this, [=]() {
        sendMessage(QByteArray("\x02A5\x03"));
    });

    qDebug() << "Nuovo socket UDP creato e segnali collegati.";
}

void udpClient::processPendingDatagrams() {
    timeoutTimer->start();

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        emit rawDataReceived(datagram);


    }
}

void udpClient::sendMessage(const QByteArray &message) {
    QHostAddress espAddress(ESP_ADDRESS);  // Indirizzo IP dell'ESP
    quint16 espPort = 12345;                // Porta su cui l'ESP � in ascolto

    qint64 sent = udpSocket->writeDatagram(message, espAddress, espPort);
    if (sent > 0) {
        sendTimer->start();
    } else {
        qDebug() << "? Errore invio UDP:" << udpSocket->errorString();
    }

}

bool udpClient::isConnectedToESP(const QString &newSsid) {
    return (newSsid == ESP_SSID_NAME);
}

void udpClient::bindToWiFiManager(WiFiManager *manager) {
    qDebug() << "WiFiManager pointer passato a udpClient:" << manager;

    connect(manager, &WiFiManager::ssidChanged, this, [=](const QString &ssid){
        qDebug() << "SSID attuale:" << ssid;

        if (isConnectedToESP(ssid)) {
            sendTimer->start();
        } else {
            sendTimer->stop();
        }
    });
}

