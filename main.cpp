#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QProcess>
#include "wifimanager.h"
#include <QQmlContext>
#include "tcpclient.h"
#include "dynamometerdata.h"
#include "canmanager.h"


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

    dynamometerData *dynDecoder = new dynamometerData();
    engine.rootContext()->setContextProperty("dynamometerData", dynDecoder);

    CanManager *canManager= new CanManager(dynDecoder, nullptr);
    engine.rootContext()->setContextProperty("canManager", canManager);


    tcpClient *tcp = new tcpClient(canManager, nullptr);
    engine.rootContext()->setContextProperty("tcpClient", tcp);
    qmlRegisterUncreatableType<tcpClient>("com.example.tcpClient", 1, 0, "TcpClient", "Enum only");



    // collegamento segnali
    QObject::connect(tcp, &tcpClient::rawDataReceived,dynDecoder, [&](const QByteArray &data) {
        dynDecoder->decodeMessage(data);

    });

    QObject::connect(tcp, &tcpClient::connectionTimeout, dynDecoder, [&]() {
        //qDebug() << "Timeout scattato, impostiamo i dati a ------";
        dynDecoder->decodeMessage("\x02X ------\x03");  // Usa lo stesso formato dei dati reali
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
