#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QProcess>
#include "wifimanager.h"
#include <QQmlContext>
#include "tcpclient.h"
#include "dynamometerdata.h"


int main(int argc, char *argv[])
{

    // Abilita la tastiera virtuale
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);



    qDebug() << "Using QPA platform:" << qgetenv("QT_QPA_PLATFORM");

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<WiFiManager>("com.wifiManager", 1, 0, "WiFiManager", WiFiManager::createSingletonInstance);
    // Registra il tipo per D-Bus
    WiFiManager::registerDBusTypes();

    // Registrazione del tipo per esporre l'enum a QML
    //qmlRegisterType<WiFiManager>("com.wifiManager", 1, 0, "WiFiManager");

     // Setta il context property per avere accesso a wifiManager come istanza
     //engine.rootContext()->setContextProperty("wifiManager", &wifiManager);

    tcpClient *tcp = new tcpClient();
    engine.rootContext()->setContextProperty("tcpClient", tcp);

    dynamometerData *dynDecoder = new dynamometerData();
    engine.rootContext()->setContextProperty("dynamometerData", dynDecoder);

    // collegamento segnali
    QObject::connect(tcp, &tcpClient::rawDataReceived,dynDecoder, [&](const QByteArray &data) {
        dynDecoder->decodeMessage(data);

    });



    const QUrl url(QStringLiteral("qrc:/StringMultipurposeDevice/QML/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML file. Exiting application.";
        return -1;
    }

    // Connetti il segnale aboutToQuit per eliminare tcp
   /* QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        qDebug() << "Chiusura";
        delete tcp; // Libera la memoria di tcp
    });*/

    return app.exec();
}
