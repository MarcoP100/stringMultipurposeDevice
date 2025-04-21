#include "protocolmanager.h"
#include "crc.h"
#include <QDebug>
#include "protocol_constants.h"

ProtocolManager::ProtocolManager(QObject *parent) : QObject(parent) {

    setupTimers();

}


void ProtocolManager::handleIncomingMessage(const QByteArray &data) {
    if (!data.startsWith(PROTO_START)) return;

        DecodedMessage message = decodeMessage(data);

    if (!message.isValid) {
        sendNack(message.counter);
        return;
    }

    sendAck(message.counter);
    emit validMessageReceived(message.state, message.value);
    stopHeartbeat();
    resumeTimeoutMsg();

}



ProtocolManager::DecodedMessage ProtocolManager::decodeMessage(const QByteArray &data) {

    DecodedMessage message;
    message.isValid = false;

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

                if (isMessageOk(message, data)){
                    message.isValid = true;
                    // valore
                    message.value = messageValue(fields[4]);

                }

            }
        }
    }
    return message;
}

QString ProtocolManager::messageValue (const QString &data){
    bool isNumeric;
    int numericValue = data.toInt(&isNumeric);
    QString value = "";

    // Se il valore non è numerico, è fuori range o negativo, impostiamo "------"
    if ((!isNumeric) || (numericValue < 0) || (numericValue > 999999)) {
        value = "000000";
    } else {
        value = QString("%1").arg(numericValue / 2, 6, 10, QChar(0x2007)); // Stringa a 6 cifre
        //message.value = QString::number(numericValue / 2).rightJustified(6, ' ', true);
    }
    return value;
}


bool ProtocolManager::isCounterOk(int newCounter) {
    if (m_lastCounter < 0){
        m_lastCounter = newCounter;
        return true; // primo valore
    }

    int delta = (newCounter - m_lastCounter + 256) % 256;
    if (delta > 1) {
        m_packetsLost += (delta - 1);
        emit packetsLostChanged(m_packetsLost);
    }

    bool ok = (delta >= 0);
    if (ok) m_lastCounter = newCounter;

    return ok;
}

int ProtocolManager::getPacketsLost() const {
    return m_packetsLost;
}

bool ProtocolManager::verifyCrc(const DecodedMessage &message, const QByteArray &originalData, uint16_t *calculatedCrc) {
    // ricostruisci la parte da usare per il CRC
    // escludi STX (\x02) e ETX (\x03)
    int start = originalData.indexOf('\x02');
    int end = originalData.lastIndexOf('\x03');
    if (start < 0 || end <= start) return false;

    QByteArray payload = originalData.mid(start + 1, end - start - 1);  // escluso STX/ETX
    int crcSep = payload.lastIndexOf('|');
    if (crcSep < 0) return false;

    QByteArray crcPayload = payload.left(crcSep);  // solo la parte prima del CRC

    /*for (int i = 0; i < crcPayload.size(); ++i) {
        qDebug() << "Byte" << i << ":" << QString("0x%1").arg((unsigned char)crcPayload[i], 2, 16, QChar('0'));
    }*/

    *calculatedCrc = crc16_ccitt((const uint8_t*)crcPayload.data(), crcPayload.size(), 0xFFFF);

    bool ok = false;
    uint16_t receivedCrc = message.crc.toUShort(&ok, 16);

    return ok && (*calculatedCrc == receivedCrc);
}

// verifica mittente messaggio
bool ProtocolManager::isSenderOk(const QString &sender, const QString senderRef){
    return sender == senderRef;
}

bool ProtocolManager::isValueMsg(const QString &msgType){
    return msgType == QString(MSG_TYPE_VALUE);
}

bool ProtocolManager::isMessageOk(const DecodedMessage &message, const QByteArray &originalData){

    //verifica searriva da esp32 corretto
    if (!isSenderOk(message.senderId, ESP_ID)){
        qWarning() << "Mittente non corretto" << message.senderId;
        return false;
    }

    if (!isValueMsg(message.msgType)){
        qWarning() << "Tipo messaggio non corretto" << message.msgType;
        return false;
    }


    if (!isCounterOk(message.counter)) {

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



void ProtocolManager::sendAck(uint8_t counter) {
    qWarning() << "Ack Counter:" << counter;
    QString ack = QString("\x02%1|%2|%3\x03")
    .arg(PROTO_ID)
        .arg(MSG_TYPE_ACK)
        .arg(counter);
    QHostAddress espAddress(ESP_STATIC_IP);
    emit messageToSend(ack.toUtf8(), espAddress, ESP_PORT);
}

void ProtocolManager::sendNack(uint8_t counter) {
    QString nack = QString("\x02%1|%2|%3\x03")
    .arg(PROTO_ID)
        .arg(MSG_TYPE_NACK)
        .arg(counter);
    QHostAddress espAddress(ESP_STATIC_IP);
    emit messageToSend(nack.toUtf8(), espAddress, ESP_PORT);
}

void ProtocolManager::startHeartbeat() {
    qDebug() << "Heartbeath start";
    if (!heartbeatTimer->isActive())
        heartbeatTimer->start();
}

void ProtocolManager::stopHeartbeat(){
    if (heartbeatTimer->isActive())
        qDebug() << "Heartbeath stop";
        heartbeatTimer->stop();
}

QString ProtocolManager::createHeartbeatMessage() {
    QString msg = QString("%1%2|%3%4")
    .arg(PROTO_START)
    .arg(PROTO_ID)
    .arg(MSG_TYPE_HEARTBEAT)
    .arg(PROTO_END);

    return msg;
}

void ProtocolManager::setupTimers(){
    heartbeatTimer = new QTimer(this);
    heartbeatTimer->setInterval(5000);  // 5s di timeout
    heartbeatTimer->setSingleShot(false);

    QString msg = createHeartbeatMessage();
    QHostAddress espAddress(ESP_STATIC_IP);
    connect(heartbeatTimer, &QTimer::timeout, this, [=]() {
        qDebug() << "Heartbeath invio";
        emit messageToSend(msg.toUtf8(), espAddress, ESP_PORT);

    });


    // timeout messaggi validi
    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(10000);  // 5s di timeout
    timeoutTimer->setSingleShot(true);

    connect(timeoutTimer, &QTimer::timeout, this, [=]() {
        //qDebug() << "Timeout: Nessun dato ricevuto da 5s!";
        startHeartbeat();
    });
}

void ProtocolManager::resumeTimeoutMsg() {
    if (!timeoutTimer->isActive())
        timeoutTimer->start();
}



