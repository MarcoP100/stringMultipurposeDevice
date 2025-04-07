#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include "dynamometerdata.h"

class CanManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(CanState  can0State READ can0State NOTIFY canStateChanged)
    Q_PROPERTY(CanState  can1State READ can1State NOTIFY canStateChanged)

public:
    explicit CanManager(dynamometerData *dynoData, QObject *parent = nullptr);

    enum CanState{
        up = 0,
        down = 1,
        unknown = 2
    };
    Q_ENUM(CanState)

    enum Interface { Can0, Can1 };

    CanState  can0State() const;
    CanState  can1State() const;

    void sendCanMessage(uint32_t id, const QByteArray &data, const int socket);
    void startSending();  // Avvia il timer
    void stopSending();   // Ferma il timer
    bool isInitialized() const;

signals:
    void errorOccurred(const QString &error);
    void canStateChanged(Interface interface);

private slots:
    void sendPeriodicMessage();

private:
    int socketCan0 = -1;
    int socketCan1 = -1;
    QTimer *sendTimer;
    QTimer *m_stateChangeTimer;
    bool initialized = false;
    dynamometerData *m_dynoData;
    static const QMap<QChar, char> stateMap;
    CanState getCanState(const QString &interfaceName);
    void updateCanState(const QString &interfaceName, CanState &storedState);

    CanState  m_can0State = CanState::unknown;
    CanState  m_can1State = CanState::unknown;
};

#endif // CANMANAGER_H

