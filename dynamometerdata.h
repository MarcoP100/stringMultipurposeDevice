#ifndef DYNAMOMETERDATA_H
#define DYNAMOMETERDATA_H

#include <QObject>
#include <QString>
#include <QTimer>

class dynamometerData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dynValue READ getDynValue NOTIFY valueChanged)
    Q_PROPERTY(QString dynState READ getDynState NOTIFY stateChanged)
    Q_PROPERTY(int packetsLost READ getPacketsLost NOTIFY packetsLostChanged)

public:
    explicit dynamometerData(QObject *parent = nullptr);

    struct DecodedMessage {
        QString senderId;
        QString msgType;
        int counter;
        QString state; // Stato (E/S/N)
        QString value; // Valore
        QString crc;
        bool isValid;  // Se il messaggio � valido
    };

    DecodedMessage decodeMessage(const QByteArray &data);
    void handleTimeout();

    QString getDynValue() const;
    QString getDynState() const;
    int getPacketsLost() const;

signals:
    void valueChanged();
    void stateChanged();
    void packetsLostChanged();
    void decodedMessage(const QString &state, const QString &value);

private:
    QString m_value;
    QString m_state;
    int m_lastCounter = -1;
    int m_packetsLost = 0;

    void setValue(const QString &value);

    void setState(const QString &state);
    bool isMessageOk(const DecodedMessage &message, const QByteArray &originalData);
    bool verifyCrc(const DecodedMessage &message, const QByteArray &originalData, uint16_t *calculatedCrc);
    bool isCounterOk(int newCounter);
    bool isSenderOk(const QString &sender, const QString senderRef);
    bool isValueMsg(const QString &msgType);
};

#endif // DYNAMOMETERDATA_H
