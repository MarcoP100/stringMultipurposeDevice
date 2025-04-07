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
#include <QFile>

CanManager::CanManager(dynamometerData *dynoData, QObject *parent) : QObject(parent), m_dynoData(dynoData)
{
    qDebug() << "CanManager constructor called!";

    QStringList interfaces = {"can0", "can1"};
    int* sockets[] = {&socketCan0, &socketCan1};

    for (int i = 0; i < interfaces.size(); ++i) {
        *sockets[i] = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (*sockets[i] < 0) {
            qWarning() << "Errore apertura socket" << interfaces[i];
            emit errorOccurred("Errore apertura socket CAN0");
            continue;
        }

        // Rendi il socket non bloccante
        int flags = fcntl(*sockets[i], F_GETFL, 0);
        fcntl(*sockets[i], F_SETFL, flags | O_NONBLOCK);

        struct sockaddr_can addr;
        struct ifreq ifr;
        std::strncpy(ifr.ifr_name, interfaces[i].toStdString().c_str(), IFNAMSIZ);
        ioctl(*sockets[i], SIOCGIFINDEX, &ifr);

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;


        if (bind(*sockets[i], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            qWarning() << "Errore nel binding del socket" << interfaces[i] << ":" << strerror(errno);
            close(*sockets[i]);
            *sockets[i] = -1;
            continue;
        }

        qDebug() << "Socket" << interfaces[i] << "configurato correttamente!";
    }

    /*
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

    */

    // Configuriamo il timer per inviare messaggi ogni 200ms
    sendTimer = new QTimer(this);
    sendTimer->setInterval(200);
    connect(sendTimer, &QTimer::timeout, this, &CanManager::sendPeriodicMessage);

    qDebug() << "Timer creato e collegato!";


    //timer per scan network
    m_stateChangeTimer = new QTimer(this);
    m_stateChangeTimer->setInterval(5000);  // 5 secondi di ritardo
    m_stateChangeTimer->setSingleShot(false);  // Timer a colpo singolo

    connect(m_stateChangeTimer, &QTimer::timeout, this, [this]() {
        updateCanState("can0", m_can0State);
        updateCanState("can1", m_can1State);
    });



    initialized = true;
}

const QMap<QChar, char> CanManager::stateMap = {
    {'S', 1},
    {'N', 2},
    {'E', 3}
};

void CanManager::sendCanMessage(uint32_t id, const QByteArray &data, const int socket)
{
    struct can_frame frame;
    frame.can_id = id;
    frame.can_dlc = data.size();
    std::memcpy(frame.data, data.constData(), frame.can_dlc);

    if (socket >= 0){
           if (write(socket, &frame, sizeof(frame)) != sizeof(frame)) {
                qWarning() << "Errore nell'invio sul socket CAN:" << strerror(errno);
                emit errorOccurred("Errore nell'invio del messaggio CAN");
            } else {
                ;//qDebug() << "Messaggio CAN inviato: ID" << id << "Dati:" << data.toHex();
            }
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

    // Rimuove spazi invisibili o caratteri speciali
    valueStr = valueStr.trimmed().remove(QChar(0x2007));

    // Converti il valore in 4 byte
    bool ok;
    uint32_t numericValue = valueStr.toUInt(&ok);
    if (!ok) {
        qWarning() << "Errore nella conversione del valore!";
    }

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

    sendCanMessage(0x3ff, canData, socketCan0);
    sendCanMessage(0x3ff, canData, socketCan1);
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

CanManager::CanState CanManager::getCanState(const QString &interfaceName) {
    QFile file("/sys/class/net/" + interfaceName + "/operstate");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString stateStr = QString::fromUtf8(file.readAll()).trimmed();
        if (stateStr == "up") return CanState::up;
        if (stateStr == "down") return CanState::down;
    }
    return CanState::unknown;
}


void CanManager::updateCanState(const QString &interfaceName, CanState &storedState) {
    CanState currentState = getCanState(interfaceName);
    qDebug() << "Stato can: " << currentState;
    if (storedState != currentState) {
        storedState = currentState;

        emit canStateChanged(Can0);  // Notifica QML che lo stato � cambiato
    }
}

CanManager::CanState  CanManager::can0State() const {
    return m_can0State;
}

CanManager::CanState  CanManager::can1State() const {
    return m_can1State;
}
