#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include "dynamometerdata.h"

class CanManager : public QObject
{
    Q_OBJECT
public:
    explicit CanManager(dynamometerData *dynoData, QObject *parent = nullptr);
    void sendCanMessage(uint32_t id, const QByteArray &data);
    void startSending();  // Avvia il timer
    void stopSending();   // Ferma il timer
    bool isInitialized() const;

signals:
    void errorOccurred(const QString &error);

private slots:
    void sendPeriodicMessage();

private:
    int socketCan;
    QTimer *sendTimer;
    bool initialized = false;
    dynamometerData *m_dynoData;
    static const QMap<QChar, char> stateMap;
};

#endif // CANMANAGER_H

