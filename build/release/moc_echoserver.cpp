/****************************************************************************
** Meta object code from reading C++ file 'echoserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../echoserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'echoserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EchoServer_t {
    QByteArrayData data[17];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EchoServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EchoServer_t qt_meta_stringdata_EchoServer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EchoServer"
QT_MOC_LITERAL(1, 11, 10), // "failListen"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 9), // "connected"
QT_MOC_LITERAL(4, 33, 4), // "addr"
QT_MOC_LITERAL(5, 38, 4), // "port"
QT_MOC_LITERAL(6, 43, 5), // "oAddr"
QT_MOC_LITERAL(7, 49, 5), // "oport"
QT_MOC_LITERAL(8, 55, 12), // "disconnected"
QT_MOC_LITERAL(9, 68, 14), // "newConnections"
QT_MOC_LITERAL(10, 83, 5), // "close"
QT_MOC_LITERAL(11, 89, 8), // "QThread*"
QT_MOC_LITERAL(12, 98, 6), // "thread"
QT_MOC_LITERAL(13, 105, 6), // "listen"
QT_MOC_LITERAL(14, 112, 14), // "getServerError"
QT_MOC_LITERAL(15, 127, 6), // "remove"
QT_MOC_LITERAL(16, 134, 12) // "QHostAddress"

    },
    "EchoServer\0failListen\0\0connected\0addr\0"
    "port\0oAddr\0oport\0disconnected\0"
    "newConnections\0close\0QThread*\0thread\0"
    "listen\0getServerError\0remove\0QHostAddress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EchoServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    4,   60,    2, 0x06 /* Public */,
       8,    4,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   78,    2, 0x08 /* Private */,
      10,    1,   79,    2, 0x0a /* Public */,
      10,    0,   82,    2, 0x2a /* Public | MethodCloned */,

 // methods: name, argc, parameters, tag, flags
      13,    0,   83,    2, 0x02 /* Public */,
      14,    0,   84,    2, 0x02 /* Public */,
      15,    1,   85,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::Int,    4,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::Int,    4,    5,    6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void, 0x80000000 | 16,    4,

       0        // eod
};

void EchoServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EchoServer *_t = static_cast<EchoServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->failListen(); break;
        case 1: _t->connected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->disconnected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->newConnections(); break;
        case 4: _t->close((*reinterpret_cast< QThread*(*)>(_a[1]))); break;
        case 5: _t->close(); break;
        case 6: _t->listen(); break;
        case 7: { QString _r = _t->getServerError();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->remove((*reinterpret_cast< const QHostAddress(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QThread* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EchoServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EchoServer::failListen)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (EchoServer::*_t)(const QString & , int , const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EchoServer::connected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (EchoServer::*_t)(const QString & , int , const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EchoServer::disconnected)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject EchoServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EchoServer.data,
      qt_meta_data_EchoServer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EchoServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EchoServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EchoServer.stringdata0))
        return static_cast<void*>(const_cast< EchoServer*>(this));
    return QObject::qt_metacast(_clname);
}

int EchoServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void EchoServer::failListen()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void EchoServer::connected(const QString & _t1, int _t2, const QString & _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EchoServer::disconnected(const QString & _t1, int _t2, const QString & _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
