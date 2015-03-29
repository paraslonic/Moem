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




#ifndef ANNOTATIONSFORM1_H
#define ANNOTATIONSFORM1_H



#include <QApplication>
#include <QObject>
#include <QDockWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QByteArray>
#include <QPalette>
#include <QTime>
#include <QTimeEdit>
#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QAction>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "mainwindow.h"
#include "edit_annotation_dock.h"
#include "viewcurve.h"
#include "utils.h"
#include "averager_dialog.h"


class UI_Mainwindow;




class UI_Annotationswindow : public QObject
{
  Q_OBJECT

public:
  UI_Annotationswindow(int, QWidget *parent);

  UI_Mainwindow *mainwindow;

  QDockWidget  *docklist;

  QListWidget  *list;

  void updateList(void);

private:

  struct annotationblock *annotation;

  int file_num,
      relative,
      selected;

  QDialog *dialog1;

  QVBoxLayout *v_layout;

  QCheckBox *checkbox1;

  QAction *show_between_act,
          *average_annot_act,
          *hide_annot_act,
          *unhide_annot_act,
          *hide_same_annots_act,
          *unhide_same_annots_act,
          *unhide_all_annots_act;

private slots:

  void annotation_selected(QListWidgetItem *, int centered=1);
  void hide_editdock(bool);
  void checkbox1_clicked(int);
  void show_between(bool);
  void average_annot(bool);
  void hide_annot(bool);
  void unhide_annot(bool);
  void hide_same_annots(bool);
  void unhide_same_annots(bool);
  void unhide_all_annots(bool);
};



#endif // ANNOTATIONSFORM1_H


