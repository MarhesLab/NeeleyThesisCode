/****************************************************************************
** Meta object code from reading C++ file 'qgcsthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QgcsThread/qgcsthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgcsthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_updateChecker_t {
    QByteArrayData data[4];
    char stringdata[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_updateChecker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_updateChecker_t qt_meta_stringdata_updateChecker = {
    {
QT_MOC_LITERAL(0, 0, 13), // "updateChecker"
QT_MOC_LITERAL(1, 14, 13), // "NewFrameFound"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 17) // "connectionChanged"

    },
    "updateChecker\0NewFrameFound\0\0"
    "connectionChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_updateChecker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void updateChecker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        updateChecker *_t = static_cast<updateChecker *>(_o);
        switch (_id) {
        case 0: _t->NewFrameFound(); break;
        case 1: _t->connectionChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (updateChecker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&updateChecker::NewFrameFound)) {
                *result = 0;
            }
        }
        {
            typedef void (updateChecker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&updateChecker::connectionChanged)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject updateChecker::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_updateChecker.data,
      qt_meta_data_updateChecker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *updateChecker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *updateChecker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_updateChecker.stringdata))
        return static_cast<void*>(const_cast< updateChecker*>(this));
    return QThread::qt_metacast(_clname);
}

int updateChecker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void updateChecker::NewFrameFound()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void updateChecker::connectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_updateSender_t {
    QByteArrayData data[1];
    char stringdata[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_updateSender_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_updateSender_t qt_meta_stringdata_updateSender = {
    {
QT_MOC_LITERAL(0, 0, 12) // "updateSender"

    },
    "updateSender"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_updateSender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void updateSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject updateSender::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_updateSender.data,
      qt_meta_data_updateSender,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *updateSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *updateSender::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_updateSender.stringdata))
        return static_cast<void*>(const_cast< updateSender*>(this));
    return QThread::qt_metacast(_clname);
}

int updateSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_QgcsThread_t {
    QByteArrayData data[55];
    char stringdata[622];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QgcsThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QgcsThread_t qt_meta_stringdata_QgcsThread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QgcsThread"
QT_MOC_LITERAL(1, 11, 15), // "apstatusChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "voltageChanged"
QT_MOC_LITERAL(4, 43, 14), // "cpuLoadChanged"
QT_MOC_LITERAL(5, 58, 13), // "statusChanged"
QT_MOC_LITERAL(6, 72, 17), // "uartstatusChanged"
QT_MOC_LITERAL(7, 90, 18), // "viconstatusChanged"
QT_MOC_LITERAL(8, 109, 8), // "newFrame"
QT_MOC_LITERAL(9, 118, 15), // "currModeChanged"
QT_MOC_LITERAL(10, 134, 11), // "stateChange"
QT_MOC_LITERAL(11, 146, 11), // "inputChange"
QT_MOC_LITERAL(12, 158, 11), // "viconChange"
QT_MOC_LITERAL(13, 170, 9), // "imuChange"
QT_MOC_LITERAL(14, 180, 9), // "dmcChange"
QT_MOC_LITERAL(15, 190, 17), // "statusChangedSlot"
QT_MOC_LITERAL(16, 208, 5), // "start"
QT_MOC_LITERAL(17, 214, 12), // "updateValues"
QT_MOC_LITERAL(18, 227, 4), // "stop"
QT_MOC_LITERAL(19, 232, 8), // "getState"
QT_MOC_LITERAL(20, 241, 1), // "i"
QT_MOC_LITERAL(21, 243, 6), // "getRef"
QT_MOC_LITERAL(22, 250, 8), // "getInput"
QT_MOC_LITERAL(23, 259, 8), // "getVicon"
QT_MOC_LITERAL(24, 268, 6), // "getImu"
QT_MOC_LITERAL(25, 275, 6), // "getDMC"
QT_MOC_LITERAL(26, 282, 15), // "getAttCtrlState"
QT_MOC_LITERAL(27, 298, 15), // "getAltCtrlState"
QT_MOC_LITERAL(28, 314, 15), // "getPosCtrlState"
QT_MOC_LITERAL(29, 330, 16), // "getCompFailState"
QT_MOC_LITERAL(30, 347, 14), // "getSerIntState"
QT_MOC_LITERAL(31, 362, 16), // "getEmerModeState"
QT_MOC_LITERAL(32, 379, 16), // "getCalibErrState"
QT_MOC_LITERAL(33, 396, 18), // "getGyroCalErrState"
QT_MOC_LITERAL(34, 415, 17), // "getAccCalErrState"
QT_MOC_LITERAL(35, 433, 17), // "getMagStrErrState"
QT_MOC_LITERAL(36, 451, 17), // "getMagIncErrState"
QT_MOC_LITERAL(37, 469, 13), // "setTrajectory"
QT_MOC_LITERAL(38, 483, 5), // "value"
QT_MOC_LITERAL(39, 489, 12), // "setReference"
QT_MOC_LITERAL(40, 502, 8), // "setMotor"
QT_MOC_LITERAL(41, 511, 8), // "apstatus"
QT_MOC_LITERAL(42, 520, 7), // "voltage"
QT_MOC_LITERAL(43, 528, 7), // "cpuload"
QT_MOC_LITERAL(44, 536, 6), // "status"
QT_MOC_LITERAL(45, 543, 10), // "uartstatus"
QT_MOC_LITERAL(46, 554, 11), // "viconstatus"
QT_MOC_LITERAL(47, 566, 8), // "currMode"
QT_MOC_LITERAL(48, 575, 5), // "state"
QT_MOC_LITERAL(49, 581, 5), // "input"
QT_MOC_LITERAL(50, 587, 5), // "vicon"
QT_MOC_LITERAL(51, 593, 3), // "imu"
QT_MOC_LITERAL(52, 597, 4), // "dmcs"
QT_MOC_LITERAL(53, 602, 7), // "desMode"
QT_MOC_LITERAL(54, 610, 11) // "gotNewFrame"

    },
    "QgcsThread\0apstatusChanged\0\0voltageChanged\0"
    "cpuLoadChanged\0statusChanged\0"
    "uartstatusChanged\0viconstatusChanged\0"
    "newFrame\0currModeChanged\0stateChange\0"
    "inputChange\0viconChange\0imuChange\0"
    "dmcChange\0statusChangedSlot\0start\0"
    "updateValues\0stop\0getState\0i\0getRef\0"
    "getInput\0getVicon\0getImu\0getDMC\0"
    "getAttCtrlState\0getAltCtrlState\0"
    "getPosCtrlState\0getCompFailState\0"
    "getSerIntState\0getEmerModeState\0"
    "getCalibErrState\0getGyroCalErrState\0"
    "getAccCalErrState\0getMagStrErrState\0"
    "getMagIncErrState\0setTrajectory\0value\0"
    "setReference\0setMotor\0apstatus\0voltage\0"
    "cpuload\0status\0uartstatus\0viconstatus\0"
    "currMode\0state\0input\0vicon\0imu\0dmcs\0"
    "desMode\0gotNewFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QgcsThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
      14,  260, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  199,    2, 0x06 /* Public */,
       3,    0,  200,    2, 0x06 /* Public */,
       4,    0,  201,    2, 0x06 /* Public */,
       5,    0,  202,    2, 0x06 /* Public */,
       6,    0,  203,    2, 0x06 /* Public */,
       7,    0,  204,    2, 0x06 /* Public */,
       8,    0,  205,    2, 0x06 /* Public */,
       9,    0,  206,    2, 0x06 /* Public */,
      10,    0,  207,    2, 0x06 /* Public */,
      11,    0,  208,    2, 0x06 /* Public */,
      12,    0,  209,    2, 0x06 /* Public */,
      13,    0,  210,    2, 0x06 /* Public */,
      14,    0,  211,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  212,    2, 0x0a /* Public */,
      16,    0,  213,    2, 0x0a /* Public */,
      17,    0,  214,    2, 0x0a /* Public */,
      18,    0,  215,    2, 0x0a /* Public */,
      19,    1,  216,    2, 0x0a /* Public */,
      21,    1,  219,    2, 0x0a /* Public */,
      22,    1,  222,    2, 0x0a /* Public */,
      23,    1,  225,    2, 0x0a /* Public */,
      24,    1,  228,    2, 0x0a /* Public */,
      25,    1,  231,    2, 0x0a /* Public */,
      26,    0,  234,    2, 0x0a /* Public */,
      27,    0,  235,    2, 0x0a /* Public */,
      28,    0,  236,    2, 0x0a /* Public */,
      29,    0,  237,    2, 0x0a /* Public */,
      30,    0,  238,    2, 0x0a /* Public */,
      31,    0,  239,    2, 0x0a /* Public */,
      32,    0,  240,    2, 0x0a /* Public */,
      33,    0,  241,    2, 0x0a /* Public */,
      34,    0,  242,    2, 0x0a /* Public */,
      35,    0,  243,    2, 0x0a /* Public */,
      36,    0,  244,    2, 0x0a /* Public */,
      37,    2,  245,    2, 0x0a /* Public */,
      39,    2,  250,    2, 0x0a /* Public */,
      40,    2,  255,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Double, QMetaType::Int,   20,
    QMetaType::Double, QMetaType::Int,   20,
    QMetaType::Double, QMetaType::Int,   20,
    QMetaType::Double, QMetaType::Int,   20,
    QMetaType::Double, QMetaType::Int,   20,
    QMetaType::Int, QMetaType::Int,   20,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,   20,   38,
    QMetaType::Void, QMetaType::Int, QMetaType::Double,   20,   38,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   20,   38,

 // properties: name, type, flags
      41, QMetaType::Int, 0x00495001,
      42, QMetaType::Double, 0x00495001,
      43, QMetaType::Double, 0x00495001,
      44, QMetaType::QString, 0x00495001,
      45, QMetaType::QString, 0x00495001,
      46, QMetaType::QString, 0x00495001,
      47, QMetaType::QString, 0x00495001,
      48, QMetaType::Double, 0x00495001,
      49, QMetaType::Double, 0x00495001,
      50, QMetaType::Double, 0x00495001,
      51, QMetaType::Double, 0x00495001,
      52, QMetaType::Double, 0x00495001,
      53, QMetaType::QString, 0x00095103,
      54, QMetaType::Bool, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,
       7,
       8,
       9,
      10,
      11,
      12,
       0,
       6,

       0        // eod
};

void QgcsThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QgcsThread *_t = static_cast<QgcsThread *>(_o);
        switch (_id) {
        case 0: _t->apstatusChanged(); break;
        case 1: _t->voltageChanged(); break;
        case 2: _t->cpuLoadChanged(); break;
        case 3: _t->statusChanged(); break;
        case 4: _t->uartstatusChanged(); break;
        case 5: _t->viconstatusChanged(); break;
        case 6: _t->newFrame(); break;
        case 7: _t->currModeChanged(); break;
        case 8: _t->stateChange(); break;
        case 9: _t->inputChange(); break;
        case 10: _t->viconChange(); break;
        case 11: _t->imuChange(); break;
        case 12: _t->dmcChange(); break;
        case 13: _t->statusChangedSlot(); break;
        case 14: _t->start(); break;
        case 15: _t->updateValues(); break;
        case 16: _t->stop(); break;
        case 17: { double _r = _t->getState((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 18: { double _r = _t->getRef((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 19: { double _r = _t->getInput((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 20: { double _r = _t->getVicon((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 21: { double _r = _t->getImu((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 22: { int _r = _t->getDMC((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 23: { QString _r = _t->getAttCtrlState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 24: { QString _r = _t->getAltCtrlState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 25: { QString _r = _t->getPosCtrlState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 26: { QString _r = _t->getCompFailState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 27: { QString _r = _t->getSerIntState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 28: { QString _r = _t->getEmerModeState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 29: { QString _r = _t->getCalibErrState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 30: { QString _r = _t->getGyroCalErrState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 31: { QString _r = _t->getAccCalErrState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 32: { QString _r = _t->getMagStrErrState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 33: { QString _r = _t->getMagIncErrState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 34: _t->setTrajectory((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 35: _t->setReference((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 36: _t->setMotor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::apstatusChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::voltageChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::cpuLoadChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::statusChanged)) {
                *result = 3;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::uartstatusChanged)) {
                *result = 4;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::viconstatusChanged)) {
                *result = 5;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::newFrame)) {
                *result = 6;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::currModeChanged)) {
                *result = 7;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::stateChange)) {
                *result = 8;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::inputChange)) {
                *result = 9;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::viconChange)) {
                *result = 10;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::imuChange)) {
                *result = 11;
            }
        }
        {
            typedef void (QgcsThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QgcsThread::dmcChange)) {
                *result = 12;
            }
        }
    }
}

const QMetaObject QgcsThread::staticMetaObject = {
    { &QQuickItem::staticMetaObject, qt_meta_stringdata_QgcsThread.data,
      qt_meta_data_QgcsThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QgcsThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QgcsThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QgcsThread.stringdata))
        return static_cast<void*>(const_cast< QgcsThread*>(this));
    return QQuickItem::qt_metacast(_clname);
}

int QgcsThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 37)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 37)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 37;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = getAPStatus(); break;
        case 1: *reinterpret_cast< double*>(_v) = getVoltage(); break;
        case 2: *reinterpret_cast< double*>(_v) = getCpuLoad(); break;
        case 3: *reinterpret_cast< QString*>(_v) = getStatus(); break;
        case 4: *reinterpret_cast< QString*>(_v) = getUartStatus(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getViconStatus(); break;
        case 6: *reinterpret_cast< QString*>(_v) = getCurrMode(); break;
        case 7: *reinterpret_cast< double*>(_v) = getState(); break;
        case 8: *reinterpret_cast< double*>(_v) = getInput(); break;
        case 9: *reinterpret_cast< double*>(_v) = getVicon(); break;
        case 10: *reinterpret_cast< double*>(_v) = getImu(); break;
        case 11: *reinterpret_cast< double*>(_v) = getDMC(); break;
        case 12: *reinterpret_cast< QString*>(_v) = getDesMode(); break;
        case 13: *reinterpret_cast< bool*>(_v) = haveNewFrame(); break;
        default: break;
        }
        _id -= 14;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 12: setDesMode(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
        _id -= 14;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 14;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QgcsThread::apstatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QgcsThread::voltageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QgcsThread::cpuLoadChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void QgcsThread::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void QgcsThread::uartstatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void QgcsThread::viconstatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void QgcsThread::newFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void QgcsThread::currModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void QgcsThread::stateChange()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}

// SIGNAL 9
void QgcsThread::inputChange()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}

// SIGNAL 10
void QgcsThread::viconChange()
{
    QMetaObject::activate(this, &staticMetaObject, 10, Q_NULLPTR);
}

// SIGNAL 11
void QgcsThread::imuChange()
{
    QMetaObject::activate(this, &staticMetaObject, 11, Q_NULLPTR);
}

// SIGNAL 12
void QgcsThread::dmcChange()
{
    QMetaObject::activate(this, &staticMetaObject, 12, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
