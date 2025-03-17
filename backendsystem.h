#ifndef BACKENDSYSTEM_H
#define BACKENDSYSTEM_H

#include <QObject>
#include <QProcess>

class BackendSystem : public QObject {
    Q_OBJECT
public:
    explicit BackendSystem(QObject *parent = nullptr);

    Q_INVOKABLE void shutdownSystem();

    Q_INVOKABLE void rebootSystem();

};

#endif // BACKENDSYSTEM_H
