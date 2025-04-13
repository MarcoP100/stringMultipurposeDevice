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

        rawMessage = rawMessage.mid(1);  // rimuovi \x02
        int end = rawMessage.indexOf('\x03');
        if (end >= 0)
            rawMessage = rawMessage.left(end);  // rimuovi tutto dopo \x03

        if (rawMessage.size() >= 2) {

            QStringList fields = rawMessage.split('|');
            if (fields.size() == 6) {

                qDebug() << "Ricevuto messaggio da" << fields[0]
                         << "valore:" << fields[4]
                         << "CRC:" << fields[5];


                message.senderId = fields[0];
                message.msgType = fields[1];
                message.counter = fields[2].toInt();
                message.state = fields[3];
                message.crc = fields[5];

                if (message.msgType != "V") {
                    return message;  // supporti solo messaggi 'V' al momento
                }

                // valore
                bool isNumeric;
                int numericValue = fields[4].toInt(&isNumeric);

                // Se il valore non è numerico, è fuori range o negativo, impostiamo "------"
                if ((!isNumeric) || (numericValue < 0) || (numericValue > 999999)) {
                    message.value = "000000";
                } else {
                    message.value = QString("%1").arg(numericValue / 2, 6, 10, QChar(0x2007)); // Stringa a 6 cifre
                    //message.value = QString::number(numericValue / 2).rightJustified(6, ' ', true);
                }

                message.isValid = true;

                setState(message.state);
                setValue(message.value);


                // Emissione del segnale con i dati decodificati
                emit decodedMessage(message.state, message.value);
            }
        }
    }

    return message;
}
