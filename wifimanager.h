#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QTimer>  // Aggiungi l'inclusione del timer
#include <QList>

class WiFiManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    // Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionStatusChanged)

public:
    explicit WiFiManager(QObject *parent = nullptr);

    Q_INVOKABLE void scanNetworks();
    Q_INVOKABLE void stopNetworkScan();       // Funzione per fermare la ricerca delle reti
    Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password);
    Q_INVOKABLE bool isConnected();
    Q_INVOKABLE QString getConnectedSSID();

    // Aggiungi la funzione per disconnettersi
    Q_INVOKABLE void disconnectNetwork();

    bool isBusy() const;

    ~WiFiManager();

    enum ConnectionStatus {
        ErrorProcessStart,
        Timeout,
        Connecting,
        Connected,
        WrongPassword,
        ConnectionFailed,
        SsidEmpty,
        Disconnected,
        ErrorDisconnection
    };
    Q_ENUM(ConnectionStatus)



signals:
    void availableNetworksChanged(const QVariantList &networks);
    void busyChanged(bool busy);
    void connectionStatusChanged(WiFiManager::ConnectionStatus status);
    void savedNetworksChanged(const QString &status);

private slots:
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);  // Slot per gestire la fine del processo
    void handleConnectionFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleSavedNetworksFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleReadyRead();

    void handleConnectionTimeout();
    void handleSavedNetworksTimeout();

private:
    struct NetworkInfo {
        QString ssid;
        bool requiresPassword;
        bool isSaved;
    };

    void emitConnectionStatus(ConnectionStatus status);
    void startScanNetworks();
    void getSavedNetworks();
    void runIwlistCommand();
    QList<NetworkInfo> parseNetworks(const QString &iwlistOutput);
    QProcess scanProcess;  // Oggetto QProcess per gestire la ricerca delle reti
    QProcess connectProcess;
    QProcess savedNetworksProcess;
    bool connectionKilled;
    bool isNetworkSaved(const QString &ssid);
    bool scanForSavedNetworks = true;

    //QTimer timeoutTimer;  // Timer per il timeout del processo
    //void onTimeout();

    bool m_busy;  // Flag per indicare se la scansione � in corso
    void setBusy(bool busy);
    QVariantMap toVariantMap(const NetworkInfo &network);
    QStringList savedNetworks;
    const int CONNECT_TIMEOUT_MS = 30000;

    QTimer connectionTimer;
    QTimer scanNetworksTimer;
    QTimer knownNetworksTimer;


};

#endif // WIFIMANAGER_H
