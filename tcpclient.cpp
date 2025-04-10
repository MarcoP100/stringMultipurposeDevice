#include "tcpclient.h"
#include <QDebug>



tcpClient::tcpClient(CanManager *canManager, QObject *parent)
    : QObject{parent}, canManager(canManager)
{
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(5000);  // 5s di timeout
    timeoutTimer->setSingleShot(true);

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        //qDebug() << "Timeout: Nessun dato ricevuto da 5s!";
        emit connectionTimeout();
    });
}

tcpClient::~tcpClient() {
    delete socket;
}


void tcpClient::initializeSocket() {
    if (socket) {

        // **Ferma il timer per evitare problemi**
        if (m_timeoutTimer && m_timeoutTimer->isActive()) {
            qDebug() << "Timeout in corso, lo fermo.";
            m_timeoutTimer->stop();
        }

        qDebug() << "Forzata chiusura socket esistente.";
        socket->abort();
        delete socket;
        socket = nullptr;
    }

    socket = new QTcpSocket(this);

    // verifico stato iniziale
    QAbstractSocket::SocketState currentState = socket->state();
    setSocketState(static_cast<SocketState>(currentState));


    // Connetti i segnali
    connect(socket, &QTcpSocket::stateChanged, this, [this](QAbstractSocket::SocketState state){
        setSocketState(static_cast<SocketState>(state));
    });

    connect(socket, &QTcpSocket::readyRead, this, &tcpClient::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &tcpClient::onTcpError);
    connect(socket, &QTcpSocket::connected, this, &tcpClient::onTcpConnected);
    connect(socket, &QTcpSocket::disconnected, this, &tcpClient::onTcpDisconnected);

    // Timer per gestire il timeout
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &tcpClient::onConnectTimeout);

    qDebug() << "Nuovo socket TCP creato e segnali collegati.";
}


void tcpClient::onConnectTimeout() {
    qWarning() << "Timeout raggiunto. La connessione non � stata completata in tempo.";

    if (socket->state() != QTcpSocket::ConnectedState) {
        //qDebug() << "Connection timeout!";
        socket->abort();
        emit connectionFailed("Connection timed out");
    }
    if (m_timeoutTimer) {
        //qDebug() << "Timer fermato, pronto per essere riutilizzato.";
        m_timeoutTimer->stop();
    } else {
        qWarning() << "Errore: m_timeoutTimer è NULL!";
    }
}

void tcpClient::onTcpError(QAbstractSocket::SocketError error) {
        qWarning() << "Errore TCP:" << error << "-" << socket->errorString();
        emit connectionFailed(socket->errorString());
    }



void tcpClient::connectToESP32(const QString &host, quint16 port)
{
    if (!socket) {
        qDebug() << "Socket non presente. Lo inizializzo";
        initializeSocket(); // Inizializza il socket
    } else{
        if (socket->state() == QTcpSocket::ConnectingState) {
            qDebug() << "Forzata chiusura socket per evitare problemi di riconnessione.";
            socket->abort();

        }
    }

    if (socket->state() == QTcpSocket::ConnectedState) {
        if (socket->peerAddress().toString() == host && socket->peerPort() == port) {
            qDebug() << "Already connected to" << host << ":" << port;
            return; // Evita di disconnettere/ripetere la connessione
        } else {
            qDebug() << "Connected to a different server. Disconnecting...";
            disconnectFromESP32();
            QTimer::singleShot(500, this, [=]() {});
        }
    }
    qDebug() << "Connecting to ESP32 at" << host << ":" << port;


    if (m_timeoutTimer) {
        if (m_timeoutTimer->isActive()) {
            qDebug() << "Timeout gi� attivo, lo fermo prima di riavviarlo.";
            m_timeoutTimer->stop();
        }
        m_timeoutTimer->start(10000);
    }

    qDebug() << "Timer superato";

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

    timeoutTimer->start();
}

void tcpClient::disconnectFromESP32() {

    if (!socket) {
        qWarning() << "Errore: Tentativo di disconnettere un socket nullo!";
        return;
    }

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
        }
        emit socketStateChanged(m_socketState);
    }
}

void tcpClient::onTcpConnected() {
    qDebug() << "TCP Connected, starting CAN messages.";
    if (!canManager || !canManager->isInitialized()) {
        qWarning() << "CanManager non inizializzato, non avvio il CAN!";
        return;
    }
    canManager->startSending();
}

void tcpClient::onTcpDisconnected() {
    qDebug() << "TCP Disconnected, stopping CAN messages.";
    if (!canManager || !canManager->isInitialized()) {
        qWarning() << "CanManager non inizializzato, non fermo il CAN!";
        return;
    }
    canManager->stopSending();
}

tcpClient::SocketState tcpClient::getSocketState() const {
    return m_socketState;
}
