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

public:
    explicit dynamometerData(QObject *parent = nullptr);

    struct DecodedMessage {
        QString state; // Stato (E/S/N)
        QString value; // Valore
        bool isValid;  // Se il messaggio � valido
    };

    DecodedMessage decodeMessage(const QByteArray &data);

    QString getDynValue() const;
    QString getDynState() const;

signals:
    void valueChanged();
    void stateChanged();
    void decodedMessage(const QString &state, const QString &value);

private:
    QString m_value;
    QString m_state;

    void setValue(const QString &value);

    void setState(const QString &state);
};

#endif // DYNAMOMETERDATA_H
