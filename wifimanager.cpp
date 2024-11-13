#include <NetworkManager.h>
#include "wifimanager.h"


WiFiManager::WiFiManager(QObject *parent)
    : QObject{parent}, m_busy(false)
{
    qDebug() << "WiFiManager constructor called - busy is initially set to:" << m_busy;

    // Connessione al bus di sistema
    QDBusConnection bus = QDBusConnection::systemBus();
    if (!bus.isConnected()) {
        qWarning() << "Unable to connect to the DBus system bus";
        return;
    }

    listNetworkDevices();
    checkCurrentConnectionStatus();
    setupPropertyChangedSignal();

}

QObject* WiFiManager::createSingletonInstance(QQmlEngine*, QJSEngine*) {
    return new WiFiManager();
}


void WiFiManager::setBusy(bool busy)
{
    if (m_busy != busy) {
        qDebug() << "WiFiManager setBusy called - changing busy from" << m_busy << "to" << busy;

        m_busy = busy;
        emit busyChanged(m_busy);
    } else {
        qDebug() << "WiFiManager setBusy called - busy already" << m_busy << ", no change";
    }
}


bool WiFiManager::isBusy() const
{
    return m_busy;
}

WiFiManager::ConnectionStatus WiFiManager::statusConnection() const
{
    return m_connectionStatus;
}


void WiFiManager::scanNetworks()
{
    setBusy(true);
    if (scanForSavedNetworks){
        getSavedNetworks();

    }
    requestScan();
}

void WiFiManager::requestScan()
{
    QDBusInterface iface("org.freedesktop.NetworkManager",
                         m_wifiDevicePath,
                         "org.freedesktop.NetworkManager.Device.Wireless",
                         QDBusConnection::systemBus());

    // Richiedi una scansione delle reti disponibili

    QVariantMap options;
    QDBusMessage response = iface.call("RequestScan", QVariant::fromValue(options));

    if (!response.errorMessage().isEmpty()) {
        qDebug() << "Failed to initiate network scan:" << response.errorMessage();
    } else {
        qDebug() << "Network scan command sent successfully.";
    }
    qDebug() << "Request scan";

}


void WiFiManager::onPropertiesChanged(QString interfaceName, QVariantMap changedProperties, QStringList invalidatedProperties)
{
    //qDebug() << "PropertiesChanged signal received for interface:" << interfaceName;
    // Verifica se l'interfaccia � quella giusta
    if (interfaceName == "org.freedesktop.NetworkManager.Device.Wireless") {
        //qDebug() << "Property changed: " << changedProperties;
        // Controlla se la propriet� "AccessPoints" � tra quelle modificate
        if (changedProperties.contains("AccessPoints")) {
            qDebug() << "Access points property changed, getting updated list of networks.";
            // Chiama GetAccessPoints per ottenere l'elenco delle reti aggiornato
            getAccessPoints();
        }
    }else if (interfaceName == "org.freedesktop.NetworkManager.Device"){
        if (changedProperties.contains("State")) {
            uint newState = changedProperties.value("State").toUInt();
            readConnectionStatus(newState);
        }
    }
}

