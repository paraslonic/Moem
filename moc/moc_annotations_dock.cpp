/****************************************************************************
** Meta object code from reading C++ file 'annotations_dock.h'
**
** Created: Mon 23. Feb 20:38:37 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../annotations_dock.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'annotations_dock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UI_Annotationswindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,   22,   21,   21, 0x08,
      74,   21,   21,   21, 0x28,
     112,   21,   21,   21, 0x08,
     132,   21,   21,   21, 0x08,
     155,   21,   21,   21, 0x08,
     174,   21,   21,   21, 0x08,
     194,   21,   21,   21, 0x08,
     211,   21,   21,   21, 0x08,
     230,   21,   21,   21, 0x08,
     253,   21,   21,   21, 0x08,
     278,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UI_Annotationswindow[] = {
    "UI_Annotationswindow\0\0,centered\0"
    "annotation_selected(QListWidgetItem*,int)\0"
    "annotation_selected(QListWidgetItem*)\0"
    "hide_editdock(bool)\0checkbox1_clicked(int)\0"
    "show_between(bool)\0average_annot(bool)\0"
    "hide_annot(bool)\0unhide_annot(bool)\0"
    "hide_same_annots(bool)\0unhide_same_annots(bool)\0"
    "unhide_all_annots(bool)\0"
};

void UI_Annotationswindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UI_Annotationswindow *_t = static_cast<UI_Annotationswindow *>(_o);
        switch (_id) {
        case 0: _t->annotation_selected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->annotation_selected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->hide_editdock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->checkbox1_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->show_between((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->average_annot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->hide_annot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->unhide_annot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->hide_same_annots((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->unhide_same_annots((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->unhide_all_annots((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UI_Annotationswindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UI_Annotationswindow::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UI_Annotationswindow,
      qt_meta_data_UI_Annotationswindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UI_Annotationswindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UI_Annotationswindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UI_Annotationswindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UI_Annotationswindow))
        return static_cast<void*>(const_cast< UI_Annotationswindow*>(this));
    return QObject::qt_metacast(_clname);
}

int UI_Annotationswindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
