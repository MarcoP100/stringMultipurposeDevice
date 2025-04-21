#include "udpclient.h"
#include <QDebug>
#include "protocol_constants.h"




udpClient::udpClient(QObject *parent) : QObject(parent) {

    setupSocket();
    setupTimers();

    qDebug() << "Nuovo socket UDP creato e segnali collegati.";
}

void udpClient::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        emit rawDataReceived(datagram);

    }
}

void udpClient::sendMessage(const QByteArray &message,
                            const QHostAddress &address,
                            quint16 port){
    qint64 sent = udpSocket->writeDatagram(message, address, port);
    if (sent > 0) {
        qDebug("Heartbeat inviato");
    } else {

        qDebug() << "? Errore invio UDP:" << udpSocket->errorString();
    }

}


void udpClient::setupSocket(){
    udpSocket = new QUdpSocket(this);
    bool success = udpSocket->bind(QHostAddress::Any, UDP_PORT);
    if (!success) {
        qDebug() << "Errore bind UDP:" << udpSocket->errorString();
    }
    connect(udpSocket, &QUdpSocket::readyRead, this, &udpClient::processPendingDatagrams);
}

void udpClient::setupTimers(){




}




