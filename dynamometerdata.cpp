#include "dynamometerdata.h"
#include <QDebug>

dynamometerData::dynamometerData(QObject *parent) : QObject(parent) {}


QString dynamometerData::getDynValue() const { return m_value; }

QString dynamometerData::getDynState() const { return m_state; }


void dynamometerData::setValue(const QString &value) {
    if (m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}

void dynamometerData::setState(const QString &state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged();
    }
}

dynamometerData::DecodedMessage dynamometerData::decodeMessage(const QByteArray &data) {

    DecodedMessage message = {"", "", false};

    QString rawMessage = QString::fromUtf8(data).trimmed();
    if (rawMessage.startsWith('\x02') && rawMessage.endsWith('\x03')) {
        rawMessage = rawMessage.mid(1, rawMessage.size() - 2).trimmed();

        if (rawMessage.size() >= 2) {
            message.state = rawMessage.left(1);         // Stato
            //qDebug() << "Stato estratto:" << message.state;
            message.value = rawMessage.mid(1).trimmed(); // Valore
            qDebug() << "Valore estratto:" << message.value;
            message.isValid = true;

            setState(message.state);
            setValue(message.value);


            // Emissione del segnale con i dati decodificati
            emit decodedMessage(message.state, message.value);
        }
    }

    return message;
}
