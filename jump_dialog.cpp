/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012, 2013 Teunis van Beelen
*
* teuniz@gmail.com
*
***************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2 of the License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
***************************************************************************
*
* This version of GPL is at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*
***************************************************************************
*/




#include "jump_dialog.h"



UI_JumpMenuDialog::UI_JumpMenuDialog(QWidget *w_parent)
{
  mainwindow = (UI_Mainwindow *)w_parent;

  jump_dialog = new QDialog(w_parent);

  jump_dialog->setMinimumSize(QSize(435, 200));
  jump_dialog->setMaximumSize(QSize(435, 200));
  jump_dialog->setWindowTitle("Jump to");
  jump_dialog->setModal(TRUE);
  jump_dialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  label1 = new QLabel(jump_dialog);
  label1->setGeometry(QRect(10, 10, 155, 25));
  label1->setText(" dd      hh:mm:ss.ms ");

  daybox1 = new QSpinBox(jump_dialog);
  daybox1->setGeometry(QRect(10, 45, 45, 25));
  daybox1->setRange(0, 30);
  daybox1->setEnabled(FALSE);

  daybox2 = new QSpinBox(jump_dialog);
  daybox2->setGeometry(QRect(10, 80, 45, 25));
  daybox2->setRange(0, 30);
  daybox2->setEnabled(FALSE);

  timeEdit1 = new QTimeEdit(jump_dialog);
  timeEdit1->setGeometry(QRect(65, 45, 110, 25));
  timeEdit1->setDisplayFormat("hh:mm:ss.zzz");
  timeEdit1->setMinimumTime(QTime(0, 0, 0, 0));
  timeEdit1->setEnabled(FALSE);

  timeEdit2 = new QTimeEdit(jump_dialog);
  timeEdit2->setGeometry(QRect(65, 80, 110, 25));
  timeEdit2->setDisplayFormat("hh:mm:ss.zzz");
  timeEdit2->setMinimumTime(QTime(0, 0, 0, 0));
  timeEdit2->setEnabled(FALSE);

  label2 = new QLabel("Offset from start of recording", jump_dialog);
  label2->setGeometry(QRect(185, 45, 250, 25));

  label3 = new QLabel("Absolute time", jump_dialog);
  label3->setGeometry(QRect(185, 80, 250, 25));

  jumpButton = new QPushButton(jump_dialog);
  jumpButton->setGeometry(QRect(10, 165, 100, 25));
  jumpButton->setText("Jump");
  jumpButton->setEnabled(FALSE);

  CloseButton = new QPushButton(jump_dialog);
  CloseButton->setGeometry(QRect(325, 165, 100, 25));
  CloseButton->setText("Cancel");

  if(mainwindow->files_open)
  {
    daybox1->setEnabled(TRUE);
    daybox2->setEnabled(TRUE);
    timeEdit1->setEnabled(TRUE);
    timeEdit2->setEnabled(TRUE);

    starttime = mainwindow->edfheaderlist[mainwindow->sel_viewtime]->l_starttime
                + mainwindow->edfheaderlist[mainwindow->sel_viewtime]->starttime_offset;

    starttime /= 10000;

    time2.setHMS((int)((starttime / 3600000LL) % 24LL), (int)((starttime % 3600000LL) / 60000LL), (int)((starttime % 60000LL) / 1000LL), (int)(starttime % 1000LL));

    timeEdit2->setMinimumTime(QTime((int)((starttime / 3600000LL) % 24LL), (int)((starttime % 3600000LL) / 60000LL), (int)((starttime % 60000LL) / 1000LL), (int)(starttime % 1000LL)));

    timeEdit2->setTime(time2);

    recording_duration = mainwindow->edfheaderlist[0]->datarecords * mainwindow->edfheaderlist[0]->long_data_record_duration;
    recording_duration /= (TIME_DIMENSION / 1000LL);

    jumpButton->setEnabled(TRUE);
  }

  QObject::connect(CloseButton, SIGNAL(clicked()),                  jump_dialog, SLOT(close()));
  QObject::connect(jumpButton,  SIGNAL(clicked()),                  this,        SLOT(jumpbutton_pressed()));
  QObject::connect(timeEdit1,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(offsettime_changed(const QTime &)));
  QObject::connect(timeEdit2,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(absolutetime_changed(const QTime &)));
  QObject::connect(daybox1,     SIGNAL(valueChanged(int)),          this,        SLOT(offsetday_changed(int)));
  QObject::connect(daybox2,     SIGNAL(valueChanged(int)),          this,        SLOT(absoluteday_changed(int)));

  jump_dialog->exec();
}


