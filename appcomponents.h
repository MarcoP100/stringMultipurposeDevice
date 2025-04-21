#ifndef APPCOMPONENTS_H
#define APPCOMPONENTS_H

#include <QQmlApplicationEngine>
#include "wifimanager.h"
#include "udpclient.h"
#include "protocolmanager.h"
#include "dynamometerdata.h"
#include "backendsystem.h"

struct AppComponents {
    WiFiManager* wifiManager;
    udpClient* udp;
    ProtocolManager* protocol;
    dynamometerData* dynData;
    BackendSystem* backend;
};

AppComponents initComponents(QQmlApplicationEngine& engine);
void setupConnections(const AppComponents& components);

#endif // APPCOMPONENTS_H
