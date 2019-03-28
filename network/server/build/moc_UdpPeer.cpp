/****************************************************************************
** Meta object code from reading C++ file 'UdpPeer.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../UdpPeer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UdpPeer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_asteroids__UdpPeer_t {
    QByteArrayData data[21];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_asteroids__UdpPeer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_asteroids__UdpPeer_t qt_meta_stringdata_asteroids__UdpPeer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "asteroids::UdpPeer"
QT_MOC_LITERAL(1, 19, 15), // "received_bullet"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 2), // "id"
QT_MOC_LITERAL(4, 39, 9), // "Vector3f&"
QT_MOC_LITERAL(5, 49, 8), // "position"
QT_MOC_LITERAL(6, 58, 8), // "velocity"
QT_MOC_LITERAL(7, 67, 17), // "received_position"
QT_MOC_LITERAL(8, 85, 6), // "x_axis"
QT_MOC_LITERAL(9, 92, 6), // "y_axis"
QT_MOC_LITERAL(10, 99, 6), // "z_axis"
QT_MOC_LITERAL(11, 106, 18), // "received_collision"
QT_MOC_LITERAL(12, 125, 7), // "obj_id1"
QT_MOC_LITERAL(13, 133, 7), // "obj_id2"
QT_MOC_LITERAL(14, 141, 12), // "received_ack"
QT_MOC_LITERAL(15, 154, 6), // "seq_nr"
QT_MOC_LITERAL(16, 161, 16), // "received_missile"
QT_MOC_LITERAL(17, 178, 1), // "x"
QT_MOC_LITERAL(18, 180, 1), // "y"
QT_MOC_LITERAL(19, 182, 1), // "z"
QT_MOC_LITERAL(20, 184, 10) // "handle_udp"

    },
    "asteroids::UdpPeer\0received_bullet\0\0"
    "id\0Vector3f&\0position\0velocity\0"
    "received_position\0x_axis\0y_axis\0z_axis\0"
    "received_collision\0obj_id1\0obj_id2\0"
    "received_ack\0seq_nr\0received_missile\0"
    "x\0y\0z\0handle_udp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_asteroids__UdpPeer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   44,    2, 0x06 /* Public */,
       7,    5,   51,    2, 0x06 /* Public */,
      11,    3,   62,    2, 0x06 /* Public */,
      14,    2,   69,    2, 0x06 /* Public */,
      16,    5,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, 0x80000000 | 4,    3,    5,    6,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, 0x80000000 | 4, 0x80000000 | 4, 0x80000000 | 4,    3,    5,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,   12,   13,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,   15,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, 0x80000000 | 4, 0x80000000 | 4, 0x80000000 | 4,    3,    5,   17,   18,   19,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void asteroids::UdpPeer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UdpPeer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->received_bullet((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Vector3f(*)>(_a[2])),(*reinterpret_cast< Vector3f(*)>(_a[3]))); break;
        case 1: _t->received_position((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Vector3f(*)>(_a[2])),(*reinterpret_cast< Vector3f(*)>(_a[3])),(*reinterpret_cast< Vector3f(*)>(_a[4])),(*reinterpret_cast< Vector3f(*)>(_a[5]))); break;
        case 2: _t->received_collision((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->received_ack((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->received_missile((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Vector3f(*)>(_a[2])),(*reinterpret_cast< Vector3f(*)>(_a[3])),(*reinterpret_cast< Vector3f(*)>(_a[4])),(*reinterpret_cast< Vector3f(*)>(_a[5]))); break;
        case 5: _t->handle_udp(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UdpPeer::*)(int , Vector3f & , Vector3f & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpPeer::received_bullet)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UdpPeer::*)(int , Vector3f & , Vector3f & , Vector3f & , Vector3f & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpPeer::received_position)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UdpPeer::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpPeer::received_collision)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UdpPeer::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpPeer::received_ack)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UdpPeer::*)(int , Vector3f & , Vector3f & , Vector3f & , Vector3f & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UdpPeer::received_missile)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject asteroids::UdpPeer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_asteroids__UdpPeer.data,
    qt_meta_data_asteroids__UdpPeer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *asteroids::UdpPeer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *asteroids::UdpPeer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_asteroids__UdpPeer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int asteroids::UdpPeer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void asteroids::UdpPeer::received_bullet(int _t1, Vector3f & _t2, Vector3f & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void asteroids::UdpPeer::received_position(int _t1, Vector3f & _t2, Vector3f & _t3, Vector3f & _t4, Vector3f & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void asteroids::UdpPeer::received_collision(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void asteroids::UdpPeer::received_ack(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void asteroids::UdpPeer::received_missile(int _t1, Vector3f & _t2, Vector3f & _t3, Vector3f & _t4, Vector3f & _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
