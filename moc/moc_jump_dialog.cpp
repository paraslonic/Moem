/****************************************************************************
** Meta object code from reading C++ file 'jump_dialog.h'
**
** Created: Sun 19. Apr 20:27:19 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../jump_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jump_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UI_JumpMenuDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      40,   18,   18,   18, 0x08,
      68,   18,   18,   18, 0x08,
      94,   18,   18,   18, 0x08,
     119,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UI_JumpMenuDialog[] = {
    "UI_JumpMenuDialog\0\0jumpbutton_pressed()\0"
    "absolutetime_changed(QTime)\0"
    "offsettime_changed(QTime)\0"
    "absoluteday_changed(int)\0"
    "offsetday_changed(int)\0"
};

void UI_JumpMenuDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UI_JumpMenuDialog *_t = static_cast<UI_JumpMenuDialog *>(_o);
        switch (_id) {
        case 0: _t->jumpbutton_pressed(); break;
        case 1: _t->absolutetime_changed((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 2: _t->offsettime_changed((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 3: _t->absoluteday_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->offsetday_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UI_JumpMenuDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UI_JumpMenuDialog::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UI_JumpMenuDialog,
      qt_meta_data_UI_JumpMenuDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UI_JumpMenuDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UI_JumpMenuDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UI_JumpMenuDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UI_JumpMenuDialog))
        return static_cast<void*>(const_cast< UI_JumpMenuDialog*>(this));
    return QObject::qt_metacast(_clname);
}

int UI_JumpMenuDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
