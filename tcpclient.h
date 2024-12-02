#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class tcpClient : public QObject
{
    Q_OBJECT
public:
    explicit tcpClient(QObject *parent = nullptr);
    ~tcpClient();

    Q_INVOKABLE void connectToESP32(const QString &host, quint16 port);


signals:
    void connectionFailed(const QString &error); // Segnale per notificare errori

private slots:
    void onConnected();
    void onReadyRead();
    //void onDisconnected();

private:
    QTcpSocket *socket;

    void sendData(const QString &data);
    void cleanup();

};

#endif // TCPCLIENT_H
