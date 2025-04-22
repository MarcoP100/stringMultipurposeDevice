#include "wifi_dbus_types.h"

void registerDBusTypes() {
    qDBusRegisterMetaType<NestedMap>();
    qDBusRegisterMetaType<QList<quint32>>();
    qDBusRegisterMetaType<QList<QList<quint32>>>();

}
