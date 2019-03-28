/****************************************************************************
** Meta object code from reading C++ file 'UdpServer.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UdpServer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UdpServer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UdpServer_t {
    QByteArrayData data[21];
    char stringdata0[168];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UdpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UdpServer_t qt_meta_stringdata_UdpServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UdpServer"
QT_MOC_LITERAL(1, 10, 8), // "fightEnd"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 9), // "winner_id"
QT_MOC_LITERAL(4, 30, 11), // "health_left"
QT_MOC_LITERAL(5, 42, 13), // "handle_bullet"
QT_MOC_LITERAL(6, 56, 2), // "id"
QT_MOC_LITERAL(7, 59, 9), // "Vector3f&"
QT_MOC_LITERAL(8, 69, 8), // "position"
QT_MOC_LITERAL(9, 78, 8), // "velocity"
QT_MOC_LITERAL(10, 87, 14), // "handle_missile"
QT_MOC_LITERAL(11, 102, 1), // "x"
QT_MOC_LITERAL(12, 104, 1), // "y"
QT_MOC_LITERAL(13, 106, 1), // "z"
QT_MOC_LITERAL(14, 108, 15), // "handle_position"
QT_MOC_LITERAL(15, 124, 6), // "x_axis"
QT_MOC_LITERAL(16, 131, 6), // "y_axis"
QT_MOC_LITERAL(17, 138, 6), // "z_axis"
QT_MOC_LITERAL(18, 145, 10), // "handle_ack"
QT_MOC_LITERAL(19, 156, 6), // "seq_nr"
QT_MOC_LITERAL(20, 163, 4) // "tick"

    },
    "UdpServer\0fightEnd\0\0winner_id\0health_left\0"
    "handle_bullet\0id\0Vector3f&\0position\0"
    "velocity\0handle_missile\0x\0y\0z\0"
    "handle_position\0x_axis\0y_axis\0z_axis\0"
    "handle_ack\0seq_nr\0tick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UdpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    3,   49,    2, 0x08 /* Private */,
      10,    5,   56,    2, 0x08 /* Private */,
      14,    5,   67,    2, 0x08 /* Private */,
      18,    2,   78,    2, 0x08 /* Private */,
      20,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 7,    6,    8,    9,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7,    6,    8,   11,   12,   13,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7,    6,    8,   15,   16,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,   19,
    QMetaType::Void,

       0        // eod
};

void UdpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UdpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fightEnd((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->handle_bullet((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Vector3f(*)>(_a[2])),(*reinterpret_cast< Vector3f(*)>(_a[3]))); break;
        case 2: _t->handle_missile((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Vector3f(*)>(_a[2])),(*reinterpret_cast< Vector3f(*)>(_a[3])),(*reinterpret_cast< Vector3f(*)>(_a[4])),(*reinterpret_cast< Vector3f(*)>(_a[5]))); break;
        case 3: _t->handle_position((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Vector3f(*)>(_a[2])),(*reinterpret_cast< Vector3f(*)>(_a[3])),(*reinterpret_cast< Vector3f(*)>(_a[4])),(*reinterpret_cast< Vector3f(*)>(_a[5]))); break;
        case 4: _t->handle_ack((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->tick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UdpServer::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpServer::fightEnd)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UdpServer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_UdpServer.data,
    qt_meta_data_UdpServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UdpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UdpServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UdpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void UdpServer::fightEnd(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
