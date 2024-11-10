#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = nullptr);
    Q_INVOKABLE void connectToESP32(const QString &host, quint16 port);


signals:


private slots:
    //void onConnected();
    //void onReadyRead();
    //void onDisconnected();

private:
    QTcpSocket *socket;

};

#endif // TCPCLIENT_H
