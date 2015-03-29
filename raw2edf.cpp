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



#include "raw2edf.h"



#if defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)

#define fopeno fopen

#else

#define fseeko fseeko64
#define ftello ftello64
#define fopeno fopen64

#endif





UI_RAW2EDFapp::UI_RAW2EDFapp(struct raw2edf_var_struct *raw2edf_var_p, char *recent_dir, char *save_dir)
{
  recent_opendir = recent_dir;
  recent_savedir = save_dir;

  raw2edf_var = raw2edf_var_p;

  edf_format = 1;

  raw2edfDialog = new QDialog;

  raw2edfDialog->setMinimumSize(600, 480);
  raw2edfDialog->setMaximumSize(600, 480);
  raw2edfDialog->setWindowTitle("Binary/raw data to EDF converter");
  raw2edfDialog->setModal(TRUE);
  raw2edfDialog->setAttribute(Qt::WA_DeleteOnClose, TRUE);

  SamplefreqLabel = new QLabel(raw2edfDialog);
  SamplefreqLabel->setGeometry(20, 20, 140, 20);
  SamplefreqLabel->setText("Samplefrequency");

  SamplefreqSpinbox = new QSpinBox(raw2edfDialog);
  SamplefreqSpinbox->setGeometry(160, 20, 120, 20);
  SamplefreqSpinbox->setRange(1,1000000);
  SamplefreqSpinbox->setSuffix(" Hz");
  SamplefreqSpinbox->setValue(raw2edf_var->sf);
  SamplefreqSpinbox->setToolTip("Samplerate");

  SignalsLabel = new QLabel(raw2edfDialog);
  SignalsLabel->setGeometry(20, 60, 140, 20);
  SignalsLabel->setText("Number of signals");

  SignalsSpinbox = new QSpinBox(raw2edfDialog);
  SignalsSpinbox->setGeometry(160, 60, 120, 20);
  SignalsSpinbox->setRange(1,256);
  SignalsSpinbox->setValue(raw2edf_var->chns);
  SignalsSpinbox->setToolTip("Number of channels");

  SampleSizeLabel = new QLabel(raw2edfDialog);
  SampleSizeLabel->setGeometry(20, 100, 140, 20);
  SampleSizeLabel->setText("Sample size");

  SampleSizeSpinbox = new QSpinBox(raw2edfDialog);
  SampleSizeSpinbox->setGeometry(160, 100, 120, 20);
  SampleSizeSpinbox->setRange(1,2);
  SampleSizeSpinbox->setValue(raw2edf_var->samplesize);
  SampleSizeSpinbox->setSuffix(" byte(s)");
  SampleSizeSpinbox->setToolTip("Bytes per sample");

  OffsetLabel = new QLabel(raw2edfDialog);
  OffsetLabel->setGeometry(20, 140, 140, 20);
  OffsetLabel->setText("Offset");

  OffsetSpinbox = new QSpinBox(raw2edfDialog);
  OffsetSpinbox->setGeometry(160, 140, 120, 20);
  OffsetSpinbox->setRange(0,1000000);
  OffsetSpinbox->setValue(raw2edf_var->offset);
  OffsetSpinbox->setToolTip("Bytes to skip from start of file to data startpoint");

  EncodingLabel = new QLabel(raw2edfDialog);
  EncodingLabel->setGeometry(20, 180, 140, 20);
  EncodingLabel->setText("Encoding");

  EncodingCombobox = new QComboBox(raw2edfDialog);
  EncodingCombobox->setGeometry(160, 180, 120, 20);
  EncodingCombobox->addItem("2's complement");
  EncodingCombobox->addItem("straight binary");
  EncodingCombobox->setCurrentIndex(raw2edf_var->straightbinary);

  skipblocksizeLabel = new QLabel(raw2edfDialog);
  skipblocksizeLabel->setGeometry(320, 20, 140, 20);
  skipblocksizeLabel->setText("Skip blocksize");

  skipblocksizeSpinbox = new QSpinBox(raw2edfDialog);
  skipblocksizeSpinbox->setGeometry(460, 20, 120, 20);
  skipblocksizeSpinbox->setRange(0, 1000000);
  skipblocksizeSpinbox->setValue(raw2edf_var->skipblocksize);
  skipblocksizeSpinbox->setToolTip("Skip after every n bytes");

  skipbytesLabel = new QLabel(raw2edfDialog);
  skipbytesLabel->setGeometry(320, 60, 140, 20);
  skipbytesLabel->setText("Skip bytes");

  skipbytesSpinbox = new QSpinBox(raw2edfDialog);
  skipbytesSpinbox->setGeometry(460, 60, 120, 20);
  skipbytesSpinbox->setRange(1, 1000000);
  skipbytesSpinbox->setValue(raw2edf_var->skipbytes);
  skipbytesSpinbox->setToolTip("Number of bytes that must be skipped");

  PhysicalMaximumLabel = new QLabel(raw2edfDialog);
  PhysicalMaximumLabel->setGeometry(320, 100, 140, 20);
  PhysicalMaximumLabel->setText("Physical maximum");

  PhysicalMaximumSpinbox = new QSpinBox(raw2edfDialog);
  PhysicalMaximumSpinbox->setGeometry(460, 100, 120, 20);
  PhysicalMaximumSpinbox->setRange(1, 10000000);
  PhysicalMaximumSpinbox->setValue(raw2edf_var->phys_max);

  PhysicalDimensionLabel = new QLabel(raw2edfDialog);
  PhysicalDimensionLabel->setGeometry(320, 140, 140, 20);
  PhysicalDimensionLabel->setText("Physical dimension");

  PhysicalDimensionLineEdit = new QLineEdit(raw2edfDialog);
  PhysicalDimensionLineEdit->setGeometry(460, 140, 120, 20);
  PhysicalDimensionLineEdit->setMaxLength(8);
  PhysicalDimensionLineEdit->setText(raw2edf_var->phys_dim);
  PhysicalDimensionLineEdited(PhysicalDimensionLineEdit->text());

  variableTypeLabel = new QLabel(raw2edfDialog);
  variableTypeLabel->setGeometry(320, 180, 140, 20);
  variableTypeLabel->setText("sampletype:     i16");

  PatientnameLabel = new QLabel(raw2edfDialog);
  PatientnameLabel->setGeometry(20, 220, 140, 20);
  PatientnameLabel->setText("Subject name");

  PatientnameLineEdit = new QLineEdit(raw2edfDialog);
  PatientnameLineEdit->setGeometry(160, 220, 420, 20);
  PatientnameLineEdit->setMaxLength(80);

  RecordingLabel = new QLabel(raw2edfDialog);
  RecordingLabel->setGeometry(20, 260, 140, 20);
  RecordingLabel->setText("Recording");

  RecordingLineEdit = new QLineEdit(raw2edfDialog);
  RecordingLineEdit->setGeometry(160, 260, 420, 20);
  RecordingLineEdit->setMaxLength(80);

  DatetimeLabel = new QLabel(raw2edfDialog);
  DatetimeLabel->setGeometry(20, 300, 140, 20);
  DatetimeLabel->setText("Startdate and time");

  StartDatetimeedit = new QDateTimeEdit(raw2edfDialog);
  StartDatetimeedit->setGeometry(160, 300, 180, 20);
  StartDatetimeedit->setDisplayFormat("dd/MM/yyyy hh:mm:ss");
  StartDatetimeedit->setDateTime(QDateTime::currentDateTime());
  StartDatetimeedit->setToolTip("dd/MM/yyyy hh:mm:ss");

  GoButton = new QPushButton(raw2edfDialog);
  GoButton->setGeometry(20, 440, 75, 25);
  GoButton->setText("Start");
  GoButton->setToolTip("Start the conversion");

  CloseButton = new QPushButton(raw2edfDialog);
  CloseButton->setGeometry(160, 440, 75, 25);
  CloseButton->setText("Close");

  SaveButton = new QPushButton(raw2edfDialog);
  SaveButton->setGeometry(400, 440, 75, 25);
  SaveButton->setText("Save");
  SaveButton->setToolTip("Save the entered parameters to a template for later use");

  LoadButton = new QPushButton(raw2edfDialog);
  LoadButton->setGeometry(510, 440, 75, 25);
  LoadButton->setText("Load");
  LoadButton->setToolTip("Load parameters from a template");

  edfsignals = 1;

  QObject::connect(GoButton,                    SIGNAL(clicked()),                this,            SLOT(gobuttonpressed()));
  QObject::connect(CloseButton,                 SIGNAL(clicked()),                raw2edfDialog,   SLOT(close()));
  QObject::connect(SaveButton,                  SIGNAL(clicked()),                this,            SLOT(savebuttonpressed()));
  QObject::connect(LoadButton,                  SIGNAL(clicked()),                this,            SLOT(loadbuttonpressed()));
  QObject::connect(PhysicalDimensionLineEdit,   SIGNAL(textEdited(QString)),      this,            SLOT(PhysicalDimensionLineEdited(QString)));
  QObject::connect(SampleSizeSpinbox,           SIGNAL(valueChanged(int)),        this,            SLOT(sampleTypeChanged(int)));
  QObject::connect(EncodingCombobox,            SIGNAL(currentIndexChanged(int)), this,            SLOT(sampleTypeChanged(int)));

  sampleTypeChanged(0);

  raw2edfDialog->exec();
}


