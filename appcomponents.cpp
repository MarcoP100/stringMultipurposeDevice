#include "appcomponents.h"
#include <QQmlContext>
#include "protocol_constants.h"


AppComponents initComponents(QQmlApplicationEngine& engine) {
    AppComponents components;

    components.wifiManager = new WiFiManager(nullptr);
    engine.rootContext()->setContextProperty("WiFiManager", components.wifiManager);
    qmlRegisterUncreatableType<WiFiManager>("com.wifiManager", 1, 0, "WiFiStatusEnum",
                                            "WiFiManager is available only via context property");
    WiFiManager::registerDBusTypes();

    components.udp = new udpClient(components.wifiManager);
    engine.rootContext()->setContextProperty("udpClient", components.udp);

    components.protocol = new ProtocolManager(components.udp);
    engine.rootContext()->setContextProperty("ProtocolManager", components.protocol);

    components.dynData = new dynamometerData(components.protocol);
    engine.rootContext()->setContextProperty("dynamometerData", components.dynData);

    components.backend = new BackendSystem();
    engine.rootContext()->setContextProperty("BackendSystem", components.backend);

    return components;
}

void setupConnections(const AppComponents& components) {
    QObject::connect(components.wifiManager, &WiFiManager::ssidChanged,
                     components.protocol, [=](const QString& ssid) {
                         if (ssid == ESP_SSID_NAME) {
                             components.protocol->startHeartbeat();
                         } else {
                             components.protocol->stopHeartbeat();
                         }
                     });

    QObject::connect(components.protocol, &ProtocolManager::packetsLostChanged,
                     components.dynData, &dynamometerData::updatePacketsLost);

    QObject::connect(components.udp, &udpClient::rawDataReceived,
                     components.protocol, &ProtocolManager::handleIncomingMessage);

    QObject::connect(components.protocol, &ProtocolManager::validMessageReceived,
                     components.dynData, &dynamometerData::updateFromMessage);

    QObject::connect(components.protocol, &ProtocolManager::messageToSend,
                     components.udp, &udpClient::sendMessage);

}

