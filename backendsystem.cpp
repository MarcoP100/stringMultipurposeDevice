#include "backendsystem.h"
#include <QDebug>

BackendSystem::BackendSystem(QObject *parent): QObject(parent) {}

void BackendSystem::shutdownSystem() {
    qDebug() << "Eseguo: shutdown";
    QProcess::execute("sudo", QStringList() << "shutdown" << "-h" << "now");
}

void BackendSystem::rebootSystem() {
    qDebug() << "Eseguo: reboot";
    QProcess::execute("sudo", QStringList() << "reboot");
}
