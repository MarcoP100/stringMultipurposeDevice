#ifndef WIFI_DBUS_TYPES_H
#define WIFI_DBUS_TYPES_H

#include <QList>
#include <QtDBus>
#include <QMap>
#include <QMetaType>


typedef QMap<QString, QVariantMap> NestedMap;
Q_DECLARE_METATYPE(NestedMap)


Q_DECLARE_METATYPE(QList<quint32>)


Q_DECLARE_METATYPE(QList<QList<quint32>>)


void registerDBusTypes();

#endif // WIFI_DBUS_TYPES_H
