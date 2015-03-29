// paraslonic@gmail.com																							Æ:-) (-::Æ

#include "report_dialog.h"   

UI_Report_Dialog::UI_Report_Dialog(QWidget *w_parent)
{
	mainwindow = (UI_Mainwindow *)w_parent;

	QDialog *report_dialog = new QDialog(w_parent);
	if (report_dialog->objectName().isEmpty())
		report_dialog->setObjectName(QString::fromUtf8("Dialog"));
	report_dialog->resize(359, 151);
	dateTimeEdit = new QDateTimeEdit(report_dialog);
	dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
	dateTimeEdit->setGeometry(QRect(130, 20, 194, 22));
	dateTimeEdit_2 = new QDateTimeEdit(report_dialog);
	dateTimeEdit_2->setObjectName(QString::fromUtf8("dateTimeEdit_2"));
	dateTimeEdit_2->setGeometry(QRect(130, 60, 194, 22));
	label = new QLabel(report_dialog);
	label->setObjectName(QString::fromUtf8("label"));
	label->setGeometry(QRect(20, 20, 111, 16));
	QFont font;
	font.setPointSize(10);
	label->setFont(font);
	label_2 = new QLabel(report_dialog);
	label_2->setObjectName(QString::fromUtf8("label_2"));
	label_2->setGeometry(QRect(20, 60, 101, 16));
	label_2->setFont(font);
	splitter = new QSplitter(report_dialog);
	splitter->setObjectName(QString::fromUtf8("splitter"));
	splitter->setGeometry(QRect(30, 110, 266, 23));
	splitter->setOrientation(Qt::Horizontal);
	pushButton = new QPushButton(splitter);
	pushButton->setObjectName(QString::fromUtf8("pushButton"));
	splitter->addWidget(pushButton);
	buttonBox = new QDialogButtonBox(splitter);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	splitter->addWidget(buttonBox);

	report_dialog->setWindowTitle(QApplication::translate("Dialog", "Report dialog", 0, QApplication::UnicodeUTF8));
	label->setText(QApplication::translate("Dialog", "start time:", 0, QApplication::UnicodeUTF8));
	label_2->setText(QApplication::translate("Dialog", "end time:", 0, QApplication::UnicodeUTF8));
	pushButton->setText(QApplication::translate("Dialog", "Folder", 0, QApplication::UnicodeUTF8));

	QObject::connect(buttonBox, SIGNAL(accepted()), report_dialog, SLOT(report()));
	QObject::connect(buttonBox, SIGNAL(accepted()), report_dialog, SLOT(accept()));

	QObject::connect(buttonBox, SIGNAL(rejected()), report_dialog, SLOT(reject()));

	QMetaObject::connectSlotsByName(report_dialog);

	report_dialog->exec();
}

void UI_Report_Dialog::report()
{
	//dateTimeEdit
	/*milliseconds = (long long)(timeEdit1->time().hour()) * 3600000LL;
	milliseconds += (long long)(timeEdit1->time().minute()) * 60000LL;
	milliseconds += (long long)(timeEdit1->time().second()) * 1000LL;
	milliseconds += (long long)(timeEdit1->time().msec());

	if(!milliseconds)
	{
		return;
	}

	mainwindow->pagetime = milliseconds * (TIME_DIMENSION / 1000);*/

}

