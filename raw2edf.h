/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2012, 2013 Teunis van Beelen
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


#ifndef UI_RAW2EDFFORM_H
#define UI_RAW2EDFFORM_H


#include <QApplication>
#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QString>
#include <QFileDialog>
#include <QCursor>
#include <QCheckBox>
#include <QMessageBox>
#include <QPixmap>
#include <QToolTip>
#include <QComboBox>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "popup_messagewindow.h"
#include "xml.h"
#include "utils.h"
#include "edflib.h"
#include "global.h"


  struct raw2edf_var_struct{
          int sf;
          int chns;
          int phys_max;
          int straightbinary;
          int samplesize;
          int offset;
          int skipblocksize;
          int skipbytes;
          char phys_dim[16];
        };




class UI_RAW2EDFapp : public QObject
{
  Q_OBJECT

public:
  UI_RAW2EDFapp(struct raw2edf_var_struct *, char *recent_dir=NULL, char *save_dir=NULL);

private:

QDialog       *raw2edfDialog;

QLabel        *SignalsLabel,
              *OffsetLabel,
              *SamplefreqLabel,
              *PatientnameLabel,
              *RecordingLabel,
              *DatetimeLabel,
              *SampleSizeLabel,
              *skipblocksizeLabel,
              *skipbytesLabel,
              *PhysicalMaximumLabel,
              *PhysicalDimensionLabel,
              *EncodingLabel,
              *variableTypeLabel;

QLineEdit     *PatientnameLineEdit,
              *RecordingLineEdit,
              *PhysicalDimensionLineEdit;

QSpinBox      *SignalsSpinbox,
              *OffsetSpinbox,
              *SamplefreqSpinbox,
              *SampleSizeSpinbox,
              *skipblocksizeSpinbox,
              *skipbytesSpinbox,
              *PhysicalMaximumSpinbox;

QComboBox     *EncodingCombobox;

QDateTimeEdit *StartDatetimeedit;

QPushButton   *GoButton,
              *CloseButton,
              *SaveButton,
              *LoadButton;

char *recent_opendir,
     *recent_savedir;

int edfsignals,
    offset,
    samplefrequency,
    edf_format;

struct raw2edf_var_struct *raw2edf_var;

private slots:

void gobuttonpressed();
void savebuttonpressed();
void loadbuttonpressed();
void PhysicalDimensionLineEdited(QString);
void sampleTypeChanged(int);

};




#endif