void UI_RAW2EDFapp::gobuttonpressed()
{
  int i, j, k, r,
      hdl,
      chns,
      sf,
      *buf,
      datarecords,
      tmp,
      straightbinary,
      samplesize,
      skipblocksize,
      skipbytes,
      skipblockcntr,
      bytecntr;

  char str[256],
       path[MAX_PATH_LENGTH];

  double phys_max;

  long long d_offset;

  FILE *inputfile;


  sf = SamplefreqSpinbox->value();
  raw2edf_var->sf = sf;

  chns = SignalsSpinbox->value();
  raw2edf_var->chns = chns;

  phys_max = PhysicalMaximumSpinbox->value();
  raw2edf_var->phys_max = phys_max;

  straightbinary = EncodingCombobox->currentIndex();
  raw2edf_var->straightbinary = straightbinary;

  samplesize = SampleSizeSpinbox->value();
  raw2edf_var->samplesize = samplesize;

  d_offset = OffsetSpinbox->value();
  raw2edf_var->offset = d_offset;

  skipblocksize = skipblocksizeSpinbox->value();
  raw2edf_var->skipblocksize = skipblocksize;

  skipbytes = skipbytesSpinbox->value();
  raw2edf_var->skipbytes = skipbytes;

  strcpy(raw2edf_var->phys_dim, PhysicalDimensionLineEdit->text().toLatin1().data());
  remove_leading_spaces(raw2edf_var->phys_dim);
  remove_trailing_spaces(raw2edf_var->phys_dim);

  if(!(strlen(PatientnameLineEdit->text().toLatin1().data())))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Please enter a subjectname.");
    messagewindow.exec();
    return;
  }

  if(!(strlen(RecordingLineEdit->text().toLatin1().data())))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Invalid input", "Please enter a recording description.");
    messagewindow.exec();
    return;
  }

  strcpy(path, QFileDialog::getOpenFileName(0, "Open data file", QString::fromLocal8Bit(recent_opendir), "All files (*)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    return;
  }

  get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);

  inputfile = fopeno(path, "rb");
  if(inputfile==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for reading.");
    messagewindow.exec();
    return;
  }

  remove_extension_from_filename(path);
  strcat(path, ".edf");

  hdl = edfopen_file_writeonly(path, EDFLIB_FILETYPE_EDFPLUS, chns);

  if(hdl<0)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for writing.\nedfopen_file_writeonly()");
    messagewindow.exec();
    fclose(inputfile);
    return;
  }

  for(i=0; i<chns; i++)
  {
    if(edf_set_samplefrequency(hdl, i, sf))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_samplefrequency()");
      messagewindow.exec();
      fclose(inputfile);
      return;
    }
  }

  if(samplesize == 2)
  {
    for(i=0; i<chns; i++)
    {
      if(edf_set_digital_maximum(hdl, i, 32767))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_digital_maximum()");
        messagewindow.exec();
        fclose(inputfile);
        return;
      }
    }

    for(i=0; i<chns; i++)
    {
      if(edf_set_digital_minimum(hdl, i, -32768))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_digital_minimum()");
        messagewindow.exec();
        fclose(inputfile);
        return;
      }
    }
  }

  if(samplesize == 1)
  {
    for(i=0; i<chns; i++)
    {
      if(edf_set_digital_maximum(hdl, i, 255))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_digital_maximum()");
        messagewindow.exec();
        fclose(inputfile);
        return;
      }
    }

    for(i=0; i<chns; i++)
    {
      if(edf_set_digital_minimum(hdl, i, -256))
      {
        QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_digital_minimum()");
        messagewindow.exec();
        fclose(inputfile);
        return;
      }
    }
  }

  for(i=0; i<chns; i++)
  {
    if(edf_set_physical_maximum(hdl, i, phys_max))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_physical_maximum()");
      messagewindow.exec();
      fclose(inputfile);
      return;
    }
  }

  for(i=0; i<chns; i++)
  {
    if(edf_set_physical_minimum(hdl, i, -phys_max))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_physical_minimum()");
      messagewindow.exec();
      fclose(inputfile);
      return;
    }
  }

  for(i=0; i<chns; i++)
  {
    if(edf_set_physical_dimension(hdl, i, raw2edf_var->phys_dim))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_physical_dimension()");
      messagewindow.exec();
      fclose(inputfile);
      return;
    }
  }

  for(i=0; i<chns; i++)
  {
    sprintf(str, "ch. %i", i + 1);

    if(edf_set_label(hdl, i, str))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_label()");
      messagewindow.exec();
      fclose(inputfile);
      return;
    }
  }

  if(edf_set_startdatetime(hdl, StartDatetimeedit->date().year(), StartDatetimeedit->date().month(), StartDatetimeedit->date().day(),
                                StartDatetimeedit->time().hour(), StartDatetimeedit->time().minute(), StartDatetimeedit->time().second()))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "edf_set_startdatetime()");
    messagewindow.exec();
    fclose(inputfile);
    edfclose_file(hdl);
    return;
  }

  buf = (int *)malloc(sizeof(int) * sf * chns);
  if(buf == NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "malloc()");
    messagewindow.exec();
    fclose(inputfile);
    edfclose_file(hdl);
    return;
  }

