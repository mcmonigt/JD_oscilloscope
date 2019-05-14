/****************************************************************************
** Meta object code from reading C++ file 'datasource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "datasource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datasource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataSource_t {
    QByteArrayData data[18];
    char stringdata0[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataSource_t qt_meta_stringdata_DataSource = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DataSource"
QT_MOC_LITERAL(1, 11, 12), // "generateData"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "type"
QT_MOC_LITERAL(4, 30, 8), // "rowCount"
QT_MOC_LITERAL(5, 39, 8), // "colCount"
QT_MOC_LITERAL(6, 48, 6), // "update"
QT_MOC_LITERAL(7, 55, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(8, 72, 6), // "series"
QT_MOC_LITERAL(9, 79, 10), // "series_num"
QT_MOC_LITERAL(10, 90, 8), // "readData"
QT_MOC_LITERAL(11, 99, 11), // "timeElapsed"
QT_MOC_LITERAL(12, 111, 11), // "setupSerial"
QT_MOC_LITERAL(13, 123, 12), // "QSerialPort*"
QT_MOC_LITERAL(14, 136, 6), // "serial"
QT_MOC_LITERAL(15, 143, 15), // "changeTimeScale"
QT_MOC_LITERAL(16, 159, 10), // "time_scale"
QT_MOC_LITERAL(17, 170, 8) // "testData"

    },
    "DataSource\0generateData\0\0type\0rowCount\0"
    "colCount\0update\0QAbstractSeries*\0"
    "series\0series_num\0readData\0timeElapsed\0"
    "setupSerial\0QSerialPort*\0serial\0"
    "changeTimeScale\0time_scale\0testData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataSource[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x0a /* Public */,
       6,    2,   56,    2, 0x0a /* Public */,
      10,    1,   61,    2, 0x0a /* Public */,
      11,    0,   64,    2, 0x0a /* Public */,
      12,    1,   65,    2, 0x0a /* Public */,
      15,    1,   68,    2, 0x0a /* Public */,
      17,    0,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Int,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,

       0        // eod
};

void DataSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataSource *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->generateData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->update((*reinterpret_cast< QAbstractSeries*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->readData((*reinterpret_cast< QAbstractSeries*(*)>(_a[1]))); break;
        case 3: { int _r = _t->timeElapsed();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->setupSerial((*reinterpret_cast< QSerialPort*(*)>(_a[1]))); break;
        case 5: _t->changeTimeScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->testData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSerialPort* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataSource::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DataSource.data,
    qt_meta_data_DataSource,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataSource.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