void WiFiManager::getAccessPoints()
{
    // Chiamata per ottenere i punti di accesso (reti Wi-Fi)
    QDBusInterface iface("org.freedesktop.NetworkManager",
                         m_wifiDevicePath,
                         "org.freedesktop.NetworkManager.Device.Wireless",
                         QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath>> apReply = iface.call("GetAccessPoints");
    if (apReply.isValid()) {
        QList<QDBusObjectPath> accessPoints = apReply.value();
        // Stampa l'elenco completo dei percorsi degli access points
        qDebug() << "Number of access points found:" << accessPoints.size();
        handleNetworkScanResult(accessPoints);
    } else {
        emit errorOccurred("Failed to get access points");
    }

}



void WiFiManager::handleNetworkScanResult(const QList<QDBusObjectPath> &networks)
{
    QStringList networkList;
    qDebug() << "Handle";
    for (const QDBusObjectPath &networkPath : networks) {
        QDBusInterface apIface("org.freedesktop.NetworkManager",
                               networkPath.path(),
                               "org.freedesktop.NetworkManager.AccessPoint",
                               QDBusConnection::systemBus());

        QVariant ssidReply = apIface.property("Ssid");
        if (ssidReply.isValid() && !ssidReply.isNull()) {
            QString ssid = ssidReply.toString();
            if (!networkList.contains(ssid)) {
                qDebug() << "Found SSID:" << ssid;
                networkList.append(ssid);
            } else {
                qDebug() << "Duplicate SSID found, ignoring:" << ssid;
            }
        } else {
            qDebug() << "Failed to get SSID for access point:" << networkPath.path();
        }
    }
    qDebug() << "Reti trovate:" << networkList;
    emit scanCompleted(networkList);
    setBusy(false);

}


void WiFiManager::connectToNetwork(const QString &ssid, const QString &password)
{
    // Creare e attivare una connessione con `AddAndActivateConnection`
    // ...
}

void WiFiManager::disconnectNetwork()
{
    // Disattivare la connessione con `DeactivateConnection`
    // ...
}


QStringList WiFiManager::getSavedNetworks()
{
    qDebug() << "Ricerca reti salvate";
    QDBusInterface iface("org.freedesktop.NetworkManager",
                         "/org/freedesktop/NetworkManager/Settings",
                         "org.freedesktop.NetworkManager.Settings",
                         QDBusConnection::systemBus());

    QStringList savedNetworks;

    QDBusReply<QList<QDBusObjectPath>> reply = iface.call("ListConnections");

    if (reply.isValid()) {
        for (const QDBusObjectPath &connectionPath  : reply.value()) {
            QDBusInterface connIface("org.freedesktop.NetworkManager",
                                     connectionPath.path(),
                                     "org.freedesktop.NetworkManager.Settings.Connection",
                                     QDBusConnection::systemBus());

            QDBusMessage message = connIface.call("GetSettings");
            if (message.type() == QDBusMessage::ReplyMessage) {
                // Ottieni il contenuto del messaggio di risposta
                if (!message.arguments().isEmpty()) {
                    QVariant argument = message.arguments().first();
                    // Stampa il contenuto di settings per capire cosa contiene
                    const QDBusArgument &dbusArg = argument.value<QDBusArgument>();
                    // Converti l'argomento in QVariantMap
                    // Definisci la mappa in cui inserire i dati
                    QMap<QString, QVariantMap> settings;
                    dbusArg >> settings;  // Estrai i dati da QDBusArgument

                    // Stampa il contenuto di settings per capire cosa contiene
                    //qDebug() << "Settings QVariantMap:" << settings;

                    if (settings.contains("802-11-wireless")) {
                        QVariantMap wifiSettings = settings.value("802-11-wireless");
                        // Estrai l'SSID (memorizzato come QByteArray)
                        QByteArray ssidArray = wifiSettings.value("ssid").toByteArray();
                        QString ssid = QString::fromUtf8(ssidArray);
                        qDebug() << "SSID:" << ssid;
                        if (!ssid.isEmpty() && !ssid.isNull() && !savedNetworks.contains(ssid)){
                            savedNetworks.append(ssid);
                        }
                    }

                } else {
                    qDebug() << "Connection settings not found in settings QVariantMap";
                }

            } else {
                qDebug() << "Failed to get settings for connection:" << connectionPath.path();
                qDebug() << "Error:" << message.errorMessage();
            }
        }
    } else {
        qDebug() << "Failed to list connections:" << reply.error().message();
    }

    scanForSavedNetworks = false;
    qDebug() << "Reti salvate:" << savedNetworks;
    return savedNetworks;
}

void WiFiManager::listNetworkDevices()
{
    // Interfaccia per NetworkManager
    QDBusInterface iface("org.freedesktop.NetworkManager",
                         "/org/freedesktop/NetworkManager",
                         "org.freedesktop.NetworkManager",
                         QDBusConnection::systemBus());

    // Ottieni la lista dei dispositivi gestiti da NetworkManager
    QDBusReply<QList<QDBusObjectPath>> reply = iface.call("GetDevices");
    if (reply.isValid()) {
        QList<QDBusObjectPath> devicePaths = reply.value();


        for (const QDBusObjectPath &devicePath : devicePaths) {
            // Per ogni dispositivo, creiamo una nuova interfaccia per ottenere dettagli
            QDBusInterface deviceIface("org.freedesktop.NetworkManager",
                                       devicePath.path(),
                                       "org.freedesktop.NetworkManager.Device",
                                       QDBusConnection::systemBus());

            // Ottieni il tipo di dispositivo
            QVariant typeReply = deviceIface.property("DeviceType");
            qDebug() << "Device device:" << typeReply;
            if (!typeReply.isNull()) {


                uint deviceType = typeReply.toUInt();
                // 2 indica un dispositivo WiFi
                if (deviceType == NM_DEVICE_TYPE_WIFI) {
                    qDebug() << "Found WiFi device:" << devicePath.path();
                    // Salva questo percorso per usi futuri (es. scansione)
                    m_wifiDevicePath = devicePath.path();
                    return;
                }
            }  else {

                qDebug() << "Failed to get device type for:" << devicePath.path();

            }
        }
        qWarning() << "No WiFi device found";
    } else {
        qWarning() << "Failed to get devices from NetworkManager";
    }
}


void WiFiManager::checkCurrentConnectionStatus()
{
    if (m_wifiDevicePath.isEmpty()) {
        qWarning() << "WiFi device path is empty. Unable to check connection status.";
        return;
    }

    QDBusInterface deviceIface("org.freedesktop.NetworkManager",
                               m_wifiDevicePath,
                               "org.freedesktop.NetworkManager.Device",
                               QDBusConnection::systemBus());


    QVariant stateVariant = deviceIface.property("State");
    if (stateVariant.isValid()) {
        uint state = stateVariant.toUInt();
        qDebug() << "Current WiFi device state:" << state;
        readConnectionStatus(state);
    }
}



void WiFiManager::setupPropertyChangedSignal()
{
    bool connected = QDBusConnection::systemBus().connect(
        "org.freedesktop.NetworkManager",      // Servizio
        m_wifiDevicePath,                      // Percorso dell'oggetto (Dispositivo WiFi)
        "org.freedesktop.DBus.Properties",     // Interfaccia generica per le propriet�
        "PropertiesChanged",                   // Nome del segnale
        this, SLOT(onPropertiesChanged(QString, QVariantMap, QStringList))  // Slot da chiamare
        );
    if (connected) {
        qDebug() << "Successfully connected to PropertiesChanged signal";
    } else {
        qDebug() << "Failed to connect to PropertiesChanged signal";
    }
}

void WiFiManager::readConnectionStatus(uint state)
{
    WiFiManager::ConnectionStatus status;
    switch (state) {
    case NM_DEVICE_STATE_DISCONNECTED:
        qDebug() << "Device is disconnected.";
        status = Disconnected;
        break;
    case NM_DEVICE_STATE_ACTIVATED:
        qDebug() << "Device is connected.";
        status = Connected;
        getConnectedSSID();
        break;
    case NM_DEVICE_STATE_PREPARE:
        qDebug() << "Il dispositivo si sta preparando per la connessione";
        status = Connecting;
        break;
    case NM_DEVICE_STATE_CONFIG:
    case NM_DEVICE_STATE_IP_CONFIG:
    case NM_DEVICE_STATE_IP_CHECK:
    case NM_DEVICE_STATE_SECONDARIES:
        qDebug() << "Il dispositivo è in fase di configurazione della connessione";
        status = Connecting;
        break;
    case NM_DEVICE_STATE_NEED_AUTH:
        qDebug() << "La connessione richiede autenticazione";
        status = WrongPassword;
        break;
    case NM_DEVICE_STATE_FAILED:
        qDebug() << "La connessione è fallita";
        status = ConnectionFailed;
        break;
    case NM_DEVICE_STATE_DEACTIVATING:
        qDebug() << "Il dispositivo si sta preparando per la disconnessione";
        status = StatusUnknown;
        break;
    case NM_DEVICE_STATE_UNKNOWN:
    case NM_DEVICE_STATE_UNMANAGED:
    case NM_DEVICE_STATE_UNAVAILABLE:
    default:
        qDebug() << "Device is in state:" << state;
        status = StatusUnknown;
        break;
    }

    setConnectionStatus(status);
}

void WiFiManager::setConnectionStatus(ConnectionStatus status) {
    if (m_connectionStatus != status) {
        m_connectionStatus = status;
        emit wifiStatusChanged(m_connectionStatus);  // Notifica il cambiamento
    }
}

void WiFiManager::getConnectedSSID()
{
    // Ottieni le connessioni attive da NetworkManager
    QDBusInterface nmIface("org.freedesktop.NetworkManager",
                           "/org/freedesktop/NetworkManager",
                           "org.freedesktop.NetworkManager",
                           QDBusConnection::systemBus());

    QVariant activeConnectionsVariant = nmIface.property("ActiveConnections");


     QList<QDBusObjectPath> activeConnections = qdbus_cast<QList<QDBusObjectPath>>(activeConnectionsVariant);
     if (!activeConnections.isEmpty()) {
        for (const QDBusObjectPath &connectionPath : activeConnections) {
            // Crea un'interfaccia per ogni connessione attiva
            QDBusInterface activeConnIface("org.freedesktop.NetworkManager",
                                           connectionPath.path(),
                                           "org.freedesktop.NetworkManager.Connection.Active",
                                           QDBusConnection::systemBus());

            // Verifica se la connessione � associata al dispositivo WiFi
            QVariant devicePathVariant = activeConnIface.property("Devices");
            if (devicePathVariant.isValid()) {
                QList<QDBusObjectPath> devices = qdbus_cast<QList<QDBusObjectPath>>(devicePathVariant);
                if (devices.contains(QDBusObjectPath(m_wifiDevicePath))) {
                    // Trova l'access point associato
                    QVariant apPathVariant = activeConnIface.property("SpecificObject");
                    qDebug() << "apPathVariant:" << apPathVariant;
                    if (apPathVariant.isValid()) {
                        QDBusObjectPath apPath = qdbus_cast<QDBusObjectPath>(apPathVariant);
                        QString apPathString  = apPath.path();
                        // Crea un'interfaccia per l'access point
                        QDBusInterface apIface("org.freedesktop.NetworkManager",
                                               apPathString,
                                               "org.freedesktop.NetworkManager.AccessPoint",
                                               QDBusConnection::systemBus());

                        // Ottieni l'SSID
                        QVariant ssidVariant = apIface.property("Ssid");
                        if (ssidVariant.isValid()) {
                            QByteArray ssidArray = ssidVariant.toByteArray();
                            QString ssid = QString::fromUtf8(ssidArray);
                            qDebug() << "Connected SSID:" << ssid;
                        } else {
                            qDebug() << "Failed to get SSID from access point:" << apIface.lastError().message();
                        }
                    }
                }
            }
        }
    } else {
      qDebug() << "activeConnections empty";
    }
}
