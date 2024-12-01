#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QProcess>
#include "wifimanager.h"
#include <QQmlContext>


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

    return app.exec();
}