//   printf("samplefrequency:  %14i Hz\n"
//          "channels:         %14i\n"
//          "physical maximum: %14.6f uV\n"
//          "straightbinary:   %14i\n"
//          "samplesize:       %14i byte(s)\n"
//          "offset:           %14lli bytes\n"
//          "skip blocksize:   %14i bytes\n"
//          "skip bytes:       %14i bytes\n\n",
//          sf,
//          chns,
//          phys_max,
//          straightbinary,
//          samplesize,
//          d_offset,
//          skipblocksize,
//          skipbytes);

  fseeko(inputfile, d_offset, SEEK_SET);

  datarecords = 0;

  union{
         int one[1];
         signed short two[2];
         char four[4];
       } var;

  skipblockcntr = 0;

  bytecntr = 0;

  while(1)
  {
    for(j=0; j<sf; j++)
    {
      for(k=0; k<chns; k++)
      {

//         tmp = fgetc(inputfile);
//         if(tmp == EOF)
//         {
//           edfclose_file(hdl);
//           fclose(inputfile);
//           free(buf);
//           return;
//         }
//
//         tmp += (fgetc(inputfile) * 256);
//
//         buf[j + (k * sf)] = tmp - 32768;

        if(samplesize == 2)
        {
          tmp = fgetc(inputfile);
          if(tmp == EOF)
          {
            edfclose_file(hdl);
            fclose(inputfile);
            free(buf);
            return;
          }
          bytecntr++;

//           printf("1: skipblockcntr is %i    tmp is %02X   bytecntr is %i\n", skipblockcntr, tmp, bytecntr);

          if(skipblocksize)
          {
            if(++skipblockcntr > skipblocksize)
            {
              for(r=0; r<skipbytes; r++)
              {
                tmp = fgetc(inputfile);
                if(tmp == EOF)
                {
                  edfclose_file(hdl);
                  fclose(inputfile);
                  free(buf);
                  return;
                }
//                bytecntr++;

//                printf("2: skipblockcntr is %i    tmp is %02X   bytecntr is %i\n", skipblockcntr, tmp, bytecntr);

              }

              skipblockcntr = 1;
            }
          }

          var.four[0] = tmp;
        }

        if(samplesize == 1)
        {
          var.four[0] = 0;
        }

        tmp = fgetc(inputfile);
        if(tmp == EOF)
        {
          edfclose_file(hdl);
          fclose(inputfile);
          free(buf);
          return;
        }
//         bytecntr++;

//         printf("3: skipblockcntr is %i    tmp is %02X   bytecntr is %i\n", skipblockcntr, tmp, bytecntr);

        if(skipblocksize)
        {
          if(++skipblockcntr > skipblocksize)
          {
            for(r=0; r<skipbytes; r++)
            {
              tmp = fgetc(inputfile);
              if(tmp == EOF)
              {
                edfclose_file(hdl);
                fclose(inputfile);
                free(buf);
                return;
              }
              bytecntr++;

//               printf("4: skipblockcntr is %i    tmp is %02X   bytecntr is %i\n", skipblockcntr, tmp, bytecntr);

            }

            skipblockcntr = 1;
          }
        }

        var.four[1] = tmp;

        if(straightbinary)
        {
          var.two[0] -= 32768;
        }

        if(samplesize == 1)
        {
          var.two[0] >>= 8;
        }

        buf[j + (k * sf)] = var.two[0];
      }
    }

    if(edf_blockwrite_digital_samples(hdl, buf))
    {
      QMessageBox messagewindow(QMessageBox::Critical, "Error", "Write error during conversion.\nedf_blockwrite_digital_samples()");
      messagewindow.exec();
      edfclose_file(hdl);
      fclose(inputfile);
      free(buf);
      return;
    }

    datarecords++;
//    if(datarecords == 1)  break;
  }

  edfclose_file(hdl);
  fclose(inputfile);
  free(buf);

  sprintf(str, "Ready, %i datarecords written.", datarecords);
  QMessageBox messagewindow(QMessageBox::NoIcon, "Done", str);
  messagewindow.exec();
}



