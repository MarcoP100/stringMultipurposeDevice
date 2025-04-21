#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>


constexpr quint16 UDP_PORT = 50000;

class udpClient: public QObject {
    Q_OBJECT


public:
    explicit udpClient(QObject *parent = nullptr);
    void sendMessage(const QByteArray &message,
                     const QHostAddress &address,
                     quint16 port);

signals:
    void rawDataReceived(QByteArray data);

private slots:
    void processPendingDatagrams();

private:

    QUdpSocket *udpSocket = nullptr;

    void setupSocket();
    void setupTimers();
};

#endif // UDPCLIENT_H