void UI_JumpMenuDialog::offsetday_changed(int days)
{
  long long milliseconds;

  if(!mainwindow->files_open)  return;

  QObject::disconnect(daybox2,     SIGNAL(valueChanged(int)),          this,        SLOT(absoluteday_changed(int)));
  QObject::disconnect(timeEdit2,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(absolutetime_changed(const QTime &)));

  milliseconds = (long long)(timeEdit1->time().hour()) * 3600000LL;
  milliseconds += (long long)(timeEdit1->time().minute()) * 60000LL;
  milliseconds += (long long)(timeEdit1->time().second()) * 1000LL;
  milliseconds += (long long)(timeEdit1->time().msec());

  milliseconds += ((long long)days * 86400000LL);

  milliseconds += starttime;

  time2.setHMS((int)((milliseconds / 3600000LL) % 24LL), (int)((milliseconds % 3600000LL) / 60000LL), (int)((milliseconds % 60000LL) / 1000LL), (int)(milliseconds % 1000LL));

  timeEdit2->setTime(time2);

  daybox2->setValue((int)(milliseconds / 86400000LL));

  QObject::connect(daybox2,     SIGNAL(valueChanged(int)),          this,        SLOT(absoluteday_changed(int)));
  QObject::connect(timeEdit2,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(absolutetime_changed(const QTime &)));
}


void UI_JumpMenuDialog::absoluteday_changed(int days)
{
  long long milliseconds;

  if(!mainwindow->files_open)  return;

  QObject::disconnect(daybox1,     SIGNAL(valueChanged(int)),          this,        SLOT(offsetday_changed(int)));
  QObject::disconnect(timeEdit1,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(offsettime_changed(const QTime &)));

  milliseconds = (long long)(timeEdit2->time().hour()) * 3600000LL;
  milliseconds += (long long)(timeEdit2->time().minute()) * 60000LL;
  milliseconds += (long long)(timeEdit2->time().second()) * 1000LL;
  milliseconds += (long long)(timeEdit2->time().msec());

  milliseconds += ((long long)days * 86400000LL);

  if(milliseconds<0)  milliseconds = 0;

  milliseconds -= starttime;

  time1.setHMS((int)((milliseconds / 3600000LL) % 24LL), (int)((milliseconds % 3600000LL) / 60000LL), (int)((milliseconds % 60000LL) / 1000LL), (int)(milliseconds % 1000LL));

  timeEdit1->setTime(time1);

  daybox1->setValue((int)(milliseconds / 86400000LL));

  QObject::connect(daybox1,     SIGNAL(valueChanged(int)),          this,        SLOT(offsetday_changed(int)));
  QObject::connect(timeEdit1,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(offsettime_changed(const QTime &)));
}


void UI_JumpMenuDialog::offsettime_changed(const QTime &time_1)
{
  long long milliseconds;

  if(!mainwindow->files_open)  return;

  QObject::disconnect(daybox2,     SIGNAL(valueChanged(int)),          this,        SLOT(absoluteday_changed(int)));
  QObject::disconnect(timeEdit2,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(absolutetime_changed(const QTime &)));

  milliseconds = (long long)(time_1.hour()) * 3600000LL;
  milliseconds += (long long)(time_1.minute()) * 60000LL;
  milliseconds += (long long)(time_1.second()) * 1000LL;
  milliseconds += (long long)(time_1.msec());

  milliseconds += ((long long)daybox1->value() * 86400000LL);

  milliseconds += starttime;

  time2.setHMS((int)((milliseconds / 3600000LL) % 24LL), (int)((milliseconds % 3600000LL) / 60000LL), (int)((milliseconds % 60000LL) / 1000LL), (int)(milliseconds % 1000LL));

  timeEdit2->setTime(time2);

  daybox2->setValue((int)(milliseconds / 86400000LL));

  QObject::connect(daybox2,     SIGNAL(valueChanged(int)),          this,        SLOT(absoluteday_changed(int)));
  QObject::connect(timeEdit2,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(absolutetime_changed(const QTime &)));
}


