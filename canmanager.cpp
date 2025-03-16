#include "canmanager.h"
#include <QDebug>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>

CanManager::CanManager(dynamometerData *dynoData, QObject *parent) : QObject(parent), m_dynoData(dynoData)
{
    qDebug() << "CanManager constructor called!";
    // Apriamo il socket CAN
    socketCan = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketCan < 0) {
        qWarning() << "Errore apertura socket CAN";
        emit errorOccurred("Errore apertura socket CAN");
        return;
    }

    struct sockaddr_can addr;
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, "can0");
    ioctl(socketCan, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socketCan, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Errore nel binding del socket CAN");
        qWarning() << "Errore nel binding del socket CAN, codice errore:" << strerror(errno);
        emit errorOccurred("Errore nel binding del socket CAN");
        close(socketCan);
        return;
    }

    qDebug() << "Socket CAN configurato correttamente!";

    // Configuriamo il timer per inviare messaggi ogni 200ms
    sendTimer = new QTimer(this);
    sendTimer->setInterval(200);
    connect(sendTimer, &QTimer::timeout, this, &CanManager::sendPeriodicMessage);

    qDebug() << "Timer creato e collegato!";
    initialized = true;
}

const QMap<QChar, char> CanManager::stateMap = {
    {'S', 1},
    {'N', 2},
    {'E', 3}
};

void CanManager::sendCanMessage(uint32_t id, const QByteArray &data)
{
    struct can_frame frame;
    frame.can_id = id;
    frame.can_dlc = data.size();
    std::memcpy(frame.data, data.constData(), frame.can_dlc);

    if (write(socketCan, &frame, sizeof(frame)) != sizeof(frame)) {
        qWarning() << "Errore nell'invio del messaggio CAN";
        emit errorOccurred("Errore nell'invio del messaggio CAN");
    } else {
        ;//qDebug() << "Messaggio CAN inviato: ID" << id << "Dati:" << data.toHex();
    }
}

void CanManager::sendPeriodicMessage()
{
    if (!m_dynoData) {
        qWarning() << "dynamometerData non inizializzato!";
        return;
    }
    // Ottieni i valori dal dinamometro
    QString valueStr = m_dynoData->getDynValue();
    QString stateStr = m_dynoData->getDynState();

    // Converti il valore in 4 byte
    uint32_t numericValue = valueStr.toUInt();
    QByteArray canData;
    canData.append(static_cast<char>((numericValue >> 24) & 0xFF));
    canData.append(static_cast<char>((numericValue >> 16) & 0xFF));
    canData.append(static_cast<char>((numericValue >> 8) & 0xFF));
    canData.append(static_cast<char>(numericValue & 0xFF));

    // Stato in 1 byte
    char stateByte = stateMap.value(stateStr.isEmpty() ? 'X' : stateStr.toUtf8().at(0), 0);
    canData.append(stateByte);

    // Padding con 0xFF
    canData.append('\xFF');
    canData.append('\xFF');
    canData.append('\xFF');

    sendCanMessage(0x123, canData);
}

void CanManager::startSending()
{
    if (!sendTimer) {
        qWarning() << "sendTimer non inizializzato!";
        return;
    }
    if (!sendTimer->isActive()) {
        sendTimer->start();
        qDebug() << "Invio periodico CAN avviato";
    }
}

void CanManager::stopSending()
{
    if (!sendTimer) {
        qWarning() << "sendTimer non inizializzato!";
        return;
    }
    if (sendTimer->isActive()) {
        sendTimer->stop();
        qDebug() << "Invio periodico CAN fermato";
    }
}

bool CanManager::isInitialized() const { return initialized; }
