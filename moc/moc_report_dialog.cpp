/****************************************************************************
** Meta object code from reading C++ file 'report_dialog.h'
**
** Created: Mon 23. Feb 20:39:18 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../paraslonic/report_dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'report_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UI_Report_Dialog[] = {

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

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UI_Report_Dialog[] = {
    "UI_Report_Dialog\0\0report()\0"
};

void UI_Report_Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UI_Report_Dialog *_t = static_cast<UI_Report_Dialog *>(_o);
        switch (_id) {
        case 0: _t->report(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData UI_Report_Dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UI_Report_Dialog::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UI_Report_Dialog,
      qt_meta_data_UI_Report_Dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UI_Report_Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UI_Report_Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UI_Report_Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UI_Report_Dialog))
        return static_cast<void*>(const_cast< UI_Report_Dialog*>(this));
    return QObject::qt_metacast(_clname);
}

int UI_Report_Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
