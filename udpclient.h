#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include "wifimanager.h"

class udpClient: public QObject {
    Q_OBJECT

public:
    explicit udpClient(QObject *parent = nullptr);
    void sendMessage(const QByteArray &message);
    void bindToWiFiManager(WiFiManager *manager);

signals:
    void rawDataReceived(QByteArray data);
    void udpConnectionTimeout();

private slots:
    void processPendingDatagrams();

private:
    QTimer *timeoutTimer;
    QTimer *sendTimer;
    QUdpSocket *udpSocket = nullptr;
    bool isConnectedToESP(const QString &newSsid);

};

#endif // UDPCLIENT_H
