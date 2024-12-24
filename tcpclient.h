#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>


class tcpClient : public QObject
{
    Q_OBJECT

Q_PROPERTY(SocketState socketState READ socketState NOTIFY socketStateChanged)

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

    explicit tcpClient(QObject *parent = nullptr);
    ~tcpClient();

    Q_INVOKABLE void connectToESP32(const QString &host, quint16 port);
    Q_INVOKABLE void disconnectFromESP32();

    SocketState socketState() const { return m_socketState; }

signals:
    void connectionFailed(const QString &error); // Segnale per notificare errori
    void socketStateChanged(const SocketState &state);
    void rawDataReceived(const QByteArray &data);

private slots:
    //void onConnected();
    void onReadyRead();
    //void onDisconnected();

private:
    QTcpSocket *socket;
    SocketState m_socketState = UnconnectedState;
    QTimer *m_timeoutTimer = nullptr;

    //void setSocketState(SocketState newState);
    void sendData(const QString &data);
    void onConnectTimeout();
    void setSocketState(const SocketState newState);
};

#endif // TCPCLIENT_H
