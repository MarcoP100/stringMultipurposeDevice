#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H


#include <QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusConnection>
#include <QQmlEngine>
#include <QJSEngine>


class WiFiManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(ConnectionStatus wifiStatus READ statusConnection NOTIFY wifiStatusChanged)
    //Q_PROPERTY(QString currentNetworkSsid READ getCurrentNetworkName NOTIFY ssidChanged)

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

        // Dichiarazione dell'operatore come amico
        friend QDebug operator<<(QDebug dbg, const NetworkEntry &entry);

    };

    explicit WiFiManager(QObject *parent = nullptr);
    static QObject* createSingletonInstance(QQmlEngine* engine, QJSEngine* scriptEngine);



    Q_INVOKABLE void scanNetworks();
    Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password);
    Q_INVOKABLE void disconnectNetwork();
    Q_INVOKABLE void checkCurrentConnectionStatus();
    //Q_INVOKABLE QString getConnectedSSID();


    bool isBusy() const;
    ConnectionStatus statusConnection() const;
    QString getCurrentNetworkName() const;



signals:

    void scanCompleted(const QVariantList &networks);
    void errorOccurred(const QString &error);
    void busyChanged(bool busy);
    void wifiStatusChanged(ConnectionStatus status);
    void ssidChanged(const QString &ssid);


private slots:
    void onPropertiesChanged(QString interfaceName, QVariantMap changedProperties, QStringList invalidatedProperties);
    void onScanFinished(QDBusPendingCallWatcher *watcher);
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
    void connectWithPassword(const QString &ssid, const QString &password, const QString &apPath);
    NetworkEntry dataNetworkKnown(const QString &ssid);


};

#endif // WIFIMANAGER_H
