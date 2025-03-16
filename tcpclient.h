#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "canmanager.h"


class tcpClient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(SocketState socketState READ getSocketState NOTIFY socketStateChanged)

public:

    enum SocketState {
        UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ClosingState,
        ListeningState
    };
    Q_ENUM(SocketState) // Rende l'enum accessibile a QML

    explicit tcpClient(CanManager *canManager, QObject *parent = nullptr);
    ~tcpClient();

    Q_INVOKABLE void connectToESP32(const QString &host, quint16 port);
    Q_INVOKABLE void disconnectFromESP32();

    Q_INVOKABLE SocketState getSocketState() const;


signals:
    void connectionFailed(const QString &error); // Segnale per notificare errori
    void socketStateChanged(const SocketState &state);
    void rawDataReceived(const QByteArray &data);
    void connectionTimeout();  // Segnale di timeout

private slots:
    //void onConnected();
    void onReadyRead();
    //void onDisconnected();
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpError(QAbstractSocket::SocketError error);

private:
    QTcpSocket *socket;
    SocketState m_socketState = UnconnectedState;
    QTimer *m_timeoutTimer = nullptr;

    //void setSocketState(SocketState newState);
    void sendData(const QString &data);
    void onConnectTimeout();
    void setSocketState(const SocketState newState);
    void initializeSocket();
    CanManager *canManager;
    QTimer *timeoutTimer;

};

#endif // TCPCLIENT_H
