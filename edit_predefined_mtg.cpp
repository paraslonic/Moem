/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2010, 2011, 2012, 2013 Teunis van Beelen
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



#include "edit_predefined_mtg.h"



UI_edit_predefined_mtg_window::UI_edit_predefined_mtg_window(QWidget *w_parent)
{
  int i;

  char str[1200];

  mainwindow = (UI_Mainwindow *)w_parent;

  edit_predefined_mtg_Dialog = new QDialog;

  edit_predefined_mtg_Dialog->setMinimumSize(QSize(800, 205));
  edit_predefined_mtg_Dialog->setMaximumSize(QSize(800, 205));
  edit_predefined_mtg_Dialog->setWindowTitle("Edit key-bindings for montages");
  edit_predefined_mtg_Dialog->setModal(TRUE);
  edit_predefined_mtg_Dialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  mtg_path_list = new QListWidget(edit_predefined_mtg_Dialog);
  mtg_path_list->setGeometry(QRect(10, 10, 780, 150));
  mtg_path_list->setSelectionBehavior(QAbstractItemView::SelectRows);
  mtg_path_list->setSelectionMode(QAbstractItemView::SingleSelection);
  mtg_path_list->setSpacing(1);
  for(i=0; i < MAXPREDEFINEDMONTAGES; i++)
  {
    sprintf(str, "F%i : ", i + 1);

    if(mainwindow->predefined_mtg_path[i][0] != 0)
    {
      strcat(str, &(mainwindow->predefined_mtg_path[i][0]));
    }

    new QListWidgetItem(str, mtg_path_list);
  }

  CloseButton = new QPushButton(edit_predefined_mtg_Dialog);
  CloseButton->setGeometry(QRect(690, 170, 100, 25));
  CloseButton->setText("Close");

  QObject::connect(CloseButton,   SIGNAL(clicked()),                      edit_predefined_mtg_Dialog,    SLOT(close()));
  QObject::connect(mtg_path_list, SIGNAL(itemClicked(QListWidgetItem *)), this,                          SLOT(rowClicked(QListWidgetItem *)));

  edit_predefined_mtg_Dialog->exec();
}


void UI_edit_predefined_mtg_window::rowClicked(QListWidgetItem *item)
{
  row = mtg_path_list->row(item);

  listItem = item;

  dialog = new QDialog(edit_predefined_mtg_Dialog);
  dialog->setMinimumSize(QSize(140, 135));
  dialog->setMaximumSize(QSize(140, 135));
  dialog->setWindowTitle("Entry");
  dialog->setModal(TRUE);
  dialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  button1 = new QPushButton(dialog);
  button1->setGeometry(20, 20, 100, 25);
  button1->setText("Change");

  button2 = new QPushButton(dialog);
  button2->setGeometry(20, 55, 100, 25);
  button2->setText("Remove");

  button3 = new QPushButton(dialog);
  button3->setGeometry(20, 90, 100, 25);
  button3->setText("Cancel");

  QObject::connect(button1, SIGNAL(clicked()), this,   SLOT(adEntry()));
  QObject::connect(button2, SIGNAL(clicked()), this,   SLOT(removeEntry()));
  QObject::connect(button3, SIGNAL(clicked()), dialog, SLOT(close()));

  dialog->exec();
}


void UI_edit_predefined_mtg_window::adEntry()
{
  char str[1200],
       path[MAX_PATH_LENGTH];


  dialog->close();

  strcpy(path, QFileDialog::getOpenFileName(0, "Select montage", QString::fromLocal8Bit(mainwindow->recent_montagedir), "MTG files (*.mtg *.MTG)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    return;
  }

  get_directory_from_path(mainwindow->recent_montagedir, path, MAX_PATH_LENGTH);

  strcpy(&(mainwindow->predefined_mtg_path[row][0]), path);

  mainwindow->load_predefined_mtg_act[row]->setText(QString::fromLocal8Bit(mainwindow->predefined_mtg_path[row]));

  sprintf(str, "F%i : ", row + 1);
  strcat(str, path);

  listItem->setText(str);
}


void UI_edit_predefined_mtg_window::removeEntry()
{
  char str[32];

  dialog->close();

  mainwindow->predefined_mtg_path[row][0] = 0;
  mainwindow->load_predefined_mtg_act[row]->setText("Empty");

  sprintf(str, "F%i :", row + 1);

  listItem->setText(str);
}

















