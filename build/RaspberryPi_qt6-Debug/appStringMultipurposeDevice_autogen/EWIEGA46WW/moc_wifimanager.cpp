/****************************************************************************
** Meta object code from reading C++ file 'wifimanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../wifimanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wifimanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSWiFiManagerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSWiFiManagerENDCLASS = QtMocHelpers::stringData(
    "WiFiManager",
    "availableNetworksChanged",
    "",
    "networks",
    "busyChanged",
    "busy",
    "connectionStatusChanged",
    "WiFiManager::ConnectionStatus",
    "status",
    "savedNetworksChanged",
    "handleProcessFinished",
    "exitCode",
    "QProcess::ExitStatus",
    "exitStatus",
    "handleConnectionFinished",
    "handleSavedNetworksFinished",
    "handleReadyRead",
    "handleConnectionTimeout",
    "handleSavedNetworksTimeout",
    "scanNetworks",
    "stopNetworkScan",
    "connectToNetwork",
    "ssid",
    "password",
    "isBusy",
    "ConnectionStatus",
    "ErrorProcessStart",
    "Timeout",
    "Connecting",
    "Connected",
    "WrongPassword",
    "ConnectionFailed",
    "SsidEmpty"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWiFiManagerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       1,  136, // properties
       1,  141, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   98,    2, 0x06,    3 /* Public */,
       4,    1,  101,    2, 0x06,    5 /* Public */,
       6,    1,  104,    2, 0x06,    7 /* Public */,
       9,    1,  107,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    2,  110,    2, 0x08,   11 /* Private */,
      14,    2,  115,    2, 0x08,   14 /* Private */,
      15,    2,  120,    2, 0x08,   17 /* Private */,
      16,    0,  125,    2, 0x08,   20 /* Private */,
      17,    0,  126,    2, 0x08,   21 /* Private */,
      18,    0,  127,    2, 0x08,   22 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      19,    0,  128,    2, 0x02,   23 /* Public */,
      20,    0,  129,    2, 0x02,   24 /* Public */,
      21,    2,  130,    2, 0x02,   25 /* Public */,
      24,    0,  135,    2, 0x102,   28 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantList,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 12,   11,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   22,   23,
    QMetaType::Bool,

 // properties: name, type, flags
       5, QMetaType::Bool, 0x00015001, uint(1), 0,

 // enums: name, alias, flags, count, data
      25,   25, 0x0,    7,  146,

 // enum data: key, value
      26, uint(WiFiManager::ErrorProcessStart),
      27, uint(WiFiManager::Timeout),
      28, uint(WiFiManager::Connecting),
      29, uint(WiFiManager::Connected),
      30, uint(WiFiManager::WrongPassword),
      31, uint(WiFiManager::ConnectionFailed),
      32, uint(WiFiManager::SsidEmpty),

       0        // eod
};

Q_CONSTINIT const QMetaObject WiFiManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSWiFiManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWiFiManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWiFiManagerENDCLASS_t,
        // property 'busy'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // enum 'ConnectionStatus'
        QtPrivate::TypeAndForceComplete<WiFiManager::ConnectionStatus, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WiFiManager, std::true_type>,
        // method 'availableNetworksChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantList &, std::false_type>,
        // method 'busyChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'connectionStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<WiFiManager::ConnectionStatus, std::false_type>,
        // method 'savedNetworksChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'handleProcessFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>,
        // method 'handleConnectionFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>,
        // method 'handleSavedNetworksFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>,
        // method 'handleReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleConnectionTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleSavedNetworksTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'scanNetworks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopNetworkScan'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectToNetwork'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'isBusy'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void WiFiManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WiFiManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->availableNetworksChanged((*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[1]))); break;
        case 1: _t->busyChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->connectionStatusChanged((*reinterpret_cast< std::add_pointer_t<WiFiManager::ConnectionStatus>>(_a[1]))); break;
        case 3: _t->savedNetworksChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->handleProcessFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 5: _t->handleConnectionFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 6: _t->handleSavedNetworksFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 7: _t->handleReadyRead(); break;
        case 8: _t->handleConnectionTimeout(); break;
        case 9: _t->handleSavedNetworksTimeout(); break;
        case 10: _t->scanNetworks(); break;
        case 11: _t->stopNetworkScan(); break;
        case 12: _t->connectToNetwork((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: { bool _r = _t->isBusy();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WiFiManager::*)(const QVariantList & );
            if (_t _q_method = &WiFiManager::availableNetworksChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WiFiManager::*)(bool );
            if (_t _q_method = &WiFiManager::busyChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WiFiManager::*)(WiFiManager::ConnectionStatus );
            if (_t _q_method = &WiFiManager::connectionStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WiFiManager::*)(const QString & );
            if (_t _q_method = &WiFiManager::savedNetworksChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<WiFiManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isBusy(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *WiFiManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WiFiManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWiFiManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WiFiManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void WiFiManager::availableNetworksChanged(const QVariantList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WiFiManager::busyChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WiFiManager::connectionStatusChanged(WiFiManager::ConnectionStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WiFiManager::savedNetworksChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
