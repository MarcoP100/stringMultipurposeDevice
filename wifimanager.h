#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H


#include <QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusConnection>
#include <QQmlEngine>
#include <QJSEngine>
#include <QDBusMetaType>


typedef QMap<QString, QVariantMap> NestedMap;
Q_DECLARE_METATYPE(NestedMap)


class WiFiManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(ConnectionStatus wifiStatus READ statusConnection NOTIFY wifiStatusChanged)
    //Q_PROPERTY(QString currentNetworkSsid READ getCurrentNetworkName NOTIFY ssidChanged)

    Q_ENUMS(DeviceState)
    Q_ENUMS(DeviceStateReason)

public:
    enum ConnectionStatus {
        Connecting,
        Connected,
        WrongPassword,
        ConnectionFailed,
        Disconnected,
        StatusUnknown,
        Disconnecting
    };
    Q_ENUM(ConnectionStatus)


    struct NetworkEntry {
        QString ssid;
        QString id;
        QString path;
        QString uuid;

        // Dichiarazione dell'operatore come amico
        friend QDebug operator<<(QDebug dbg, const NetworkEntry &entry);

    };

    explicit WiFiManager(QObject *parent = nullptr);
    static QObject* createSingletonInstance(QQmlEngine* engine, QJSEngine* scriptEngine);
    static void registerDBusTypes();


    Q_INVOKABLE void scanNetworks();
    Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password, const bool &savePassword);
    Q_INVOKABLE void disconnectNetwork();
    Q_INVOKABLE void checkCurrentConnectionStatus();
    //Q_INVOKABLE QString getConnectedSSID();


    bool isBusy() const;
    ConnectionStatus statusConnection() const;
    QString getCurrentNetworkName() const;

    enum DeviceState {
        NM_DEVICE_STATE_UNKNOWN = 0,
        NM_DEVICE_STATE_UNMANAGED = 10,
        NM_DEVICE_STATE_UNAVAILABLE = 20,
        NM_DEVICE_STATE_DISCONNECTED = 30,
        NM_DEVICE_STATE_PREPARE = 40,
        NM_DEVICE_STATE_CONFIG = 50,
        NM_DEVICE_STATE_NEED_AUTH = 60,
        NM_DEVICE_STATE_IP_CONFIG = 70,
        NM_DEVICE_STATE_IP_CHECK = 80,
        NM_DEVICE_STATE_SECONDARIES = 90,
        NM_DEVICE_STATE_ACTIVATED = 100,
        NM_DEVICE_STATE_DEACTIVATING = 110,
        NM_DEVICE_STATE_FAILED = 120
    };

    enum DeviceStateReason {
        NM_DEVICE_STATE_REASON_NONE = 0,
        NM_DEVICE_STATE_REASON_UNKNOWN = 1,
        NM_DEVICE_STATE_REASON_NOW_MANAGED = 2,
        NM_DEVICE_STATE_REASON_NOW_UNMANAGED = 3,
        NM_DEVICE_STATE_REASON_CONFIG_FAILED = 4,
        NM_DEVICE_STATE_REASON_IP_CONFIG_UNAVAILABLE = 5,
        NM_DEVICE_STATE_REASON_IP_CONFIG_EXPIRED = 6,
        NM_DEVICE_STATE_REASON_NO_SECRETS = 7,
        NM_DEVICE_STATE_REASON_SUPPLICANT_DISCONNECT = 8,
        NM_DEVICE_STATE_REASON_SUPPLICANT_CONFIG_FAILED = 9,
        NM_DEVICE_STATE_REASON_SUPPLICANT_FAILED = 10,
        NM_DEVICE_STATE_REASON_SUPPLICANT_TIMEOUT = 11,
        NM_DEVICE_STATE_REASON_PPP_START_FAILED = 12,
        NM_DEVICE_STATE_REASON_PPP_DISCONNECT = 13,
        NM_DEVICE_STATE_REASON_PPP_FAILED = 14,
        NM_DEVICE_STATE_REASON_DHCP_START_FAILED = 15,
        NM_DEVICE_STATE_REASON_DHCP_ERROR = 16,
        NM_DEVICE_STATE_REASON_DHCP_FAILED = 17,
        NM_DEVICE_STATE_REASON_SHARED_START_FAILED = 18,
        NM_DEVICE_STATE_REASON_SHARED_FAILED = 19,
        NM_DEVICE_STATE_REASON_AUTOIP_START_FAILED = 20,
        NM_DEVICE_STATE_REASON_AUTOIP_ERROR = 21,
        NM_DEVICE_STATE_REASON_AUTOIP_FAILED = 22,
        NM_DEVICE_STATE_REASON_MODEM_BUSY = 23,
        NM_DEVICE_STATE_REASON_MODEM_NO_DIAL_TONE = 24,
        NM_DEVICE_STATE_REASON_MODEM_NO_CARRIER = 25,
        NM_DEVICE_STATE_REASON_MODEM_DIAL_TIMEOUT = 26,
        NM_DEVICE_STATE_REASON_MODEM_DIAL_FAILED = 27,
        NM_DEVICE_STATE_REASON_MODEM_INIT_FAILED = 28,
        NM_DEVICE_STATE_REASON_GSM_APN_FAILED = 29,
        NM_DEVICE_STATE_REASON_GSM_REGISTRATION_NOT_SEARCHING = 30,
        NM_DEVICE_STATE_REASON_GSM_REGISTRATION_DENIED = 31,
        NM_DEVICE_STATE_REASON_GSM_REGISTRATION_TIMEOUT = 32,
        NM_DEVICE_STATE_REASON_GSM_REGISTRATION_FAILED = 33,
        NM_DEVICE_STATE_REASON_GSM_PIN_CHECK_FAILED = 34,
        NM_DEVICE_STATE_REASON_FIRMWARE_MISSING = 35,
        NM_DEVICE_STATE_REASON_REMOVED = 36,
        NM_DEVICE_STATE_REASON_SLEEPING = 37,
        NM_DEVICE_STATE_REASON_CONNECTION_REMOVED = 38,
        NM_DEVICE_STATE_REASON_USER_REQUESTED = 39,
        NM_DEVICE_STATE_REASON_CARRIER = 40,
        NM_DEVICE_STATE_REASON_CONNECTION_ASSUMED = 41,
        NM_DEVICE_STATE_REASON_SUPPLICANT_AVAILABLE = 42,
        NM_DEVICE_STATE_REASON_MODEM_NOT_FOUND = 43,
        NM_DEVICE_STATE_REASON_BT_FAILED = 44,
        NM_DEVICE_STATE_REASON_GSM_SIM_NOT_INSERTED = 45,
        NM_DEVICE_STATE_REASON_GSM_SIM_PIN_REQUIRED = 46,
        NM_DEVICE_STATE_REASON_GSM_SIM_PUK_REQUIRED = 47,
        NM_DEVICE_STATE_REASON_GSM_SIM_WRONG = 48,
        NM_DEVICE_STATE_REASON_INFINIBAND_MODE = 49,
        NM_DEVICE_STATE_REASON_DEPENDENCY_FAILED = 50,
        NM_DEVICE_STATE_REASON_BR2684_FAILED = 51,
        NM_DEVICE_STATE_REASON_MODMAN_UNSUPPORTED = 52,
        NM_DEVICE_STATE_REASON_MODEM_FAILED = 53,
        NM_DEVICE_STATE_REASON_SSID_NOT_FOUND = 54,
        NM_DEVICE_STATE_REASON_SECONDARY_CONNECTION_FAILED = 55,
        NM_DEVICE_STATE_REASON_DCB_FCOE_FAILED = 56,
        NM_DEVICE_STATE_REASON_TEAMD_CONTROL_FAILED = 57,
        NM_DEVICE_STATE_REASON_MODEM_FORCE_STOP = 58,
        NM_DEVICE_STATE_REASON_MODMAN_FAILED = 59,
        NM_DEVICE_STATE_REASON_MODEM_NO_SIGNAL = 60,
        NM_DEVICE_STATE_REASON_SIM_PIN_INCORRECT = 61,
        NM_DEVICE_STATE_REASON_NEW_ACTIVATION = 62,

    };


