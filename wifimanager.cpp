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

    m_scanTimeoutTimer = new QTimer(this);
    m_scanTimeoutTimer->setSingleShot(true); // Timer che scatta una volta sola
    qDebug() << "inizializzazione m_scan";
    // Configura il timer per gestire il timeout della scansione
    connect(m_scanTimeoutTimer, &QTimer::timeout, this, &WiFiManager::onScanTimeout);
    qDebug() << "Connect per m_scanTimeoutTimer fatto.";


    //timer per scan network
    m_emitDelayTimer = new QTimer(this);
    m_emitDelayTimer->setInterval(5000);  // 5 secondi di ritardo
    m_emitDelayTimer->setSingleShot(true);  // Timer a colpo singolo

    connect(m_emitDelayTimer, &QTimer::timeout, this, &WiFiManager::onScanFinished);


    listNetworkDevices();
    //checkCurrentConnectionStatus();
    setupPropertyChangedSignal();
    setupStatusChangedSignal();



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


QVariantMap WiFiManager::toVariantMap(const NetworkInfo &network) {
    QVariantMap networkInfo;
    networkInfo["ssid"] = network.ssid;
    networkInfo["requiresPassword"] = network.requiresPassword;
    networkInfo["networkKnown"] = network.isSaved;
    return networkInfo;
}


WiFiManager::ConnectionStatus WiFiManager::statusConnection() const
{
    return m_connectionStatus;
}


void WiFiManager::scanNetworks()
{
    if (m_busy) {
        qDebug() << "Scansione gi� in corso";
        return;
    }

    setBusy(true);

    //if (scanForSavedNetworks){
        m_savedNetworks = getSavedNetworks();

    //}
    requestScan();
}

void WiFiManager::requestScan()
{
    QDBusInterface iface("org.freedesktop.NetworkManager",
                         m_wifiDevicePath,
                         "org.freedesktop.NetworkManager.Device.Wireless",
                         QDBusConnection::systemBus());

    if (!iface.isValid()) {
        qWarning() << "Interfaccia D-Bus non valida per avviare la scansione";
        setBusy(false);
        return;
    }
    QVariantMap options;
    QDBusPendingReply<void> reply = iface.asyncCall("RequestScan", options);
    // Richiedi una scansione delle reti disponibili
    //QVariantMap options;
    //QDBusMessage response = iface.call("RequestScan", QVariant::fromValue(options));

    /*if (!response.errorMessage().isEmpty()) {
        qDebug() << "Failed to initiate network scan:" << response.errorMessage();
    } else {
        qDebug() << "Network scan command sent successfully.";
    }
    qDebug() << "Request scan";*/

    if (reply.isError()) {
        qWarning() << "Errore durante la richiesta di scansione:" << reply.error().message();
        setBusy(false);
        return;
    }
    qDebug() << "Scansione avviata (asincrona).";

    // Avvia il timer per il timeout della scansione (ad esempio 10 secondi)
    m_scanTimeoutTimer->start(10000);

    // Utilizza QDBusPendingCallWatcher per monitorare quando la chiamata � terminata
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &WiFiManager::onNetworkFound);


    // Forza una chiamata a `getAccessPoints()` dopo un breve ritardo
    //QTimer::singleShot(30000, this, &WiFiManager::getAccessPoints);

}

void WiFiManager::onScanTimeout() {
    qWarning() << "Timeout della scansione WiFi raggiunto. La scansione non � stata completata in tempo.";

    // Interrompi lo stato di scansione
    setBusy(false);

    // Qui puoi gestire la UI per informare l'utente che la scansione non � andata a buon fine
    emit scanCompleted(QVariantList()); // Emetti una lista vuota per indicare che non sono state trovate reti
}



// Slot che viene chiamato quando la scansione � terminata
void WiFiManager::onNetworkFound(QDBusPendingCallWatcher *watcher) {

    // Ferma il timer poich� la scansione � completata
    if (m_scanTimeoutTimer->isActive()) {
        m_scanTimeoutTimer->stop();
    }

    qDebug() << "onScanFinished chiamato. Risposta da D-Bus:" << watcher->isFinished();



    // Gestisci l'eventuale errore
    QDBusPendingReply<void> reply = *watcher;
    if (reply.isError()) {
        qWarning() << "Errore durante la scansione WiFi:" << reply.error().message();
        setBusy(false);
        watcher->deleteLater();
        return;
    }

    // Riavvia il timer per emettere la lista
    m_emitDelayTimer->start();

    watcher->deleteLater();
}

