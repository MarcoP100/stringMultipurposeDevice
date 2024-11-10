#include "tcpclient.h"

tcpClient::tcpClient(QObject *parent)
    : QObject{parent}
{
    //socket = new QTcpSocket(this);
    // Connetti i segnali per la gestione della connessione
    //connect(socket, &QTcpSocket::connected, this, &tcpClient::onConnected);
    //connect(socket, &QTcpSocket::readyRead, this, &tcpClient::onReadyRead);
    //connect(socket, &QTcpSocket::disconnected, this, &tcpClient::onDisconnected);
}


void tcpClient::connectToESP32(const QString &host, quint16 port)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Already connected, disconnecting...";
        socket->disconnectFromHost();
        if (!socket->waitForDisconnected(3000)) {
            qDebug() << "Failed to disconnect the previous connection!";
            return;
        }
    }

    qDebug() << "Connecting to ESP32...";
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed!";
    }
}

