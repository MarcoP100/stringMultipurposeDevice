#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QHostAddress>


class ProtocolManager: public QObject {

    Q_OBJECT
public:
    explicit ProtocolManager(QObject *parent = nullptr);

    struct DecodedMessage {
        QString senderId;
        QString msgType;
        int counter;
        QString state; // Stato (E/S/N)
        QString value; // Valore
        QString crc;
        bool isValid;  // Se il messaggio � valido
    };

    int getPacketsLost() const;

public slots:
    void startHeartbeat();
    void stopHeartbeat();
    void handleIncomingMessage(const QByteArray &data);

signals:
    void packetsLostChanged(const int &packetslost);
    void validMessageReceived(const QString &state, QString &value);
    void messageToSend(const QByteArray &message,
                            const QHostAddress &address,
                            quint16 port);


private:
    int m_lastCounter = -1;
    int m_packetsLost = 0;
    QTimer *heartbeatTimer;
    QTimer *timeoutTimer;


    DecodedMessage decodeMessage(const QByteArray &data);
    bool isMessageOk(const DecodedMessage &message, const QByteArray &originalData);
    bool verifyCrc(const DecodedMessage &message, const QByteArray &originalData, uint16_t *calculatedCrc);
    bool isCounterOk(int newCounter);
    bool isSenderOk(const QString &sender, const QString senderRef);
    bool isValueMsg(const QString &msgType);
    QString messageValue (const QString &data);

    void sendAck(uint8_t counter);
    void sendNack(uint8_t counter);
    void startSendingHeartbeat();
    QString createHeartbeatMessage();
    void setupTimers();
    void resumeTimeoutMsg();
};


#endif // PROTOCOLMANAGER_H