void UI_RAW2EDFapp::savebuttonpressed()
{
  char path[MAX_PATH_LENGTH],
       str[128];

  FILE *outputfile;


  raw2edf_var->sf = SamplefreqSpinbox->value();

  raw2edf_var->chns = SignalsSpinbox->value();

  raw2edf_var->phys_max = PhysicalMaximumSpinbox->value();

  raw2edf_var->straightbinary = EncodingCombobox->currentIndex();

  raw2edf_var->samplesize = SampleSizeSpinbox->value();

  raw2edf_var->offset = OffsetSpinbox->value();

  raw2edf_var->skipblocksize = skipblocksizeSpinbox->value();

  raw2edf_var->skipbytes = skipbytesSpinbox->value();

  strcpy(raw2edf_var->phys_dim, PhysicalDimensionLineEdit->text().toLatin1().data());
  remove_leading_spaces(raw2edf_var->phys_dim);
  remove_trailing_spaces(raw2edf_var->phys_dim);

  path[0] = 0;
  if(recent_savedir[0]!=0)
  {
    strcpy(path, recent_savedir);
    strcat(path, "/");
  }
  strcat(path, "binary_to_edf.template");

  strcpy(path, QFileDialog::getSaveFileName(0, "Save parameters", QString::fromLocal8Bit(path), "Template files (*.template *.TEMPLATE)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    return;
  }

  get_directory_from_path(recent_savedir, path, MAX_PATH_LENGTH);

  outputfile = fopeno(path, "wb");
  if(outputfile==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for writing.");
    messagewindow.exec();
    return;
  }

  fprintf(outputfile, "<?xml version=\"1.0\"?>\n<" PROGRAM_NAME "_raw2edf_template>\n");

  fprintf(outputfile, "  <sf>%i</sf>\n", raw2edf_var->sf);

  fprintf(outputfile, "  <chns>%i</chns>\n", raw2edf_var->chns);

  fprintf(outputfile, "  <phys_max>%i</phys_max>\n", raw2edf_var->phys_max);

  fprintf(outputfile, "  <straightbinary>%i</straightbinary>\n", raw2edf_var->straightbinary);

  fprintf(outputfile, "  <samplesize>%i</samplesize>\n", raw2edf_var->samplesize);

  fprintf(outputfile, "  <offset>%i</offset>\n", raw2edf_var->offset);

  fprintf(outputfile, "  <skipblocksize>%i</skipblocksize>\n", raw2edf_var->skipblocksize);

  fprintf(outputfile, "  <skipbytes>%i</skipbytes>\n", raw2edf_var->skipbytes);

  xml_strcpy_encode_entity(str, raw2edf_var->phys_dim);

  fprintf(outputfile, "  <phys_dim>%s</phys_dim>\n", str);

  fprintf(outputfile, "</" PROGRAM_NAME "_raw2edf_template>\n");

  fclose(outputfile);
}


void UI_RAW2EDFapp::loadbuttonpressed()
{

  char path[MAX_PATH_LENGTH],
       *result;

  struct xml_handle *xml_hdl;

  strcpy(path, QFileDialog::getOpenFileName(0, "Load parameters", QString::fromLocal8Bit(recent_opendir), "Template files (*.template *.TEMPLATE)").toLocal8Bit().data());

  if(!strcmp(path, ""))
  {
    return;
  }

  get_directory_from_path(recent_opendir, path, MAX_PATH_LENGTH);

  xml_hdl = xml_get_handle(path);
  if(xml_hdl==NULL)
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "Can not open file for reading.");
    messagewindow.exec();
    return;
  }

  if(strcmp(xml_hdl->elementname, PROGRAM_NAME "_raw2edf_template"))
  {
    QMessageBox messagewindow(QMessageBox::Critical, "Error", "There seems to be an error in this template.");
    messagewindow.exec();
    xml_close(xml_hdl);
    return;
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "sf", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->sf = atoi(result);
    if(raw2edf_var->sf < 1)  raw2edf_var->sf = 1;
    if(raw2edf_var->sf > 1000000)  raw2edf_var->sf = 1000000;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "chns", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->chns = atoi(result);
    if(raw2edf_var->chns < 1)  raw2edf_var->chns = 1;
    if(raw2edf_var->chns > 256)  raw2edf_var->chns = 256;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "phys_max", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->phys_max = atoi(result);
    if(raw2edf_var->phys_max < 1)  raw2edf_var->phys_max = 1;
    if(raw2edf_var->phys_max > 10000000)  raw2edf_var->phys_max = 10000000;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "straightbinary", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->straightbinary = atoi(result);
    if(raw2edf_var->straightbinary < 0)  raw2edf_var->straightbinary = 0;
    if(raw2edf_var->straightbinary > 1)  raw2edf_var->straightbinary = 1;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "samplesize", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->samplesize = atoi(result);
    if(raw2edf_var->samplesize < 1)  raw2edf_var->samplesize = 1;
    if(raw2edf_var->samplesize > 2)  raw2edf_var->samplesize = 2;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "offset", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->offset = atoi(result);
    if(raw2edf_var->offset < 0)  raw2edf_var->offset = 0;
    if(raw2edf_var->offset > 1000000)  raw2edf_var->offset = 1000000;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "skipblocksize", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->skipblocksize = atoi(result);
    if(raw2edf_var->skipblocksize < 0)  raw2edf_var->skipblocksize = 0;
    if(raw2edf_var->skipblocksize > 1000000)  raw2edf_var->skipblocksize = 1000000;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "skipbytes", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    raw2edf_var->skipbytes = atoi(result);
    if(raw2edf_var->skipbytes < 1)  raw2edf_var->skipbytes = 1;
    if(raw2edf_var->skipbytes > 1000000)  raw2edf_var->skipbytes = 1000000;

    free(result);

    xml_go_up(xml_hdl);
  }

  if(!(xml_goto_nth_element_inside(xml_hdl, "phys_dim", 0)))
  {
    result = xml_get_content_of_element(xml_hdl);
    if(result==NULL)
    {
      xml_close(xml_hdl);
      return;
    }

    strncpy(raw2edf_var->phys_dim, result, 16);
    raw2edf_var->phys_dim[15] = 0;
    latin1_to_ascii(raw2edf_var->phys_dim, 16);
    remove_leading_spaces(raw2edf_var->phys_dim);
    remove_trailing_spaces(raw2edf_var->phys_dim);

    free(result);

    xml_go_up(xml_hdl);
  }

  xml_close(xml_hdl);

  SamplefreqSpinbox->setValue(raw2edf_var->sf);

  SignalsSpinbox->setValue(raw2edf_var->chns);

  PhysicalMaximumSpinbox->setValue(raw2edf_var->phys_max);

  EncodingCombobox->setCurrentIndex(raw2edf_var->straightbinary);

  SampleSizeSpinbox->setValue(raw2edf_var->samplesize);

  OffsetSpinbox->setValue(raw2edf_var->offset);

  skipblocksizeSpinbox->setValue(raw2edf_var->skipblocksize);

  skipbytesSpinbox->setValue(raw2edf_var->skipbytes);
}


void UI_RAW2EDFapp::sampleTypeChanged(int)
{
  if(SampleSizeSpinbox->value() == 1)
  {
    if(EncodingCombobox->currentIndex() == 0)
    {
      variableTypeLabel->setText("sampletype:     I8");
    }
    else
    {
      variableTypeLabel->setText("sampletype:     U8");
    }
  }
  else
  {
    if(EncodingCombobox->currentIndex() == 0)
    {
      variableTypeLabel->setText("sampletype:     I16");
    }
    else
    {
      variableTypeLabel->setText("sampletype:     U16");
    }
  }
}


void UI_RAW2EDFapp::PhysicalDimensionLineEdited(QString qstr)
{
  qstr.prepend(" ");

  PhysicalMaximumSpinbox->setSuffix(qstr);
}