signals:

    void scanCompleted(const QVariantList &networks);
    void errorOccurred(const QString &error);
    void busyChanged(bool busy);
    void wifiStatusChanged(ConnectionStatus status);
    void ssidChanged(const QString &ssid);
    void reasonChangeState(const uint &newState, const uint &reason);


private slots:
    void onPropertiesChanged(QString interfaceName, QVariantMap changedProperties, QStringList invalidatedProperties);
    void onNetworkFound(QDBusPendingCallWatcher *watcher);
    void onScanFinished();
    void onDeviceStateChanged(uint newState, uint oldState, uint reason);
    void onScanTimeout();

private:

     struct NetworkInfo {
        QString ssid;
        bool requiresPassword;
        bool isSaved;

        // Definizione dell'operatore di uguaglianza
        bool operator==(const NetworkInfo &other) const {
            return ssid == other.ssid &&
                   requiresPassword == other.requiresPassword &&
                   isSaved == other.isSaved;
        }
    };

    struct NetworkPaths {
        QString apPath;
        QString connectionPath;
    };


    QList<NetworkEntry> m_savedNetworks;
    QList<NetworkEntry> getSavedNetworks();

    QVariantMap toVariantMap(const NetworkInfo &network);
    void handleNetworkScanResult(const QList<QDBusObjectPath> &networks);
    void listNetworkDevices();
    void requestScan();
    bool isNetworkKnown(const QString &ssid);
    void getAccessPoints();

    void setupPropertyChangedSignal();
    void setupStatusChangedSignal();
    void readConnectionStatus(uint state);
    void setConnectionStatus(ConnectionStatus status);
    QString getConnectedSSID();
    NetworkPaths getConnectedNetworkPath();
    void setCurrentNetworkName(const QString &ssid);

    ConnectionStatus m_connectionStatus = StatusUnknown;

    QString m_wifiDevicePath;

    bool m_scanCompletionTimer ;


    bool m_busy = false;  // Flag per indicare se la scansione � in corso
    void setBusy(bool busy);
    bool scanForSavedNetworks = true;

    typedef QMap<QString, QVariantMap> Connection;

    QTimer *m_scanTimeoutTimer = nullptr; // Timer per gestire il timeout della scansione
    QString m_connectedSsid = "";

    //connection
    void connectNoPassword(const QString &ssid, const QString &apPath);
    void connectWithPassword(const QString &ssid, const QString &apPath, const QString &password, const bool &savePassword);
    NetworkEntry dataNetworkKnown(const QString &ssid);
    void checkConnectionFailureReason();

    QTimer *m_emitDelayTimer;  // Timer per il ritardo nell'emissione lista reti trovate


};

#endif // WIFIMANAGER_H
