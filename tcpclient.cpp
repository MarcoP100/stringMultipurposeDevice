#include "tcpclient.h"
#include <QDebug>


tcpClient::tcpClient(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);
    // Connetti i segnali per la gestione della connessione
    connect(socket, &QTcpSocket::connected, this, &tcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &tcpClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "Disconnected from server";
    });

    connect(socket, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
        qDebug() << "Errore TCP:" << socket->errorString();
        emit connectionFailed(socket->errorString()); // Segnale per notificare l'errore
    });
}

tcpClient::~tcpClient() {
    delete socket;
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

    qDebug() << "Connecting to ESP32 at " << host << ":" << port;
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(25000)) {
        qDebug() << "Connection failed!";
         emit connectionFailed("Connection timed out");
    }
}

void tcpClient::onConnected(){
    qDebug() << "Connected!";
}

void tcpClient::sendData(const QString &data) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(data.toUtf8());
        socket->flush();
    } else {
        qDebug() << "Socket is not connected!";
    }
}

void tcpClient::onReadyRead() {
    QByteArray data = socket->readAll();
    emit rawDataReceived(data);
}

void tcpClient::cleanup() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected(3000);
    }
}


