#include "dynamometerdata.h"
#include <QDebug>
#include "crc.h"
#include "protocol_constants.h"

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

                /*qDebug() << "Ricevuto messaggio da" << fields[0]
                         << "valore:" << fields[4]
                         << "CRC:" << fields[5];*/


                message.senderId = fields[0];
                message.msgType = fields[1];
                message.counter = fields[2].toInt();
                message.state = fields[3];
                message.crc = fields[5];

                if (!isMessageOk(message, data)){
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


void dynamometerData::handleTimeout() {
    setValue("------");
    setState("X");  // oppure "Timeout" o stato dedicato
    emit decodedMessage(m_state, m_value);
}


bool dynamometerData::isCounterOk(int newCounter) {
    if (m_lastCounter < 0) return true; // primo valore
    int delta = (newCounter - m_lastCounter + 256) % 256;
    if (delta > 1) {
        m_packetsLost += (delta - 1);
        emit packetsLostChanged();
    }
    return delta > 0;
}

int dynamometerData::getPacketsLost() const { return m_packetsLost; }

bool dynamometerData::verifyCrc(const DecodedMessage &message, const QByteArray &originalData, uint16_t *calculatedCrc) {
    // ricostruisci la parte da usare per il CRC
    // escludi STX (\x02) e ETX (\x03)
    int start = originalData.indexOf('\x02');
    int end = originalData.lastIndexOf('\x03');
    if (start < 0 || end <= start) return false;

    QByteArray payload = originalData.mid(start + 1, end - start - 1);  // escluso STX/ETX

    *calculatedCrc = crc16_ccitt((const uint8_t*)payload.data(), payload.size(), 0xFFFF);

    bool ok = false;
    uint16_t receivedCrc = message.crc.toUShort(&ok, 16);

    return ok && (*calculatedCrc == receivedCrc);
}

// verifica mittente messaggio
bool dynamometerData::isSenderOk(const QString &sender, const QString senderRef){
    return sender == senderRef;
}

bool dynamometerData::isValueMsg(const QString &msgType){
    return msgType == MSG_TYPE_VALUE;
}

bool dynamometerData::isMessageOk(const DecodedMessage &message, const QByteArray &originalData){

    //verifica searriva da esp32 corretto
    if (!isSenderOk(message.senderId, ESP_ID)){
            qWarning() << "Mittente non corretto" << message.senderId;
            return false;
    }

    if (!isValueMsg(message.msgType)){
        qWarning() << "Tipo messaggio non corretto" << message.msgType;
        return false;
    }


    if (isCounterOk(message.counter)) {
        m_lastCounter = message.counter;
        // prosegui con visualizzazione
    } else {
        qWarning() << "?? Pacchetto saltato o duplicato. Counter:" << message.counter;
        return false;
    }

    uint16_t calculatedCrc = 0;
    if (!verifyCrc(message, originalData, &calculatedCrc)){
        qWarning() << "Crc non corretto. Inviato: " << message.crc <<" - crc calcolato: " << QString::number(calculatedCrc, 16).toUpper();
        return false;
    }

    return true;
}