void WiFiManager::onScanFinished() {
    qDebug() << "Scansione completata.";

    getAccessPoints();

}

void WiFiManager::onPropertiesChanged(QString interfaceName, QVariantMap changedProperties, QStringList invalidatedProperties)
{
    ;//qDebug() << "PropertiesChanged signal received for interface:" << interfaceName;
    // Verifica se l'interfaccia � quella giusta
    /*if (interfaceName == "org.freedesktop.NetworkManager.Device.Wireless") {
        //qDebug() << "Property changed: " << changedProperties;
        // Controlla se la propriet� "AccessPoints" � tra quelle modificate
        if (changedProperties.contains("AccessPoints")) {
            qDebug() << "Access points property changed, getting updated list of networks.";
            // Chiama GetAccessPoints per ottenere l'elenco delle reti aggiornato
            getAccessPoints();

            // Reset del timer per aspettare ulteriori cambiamenti
            if (!m_scanCompletionTimer) {
                m_scanCompletionTimer = new QTimer(this);
                connect(m_scanCompletionTimer, &QTimer::timeout, this, &WiFiManager::onScanTimeout);
                m_scanCompletionTimer->setSingleShot(true);
            }
            m_scanCompletionTimer->start(3000);  // Aspetta 3 secondi dopo l'ultimo cambiamento
        }
    }else if (interfaceName == "org.freedesktop.NetworkManager.Device"){
        if (changedProperties.contains("State")) {
            uint newState = changedProperties.value("State").toUInt();
            readConnectionStatus(newState);
        }
    }*/
}



