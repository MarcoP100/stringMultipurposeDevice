#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H


#include <QObject>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusConnection>


class WiFiManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(ConnectionStatus wifiStatus READ statusConnection NOTIFY wifiStatusChanged)

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

    explicit WiFiManager(QObject *parent = nullptr);




    Q_INVOKABLE void scanNetworks();
    Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password);
    Q_INVOKABLE void disconnectNetwork();
    //Q_INVOKABLE QString getConnectedSSID();
    Q_INVOKABLE QStringList getSavedNetworks();

    bool isBusy() const;
    ConnectionStatus statusConnection() const;




signals:

    void scanCompleted(const QStringList &networks);
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);
    void busyChanged(bool busy);
    void wifiStatusChanged(ConnectionStatus status);


private slots:
    void onPropertiesChanged(QString interfaceName, QVariantMap changedProperties, QStringList invalidatedProperties);

private:
    void handleNetworkScanResult(const QList<QDBusObjectPath> &networks);
    void listNetworkDevices();
    void requestScan();
    void getAccessPoints();
    void checkCurrentConnectionStatus();
    void setupPropertyChangedSignal();
    void readConnectionStatus(uint state);
    void setConnectionStatus(ConnectionStatus status);
    void getConnectedSSID();
    ConnectionStatus m_connectionStatus = StatusUnknown;

    QString m_wifiDevicePath;

    bool m_busy;  // Flag per indicare se la scansione � in corso
    void setBusy(bool busy);
    bool scanForSavedNetworks = true;

    typedef QMap<QString, QVariantMap> Connection;
};

#endif // WIFIMANAGER_H
