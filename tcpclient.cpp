#include "tcpclient.h"
#include <QDebug>


tcpClient::tcpClient(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);
    // Connetti i segnali per la gestione della connessione
    connect(socket, &QTcpSocket::stateChanged, this, [this](QAbstractSocket::SocketState state){
        setSocketState(static_cast<SocketState>(state));
    });

    //connect(socket, &QTcpSocket::connected, this, &tcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &tcpClient::onReadyRead);
    /*connect(socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "Disconnected from server";
    });*/

    connect(socket, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError error) {
        qDebug() << "Errore TCP:" << socket->errorString();
        emit connectionFailed(socket->errorString()); // Segnale per notificare l'errore
    });

    // Timer per gestire il timeout
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &tcpClient::onConnectTimeout);
}

tcpClient::~tcpClient() {
    delete socket;
}

void tcpClient::onConnectTimeout() {
    qWarning() << "Timeout raggiunto. La connessione non � stata completata in tempo.";

    if (socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "Connection timeout!";
        socket->abort();
        emit connectionFailed("Connection timed out");
    }
    m_timeoutTimer->deleteLater();
}


void tcpClient::connectToESP32(const QString &host, quint16 port)
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        if (socket->peerAddress().toString() == host && socket->peerPort() == port) {
            qDebug() << "Already connected to" << host << ":" << port;
            return; // Evita di disconnettere/ripetere la connessione
        } else {
            qDebug() << "Connected to a different server. Disconnecting...";
            disconnectFromESP32();
        }
    }

    qDebug() << "Connecting to ESP32 at" << host << ":" << port;



    m_timeoutTimer->start(10000);

    socket->connectToHost(host, port);
}

/*void tcpClient::onConnected(){
    qDebug() << "Connected!";
    emit connectionEstablished(); // Notifica al QML
}*/

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

void tcpClient::disconnectFromESP32() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Disconnecting...";
        socket->disconnectFromHost();
    } else {
        qDebug() << "Not connected. Nothing to disconnect.";
    }
}

void tcpClient::setSocketState(SocketState newState) {
    if (m_socketState != newState) {
        m_socketState = newState;
        qDebug() << "Cambio stato " << newState;

        if (m_socketState == SocketState::ConnectedState) {
            m_timeoutTimer->stop(); // Ferma il timer se ci si connette
            m_timeoutTimer->deleteLater(); // Elimina il timer
        }
        emit socketStateChanged(m_socketState);
    }
}