void WiFiManager::getAccessPoints()
{
    // Chiamata per ottenere i punti di accesso (reti Wi-Fi)
    QDBusInterface iface("org.freedesktop.NetworkManager",
                         m_wifiDevicePath,
                         "org.freedesktop.NetworkManager.Device.Wireless",
                         QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath>> apReply = iface.call("GetAllAccessPoints");
    if (apReply.isValid()) {
        QList<QDBusObjectPath> accessPoints = apReply.value();
        // Stampa l'elenco completo dei percorsi degli access points
        qDebug() << "Number of access points found:" << accessPoints.size();
        handleNetworkScanResult(accessPoints);
    } else {
        emit errorOccurred("Failed to get access points");
        setBusy(false);

    }


}



void WiFiManager::handleNetworkScanResult(const QList<QDBusObjectPath> &networks)
{
    QList<NetworkInfo> networkList;
    qDebug() << "Handle";
    for (const QDBusObjectPath &networkPath : networks) {
        QDBusInterface apIface("org.freedesktop.NetworkManager",
                               networkPath.path(),
                               "org.freedesktop.NetworkManager.AccessPoint",
                               QDBusConnection::systemBus());

        QVariant ssidReply = apIface.property("Ssid");
        QVariant wpaFlagsReply = apIface.property("Flags");

        if (ssidReply.isValid() && !ssidReply.isNull()) {
            QString ssid = ssidReply.toString();

            if (!ssid.isEmpty()) {
                NetworkInfo networkInfo;
                networkInfo.ssid = ssid;
                networkInfo.requiresPassword = wpaFlagsReply.isValid() && wpaFlagsReply.toInt() != 0;  // Se WpaFlags > 0, la rete � protetta
                networkInfo.isSaved = isNetworkKnown(ssid);

                if (!networkList.contains(networkInfo)) {
                    qDebug() << "Found SSID:" << ssid << "Protected:" << networkInfo.requiresPassword << "- Saved: " << networkInfo.isSaved;
                    networkList.append(networkInfo);

                } else {
                    qDebug() << "Duplicate SSID found, ignoring:" << ssid;
                }
            } else {
                qDebug() << "Empty SSID for access point:" << networkPath.path();
            }
        } else {
            qDebug() << "Failed to get SSID for access point:" << networkPath.path();
        }
    }
    qDebug() << "Reti trovate:" << networkList.size();

    QVariantList variantNetworks;
    for (const NetworkInfo &network : networkList) {
        variantNetworks.append(toVariantMap(network));
    }

    emit scanCompleted(variantNetworks);
    setBusy(false);

}

bool WiFiManager::isNetworkKnown(const QString &ssid) {
    for (const NetworkEntry &network  : m_savedNetworks) {
        if (network.ssid == ssid){
            return true;
        }
    }
    return false;
}

WiFiManager::NetworkEntry WiFiManager::dataNetworkKnown(const QString &ssid) {
    NetworkEntry selNetwork = {};
    for (const NetworkEntry &network  : m_savedNetworks) {
        if (network.ssid == ssid){
            selNetwork = network;
            break;
        }
    }
    return selNetwork;
}



void WiFiManager::connectToNetwork(const QString &ssid, const QString &password, const bool &savePassword)
{
    QString connectedSsid = getConnectedSSID().trimmed();  // Elimina spazi e caratteri non visibili
    QString sanitizedSsid = ssid.trimmed();               // Elimina spazi e caratteri non visibili

    // Verifica se siamo gi� connessi alla rete specificata
    if (QString::compare(connectedSsid, sanitizedSsid, Qt::CaseInsensitive) == 0) {
        qDebug() << "Gi� connesso alla rete:" << connectedSsid << " = " << sanitizedSsid;
        return;
    } else {
        qDebug() << "SSID differenti:" << connectedSsid << " != " << sanitizedSsid;
    }

    qDebug() << "Tentativo di connessione alla rete SSID:" << ssid;

    // Ottieni i punti di accesso disponibili per il dispositivo WiFi
    QDBusInterface wifiDeviceIface("org.freedesktop.NetworkManager",
                                   m_wifiDevicePath,
                                   "org.freedesktop.NetworkManager.Device.Wireless",
                                   QDBusConnection::systemBus());

    QDBusReply<QList<QDBusObjectPath>> accessPointsReply = wifiDeviceIface.call("GetAccessPoints");
    if (!accessPointsReply.isValid()) {
        // Stampa un messaggio di errore se non � possibile ottenere i punti di accesso
        qWarning() << "Impossibile ottenere i punti di accesso:" << accessPointsReply.error().message();
        return;
    }

    qDebug() << "Punti di accesso trovati:" << accessPointsReply.value().size();

    // Cerca il punto di accesso che corrisponde all'SSID specificato
    QString apPath;
    for (const QDBusObjectPath &ap : accessPointsReply.value()) {
        QDBusInterface apIface("org.freedesktop.NetworkManager",
                               ap.path(),
                               "org.freedesktop.NetworkManager.AccessPoint",
                               QDBusConnection::systemBus());
        QString apSsid = apIface.property("Ssid").toString();
        // Confronta l'SSID del punto di accesso con l'SSID desiderato
        qDebug() << "Controllando SSID del punto di accesso:" << apSsid;
        if (apSsid == ssid) {
            apPath = ap.path();
            break;
        }
    }

    // Se non viene trovato alcun punto di accesso corrispondente, stampa un avviso ed esci
    if (apPath.isEmpty()) {
        qWarning() << "Punto di accesso non trovato per SSID:" << ssid;
        return;
    }

    qDebug() << "Punto di accesso trovato per SSID:" << ssid;

    setCurrentNetworkName(ssid);
    // verifico se connettere con paswword o senza
    if (password == ""){
        connectNoPassword(ssid, apPath);
    } else {
        connectWithPassword(ssid, apPath, password, savePassword);
    }

}

void WiFiManager::connectNoPassword(const QString &ssid, const QString &apPath){

    NetworkEntry network = {};

    // Crea un'interfaccia per attivare la connessione utilizzando D-Bus
    QDBusInterface nmIfaceActivate("org.freedesktop.NetworkManager",
                                   "/org/freedesktop/NetworkManager",
                                   "org.freedesktop.NetworkManager",
                                   QDBusConnection::systemBus());

    qDebug() << "Tentativo di attivare la connessione salvata per SSID:" << ssid;

    // cerco l'ssid tra i network salvati
    m_savedNetworks = getSavedNetworks();
    // Determino il percorso della connessione
    QString connectionPath = "/";
    if (isNetworkKnown(ssid)) {
        network = dataNetworkKnown(ssid);
        connectionPath = network.path;  // Usa il percorso salvato per la rete
    }

        QDBusReply<QDBusObjectPath> activeConnectionReply = nmIfaceActivate.call(
            "ActivateConnection",
            QDBusObjectPath(connectionPath),
            QDBusObjectPath(m_wifiDevicePath),
            QDBusObjectPath(apPath));

        if (!activeConnectionReply.isValid()) {
            qWarning() << "Errore durante l'attivazione della connessione:" << activeConnectionReply.error().message();
            //emit connectionFailed(ssid, "Errore durante l'attivazione");
        } else {
            qDebug() << "Connessione avviata con successo per SSID:" << ssid;
            m_connectedSsid = ssid;
            //emit connectionSuccessful(ssid);
        }


}


void WiFiManager::connectWithPassword(const QString &ssid, const QString &apPath, const QString &password, const bool &savePassword){

    NetworkEntry network = {};

    QVariantMap wirelessSettings;
    wirelessSettings["ssid"] = ssid.toUtf8();
    wirelessSettings["mode"] = "infrastructure";
    qDebug() << "Qmap 1 ottenuto:" << wirelessSettings;

    QVariantMap wirelessSecuritySettings;
    wirelessSecuritySettings["key-mgmt"] = "wpa-psk";
    wirelessSecuritySettings["psk"] = password;
    qDebug() << "Qmap 2 ottenuto:" << wirelessSecuritySettings;

    QVariantMap connectionMap;
    connectionMap["id"] = ssid;
    if (savePassword){
        m_savedNetworks = getSavedNetworks();
        if (isNetworkKnown(ssid)) {
            network = dataNetworkKnown(ssid);
            connectionMap["id"] = network.id;  // Usa il percorso salvato per la rete
            if (!network.uuid.isEmpty()) {
                connectionMap["uuid"] = network.uuid;
            }
        }
    }

    connectionMap["type"] = "802-11-wireless";
    qDebug() << "Qmap 3 ottenuto:" << connectionMap;

    // Crea la configurazione annidata direttamente
    QMap<QString, QVariantMap> connectionSettings;
    // Inserisci la sezione "connection" nella mappa principale
    connectionSettings.insert("connection", connectionMap);
    // Inserisci la sezione "802-11-wireless" nella mappa principale
    connectionSettings.insert("802-11-wireless", wirelessSettings);
    // Inserisci la sezione "802-11-wireless-security" nella mappa principale
    connectionSettings.insert("802-11-wireless-security", wirelessSecuritySettings);

    // Converte la mappa annidata per il metodo AddAndActivateConnection2
    QVariant finalConnectionSettings = QVariant::fromValue(connectionSettings);

    qDebug() << "Qmap ottenuto:" << connectionSettings;
    // Opzioni per una connessione temporanea
    QVariantMap options;
    if (savePassword) {  // Controlla lo stato della spunta
        options["persist"] = "disk";  // Salva la connessione
       qDebug() << "Password da salvare";
    } else {
        options["persist"] = "volatile";  // Connessione temporanea
    }



    // Chiamata a AddAndActivateConnection2
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus()
        );

    QDBusMessage reply = nmInterface.call("AddAndActivateConnection2",
                                          (finalConnectionSettings),  // Connessione
                                          QDBusObjectPath(m_wifiDevicePath),              // Dispositivo
                                          QDBusObjectPath(apPath),                     // Specific Object
                                          QVariant::fromValue(options));            // Opzioni

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Errore nella connessione:" << reply.errorMessage();
    } else {
        QList<QVariant> arguments = reply.arguments();
        if (arguments.size() >= 2) {
            QString activeConnectionPath = arguments[1].value<QDBusObjectPath>().path();
            qDebug() << "Connessione attiva al path:" << activeConnectionPath;
        }
            m_connectedSsid = ssid;
            //emit connectionSuccessful(ssid);
        }
    }




