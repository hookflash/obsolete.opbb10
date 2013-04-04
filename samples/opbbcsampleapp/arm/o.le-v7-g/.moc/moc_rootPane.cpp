/****************************************************************************
** Meta object code from reading C++ file 'rootPane.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/rootPane.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rootPane.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_hookflash__blackberry__RootPane[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      38,   33,   32,   32, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_hookflash__blackberry__RootPane[] = {
    "hookflash::blackberry::RootPane\0\0page\0"
    "OnLoginClick(QObject*)\0"
};

void hookflash::blackberry::RootPane::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RootPane *_t = static_cast<RootPane *>(_o);
        switch (_id) {
        case 0: _t->OnLoginClick((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData hookflash::blackberry::RootPane::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject hookflash::blackberry::RootPane::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_hookflash__blackberry__RootPane,
      qt_meta_data_hookflash__blackberry__RootPane, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &hookflash::blackberry::RootPane::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *hookflash::blackberry::RootPane::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *hookflash::blackberry::RootPane::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_hookflash__blackberry__RootPane))
        return static_cast<void*>(const_cast< RootPane*>(this));
    return QObject::qt_metacast(_clname);
}

int hookflash::blackberry::RootPane::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
