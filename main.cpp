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
    WiFiManager wifiManager;

     engine.rootContext()->setContextProperty("wifiManager", &wifiManager);

    // Registra la classe WiFiManager in modo che sia disponibile in QML
    //qmlRegisterType<WiFiManager>("CustomComponents", 1, 0, "WiFiManager");

    const QUrl url(QStringLiteral("qrc:/StringMultipurposeDevice/main.qml"));
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