void WiFiManager::disconnectNetwork()
{

    QString connectionPathString = getConnectedNetworkPath().connectionPath;
    qDebug() << "Inizio disattivazione: " << connectionPathString;
    QDBusInterface nmInterface(
        "org.freedesktop.NetworkManager",
        "/org/freedesktop/NetworkManager",
        "org.freedesktop.NetworkManager",
        QDBusConnection::systemBus()
        );

    nmInterface.call("DeactivateConnection", QDBusObjectPath(connectionPathString));
    qDebug() << "Richiesta disattivazione:" << connectionPathString;

}


QList<WiFiManager::NetworkEntry> WiFiManager::getSavedNetworks()
{
    QList<NetworkEntry> savedNetworks;
    QMap<QString, QList<NetworkEntry>> ssidMap;

    qDebug() << "Inizio ricerca delle reti salvate";

    QDBusInterface iface("org.freedesktop.NetworkManager",
                         "/org/freedesktop/NetworkManager/Settings",
                         "org.freedesktop.NetworkManager.Settings",
                         QDBusConnection::systemBus());

    // Chiamata per ottenere tutte le connessioni salvate
    QDBusReply<QList<QDBusObjectPath>> reply = iface.call("ListConnections");
    if (!reply.isValid()) {
        qDebug() << "Errore durante la lista delle connessioni salvate:" << reply.error().message();
        return savedNetworks;
    }


    for (const QDBusObjectPath &connectionPath  : reply.value()) {
        QDBusInterface connIface("org.freedesktop.NetworkManager",
                                 connectionPath.path(),
                                 "org.freedesktop.NetworkManager.Settings.Connection",
                                 QDBusConnection::systemBus());

        // Ottieni le impostazioni della connessione
        QDBusMessage message = connIface.call("GetSettings");
        if (message.type() != QDBusMessage::ReplyMessage || message.arguments().isEmpty()) {
            qDebug() << "Impossibile ottenere le impostazioni per la connessione:" << connectionPath.path();
            qDebug() << "Error:" << message.errorMessage();
            continue;
        }

        QVariant argument = message.arguments().first();
        QMap<QString, QVariantMap> settings;
        const QDBusArgument &dbusArg = argument.value<QDBusArgument>();
        dbusArg >> settings;  // Estrai i dati da QDBusArgument

        // Stampa il contenuto di settings per capire cosa contiene
        //qDebug() << "Settings QVariantMap:" << settings;

        // Controlla se la connessione � di tipo Wi-Fi
        if (!settings.contains("802-11-wireless")) {
            qDebug() << "La connessione non � di tipo Wi-Fi. Ignorata:" << connectionPath.path();
            continue;
        }

        QVariantMap wifiSettings = settings["802-11-wireless"];
        QByteArray ssidArray = wifiSettings.value("ssid").toByteArray();
        QString ssid = QString::fromUtf8(ssidArray);
        if (ssid.isEmpty() || ssid.isNull()) {
            qDebug() << "SSID vuoto o nullo, ignorato";
            continue;
        }

        QVariantMap connectionSettings = settings["connection"];
        QString id = connectionSettings.value("id").toString();
        QString uuid = connectionSettings.value("uuid").toString();
        QString path = connectionPath.path();

        NetworkEntry entry = { ssid, id, path, uuid };

        if (ssidMap.contains(ssid)) {
            ssidMap[ssid].append(entry); // Duplicato
            qDebug() << "SSID duplicato aggiunto alla mappa:" << ssid;
        } else {
            ssidMap.insert(ssid, QList<NetworkEntry>{entry});
            qDebug() << "SSID nuovo aggiunto alla mappa:" << ssid;
        }
    }


    // Converte la mappa con i duplicati in una lista piatta per uso semplificato
    savedNetworks.clear();
    for (const QList<NetworkEntry> &entries : ssidMap) {
        savedNetworks.append(entries);
    }
    scanForSavedNetworks = false;
    qDebug() << "Totale reti salvate:" << savedNetworks.size();
    qDebug() << "Reti salvate trovate:" << savedNetworks;
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


void WiFiManager::setupStatusChangedSignal()
{
    bool connected = QDBusConnection::systemBus().connect(
        "org.freedesktop.NetworkManager",      // Servizio
        m_wifiDevicePath,                      // Percorso dell'oggetto (Dispositivo WiFi)
        "org.freedesktop.NetworkManager.Device",     // Interfaccia generica per le propriet�
        "StateChanged",                   // Nome del segnale
        this, SLOT(onDeviceStateChanged(uint, uint, uint))  // Slot da chiamare
        );
    if (connected) {
        qDebug() << "Successfully connected to StateChanged signal";
    } else {
        qDebug() << "Failed to connect to StateChanged signal";
    }
}

void WiFiManager::onDeviceStateChanged(uint newState, uint oldState, uint reason) {
    qDebug() << "Stato cambiato da" << oldState << "a" << newState << "Motivo:" << reason;

    readConnectionStatus(newState);
    emit reasonChangeState(newState, reason);
}


void WiFiManager::readConnectionStatus(uint state)
{
    QString ssid;
    WiFiManager::ConnectionStatus status;
    switch (state) {
    case NM_DEVICE_STATE_DISCONNECTED:
        qDebug() << "Device is disconnected.";
        status = Disconnected;
        ssid = "";
        break;
    case NM_DEVICE_STATE_ACTIVATED:
        qDebug() << "Device is connected. Icon green";
        status = Connected;
        ssid = getConnectedSSID();
        break;
    case NM_DEVICE_STATE_PREPARE:
        qDebug() << "Il dispositivo si sta preparando per la connessione";
        status = Connecting;
        ssid = "";
        break;
    case NM_DEVICE_STATE_CONFIG:
    case NM_DEVICE_STATE_IP_CONFIG:
    case NM_DEVICE_STATE_IP_CHECK:
    case NM_DEVICE_STATE_SECONDARIES:
        qDebug() << "Il dispositivo è in fase di configurazione della connessione";
        status = Connecting;
        ssid = "";
        break;
    case NM_DEVICE_STATE_NEED_AUTH:
        qDebug() << "La connessione richiede autenticazione";
        status = WrongPassword;
        ssid = "";
        break;
    case NM_DEVICE_STATE_FAILED:
        qDebug() << "La connessione è fallita";
        status = ConnectionFailed;
        ssid = "";
        checkConnectionFailureReason();
        break;
    case NM_DEVICE_STATE_DEACTIVATING:
        qDebug() << "Il dispositivo si sta preparando per la disconnessione";
        status = StatusUnknown;
        ssid = "";
        break;
    case NM_DEVICE_STATE_UNKNOWN:
    case NM_DEVICE_STATE_UNMANAGED:
    case NM_DEVICE_STATE_UNAVAILABLE:
    default:
        qDebug() << "Device is in state:" << state;
        status = StatusUnknown;
        ssid = "";
        break;
    }

    setConnectionStatus(status);
    setCurrentNetworkName(ssid);
}

void WiFiManager::setConnectionStatus(ConnectionStatus status) {
    if (m_connectionStatus != status) {
        m_connectionStatus = status;
        emit wifiStatusChanged(m_connectionStatus);  // Notifica il cambiamento
    }
}


void WiFiManager::checkConnectionFailureReason() {
    QDBusInterface deviceInterface(
        "org.freedesktop.NetworkManager",
        m_wifiDevicePath,
        "org.freedesktop.DBus.Properties",
        QDBusConnection::systemBus()
        );

    QDBusMessage reply = deviceInterface.call("Get", "org.freedesktop.NetworkManager.Device", "StateReason");
    qDebug() << "reply:" << reply;
    if (reply.type() == QDBusMessage::ReplyMessage) {
        QList<QVariant> arguments = reply.arguments();
        qDebug() << "arguments:" << arguments;
        QVariant stateReasonVariant = reply.arguments().first();
        qDebug() << "stateReasonVariant:" << stateReasonVariant;
        /*QDBusArgument dbusArg = stateReasonVariant.value<QDBusArgument>();
            uint state = 0, reason = 0;
            dbusArg.beginStructure();
            dbusArg >> state >> reason;
            dbusArg.endStructure();

            qDebug() << "Stato:" << state << ", Motivo:" << reason;*/

    } else {
        qDebug() << "Errore nel recuperare StateReason:" << reply.errorMessage();
    }

}




WiFiManager::NetworkPaths WiFiManager::getConnectedNetworkPath(){

    NetworkPaths paths;
    paths.apPath = "";
    paths.connectionPath = "";

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
                        paths.apPath = apPath.path();
                        paths.connectionPath = connectionPath.path();
                        return paths;  // Esci dopo aver trovato il Wi-Fi attivo

                    }
                }
            }
        }
    } else {
        qDebug() << "activeConnections empty";
    }
    return paths;
}


