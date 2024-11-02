#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QTimer>  // Aggiungi l'inclusione del timer

class WiFiManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)

public:
    explicit WiFiManager(QObject *parent = nullptr);

    Q_INVOKABLE void getAvailableNetworks();
    Q_INVOKABLE void stopNetworkScan();       // Funzione per fermare la ricerca delle reti
    Q_INVOKABLE void connectToNetwork(const QString &ssid, const QString &password);

    bool isBusy() const;

    ~WiFiManager();

signals:
    void availableNetworksChanged(QStringList networks);
    void busyChanged(bool busy);
    void connectionStatusChanged(QString status);

private slots:
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);  // Slot per gestire la fine del processo
    void handleReadyRead();

private:
    QStringList parseNetworks(const QString &nmcliOutput);
    QProcess process;  // Oggetto QProcess per gestire la ricerca delle reti
    //QTimer timeoutTimer;  // Timer per il timeout del processo
    //void onTimeout();

    bool m_busy;  // Flag per indicare se la scansione � in corso
    void setBusy(bool busy);

};

#endif // WIFIMANAGER_H