void UI_JumpMenuDialog::absolutetime_changed(const QTime &time_2)
{
  long long milliseconds;

  if(!mainwindow->files_open)  return;

  QObject::disconnect(daybox1,     SIGNAL(valueChanged(int)),          this,        SLOT(offsetday_changed(int)));
  QObject::disconnect(timeEdit1,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(offsettime_changed(const QTime &)));

  milliseconds = (long long)(time_2.hour()) * 3600000LL;
  milliseconds += (long long)(time_2.minute()) * 60000LL;
  milliseconds += (long long)(time_2.second()) * 1000LL;
  milliseconds += (long long)(time_2.msec());

  milliseconds += ((long long)daybox2->value() * 86400000LL);

  if(milliseconds<0)  milliseconds = 0;

  milliseconds -= starttime;

  time1.setHMS((int)((milliseconds / 3600000LL) % 24LL), (int)((milliseconds % 3600000LL) / 60000LL), (int)((milliseconds % 60000LL) / 1000LL), (int)(milliseconds % 1000LL));

  timeEdit1->setTime(time1);

  daybox1->setValue((int)(milliseconds / 86400000LL));

  QObject::connect(daybox1,     SIGNAL(valueChanged(int)),          this,        SLOT(offsetday_changed(int)));
  QObject::connect(timeEdit1,   SIGNAL(timeChanged(const QTime &)), this,        SLOT(offsettime_changed(const QTime &)));
}


void UI_JumpMenuDialog::jumpbutton_pressed()
{
  int i;

  long long milliseconds;

  if(!mainwindow->files_open)  return;

  milliseconds = (long long)(timeEdit1->time().hour()) * 3600000LL;
  milliseconds += (long long)(timeEdit1->time().minute()) * 60000LL;
  milliseconds += (long long)(timeEdit1->time().second()) * 1000LL;
  milliseconds += (long long)(timeEdit1->time().msec());

  milliseconds += ((long long)daybox1->value() * 86400000LL);

  if(mainwindow->viewtime_sync==VIEWTIME_SYNCED_OFFSET)
  {
    for(i=0; i<mainwindow->files_open; i++)
    {
      mainwindow->edfheaderlist[i]->viewtime = milliseconds * (TIME_DIMENSION / 1000);
    }
  }

  if(mainwindow->viewtime_sync==VIEWTIME_UNSYNCED)
  {
    mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime = milliseconds * (TIME_DIMENSION / 1000);
  }

  if(mainwindow->viewtime_sync==VIEWTIME_SYNCED_ABSOLUT)
  {
    mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime = milliseconds * (TIME_DIMENSION / 1000);

    for(i=0; i<mainwindow->files_open; i++)
    {
      if(i!=mainwindow->sel_viewtime)
      {
        mainwindow->edfheaderlist[i]->viewtime = mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime - ((mainwindow->edfheaderlist[i]->utc_starttime - mainwindow->edfheaderlist[mainwindow->sel_viewtime]->utc_starttime) * TIME_DIMENSION);
      }
    }
  }

  if(mainwindow->viewtime_sync==VIEWTIME_USER_DEF_SYNCED)
  {
    for(i=0; i<mainwindow->files_open; i++)
    {
      if(i!=mainwindow->sel_viewtime)
      {
        mainwindow->edfheaderlist[i]->viewtime -= (mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime - milliseconds * (TIME_DIMENSION / 1000));
      }
    }

    mainwindow->edfheaderlist[mainwindow->sel_viewtime]->viewtime = milliseconds * (TIME_DIMENSION / 1000);
  }

  mainwindow->setup_viewbuf();

  jump_dialog->close();
}