QString WiFiManager::getConnectedSSID()
{
    QString ssid = "";
    QString apPathString = getConnectedNetworkPath().apPath;

    if (apPathString != ""){
        // Crea un'interfaccia per l'access point
        QDBusInterface apIface("org.freedesktop.NetworkManager",
                               apPathString,
                               "org.freedesktop.NetworkManager.AccessPoint",
                               QDBusConnection::systemBus());

        // Ottieni l'SSID
        QVariant ssidVariant = apIface.property("Ssid");
        if (ssidVariant.isValid()) {
            QByteArray ssidArray = ssidVariant.toByteArray();
            ssid = QString::fromUtf8(ssidArray);
            qDebug() << "Connected SSID:" << ssid;
        } else {
            qDebug() << "Failed to get SSID from access point:" << apIface.lastError().message();
        }
    }
    return ssid;
}

void WiFiManager::setCurrentNetworkName(const QString &ssid){
    if (ssid != m_connectedSsid){
        m_connectedSsid = ssid;
        emit  ssidChanged(m_connectedSsid);
    }
    return;

}


// Definizione dell'operatore per qDebug
QDebug operator<<(QDebug dbg, const WiFiManager::NetworkEntry &entry) {
    dbg.nospace() << "{ssid: " << entry.ssid
                  << ", id: " << entry.id
                  << ", path: " << entry.path << "}";
    return dbg.space();
}

void WiFiManager::registerDBusTypes() {
    qDBusRegisterMetaType<NestedMap>();
}
