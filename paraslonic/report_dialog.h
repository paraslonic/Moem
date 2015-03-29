/********************************************************************************
** Form generated from reading UI file 'designerMu7952.ui'
**
** Created: Wed 27. Feb 23:54:55 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNERMU7952_H
#define DESIGNERMU7952_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>

#include "../global.h"
#include "../mainwindow.h"

class UI_Mainwindow;

class UI_Report_Dialog : public QObject
{
	Q_OBJECT
public:
	UI_Report_Dialog(QWidget *parent=0);
	UI_Mainwindow *mainwindow;
private:
private:
    QDateTimeEdit *dateTimeEdit;
    QDateTimeEdit *dateTimeEdit_2;
    QLabel *label;
    QLabel *label_2;
    QSplitter *splitter;
    QPushButton *pushButton;
    QDialogButtonBox *buttonBox;

private slots:
	void report();

};

#endif // DESIGNERMU7952_H
