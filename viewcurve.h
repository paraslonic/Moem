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



#ifndef VIEWCURVE_H
#define VIEWCURVE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDialog>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPrinter>
#include <QPixmap>
#include <QPrintDialog>
#include <QColor>
#include <QPen>
#include <QMessageBox>
#include <QString>
#include <QThread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global.h"
#include "mainwindow.h"
#include "colordialog.h"
#include "filter.h"
#include "popup_messagewindow.h"
#include "spectrumanalyzer.h"
#include "utils.h"
#include "adjustfiltersettings.h"
#include "ravg_filter.h"
#include "ecg_filter.h"
#include "statistics_dialog.h"
#include "z_score_dialog.h"
#include "z_ratio_filter.h"

#include "third_party/fidlib/fidlib.h"
#include "paraslonic/clever_scoring.h"


class UI_Mainwindow;



class drawCurve_stage_1_thread : public QThread
{
public:
  void init_vars(UI_Mainwindow *, struct signalcompblock **, int,
                 int, char *, int, int, int *, int,
                 struct graphicBufStruct *, double,
                 double, struct crossHairStruct *,
                 struct crossHairStruct *, int);


private:

  int i, h, w,
      signalcomps,
      *screensamples,
      *screensamples_b,
      printing,
      cpu_cnt;

  char *viewbuf;

  double printsize_x_factor,
         printsize_y_factor;

  UI_Mainwindow *mainwindow;

  struct signalcompblock *signalcomp, **signalcomp_b;

  struct graphicBufStruct *graphicBuf;

  struct crossHairStruct *crosshair_1,
                         *crosshair_2;

void run();

};



class ViewCurve : public QWidget
{
  Q_OBJECT

public:
  ViewCurve(QWidget *parent=0);
  ~ViewCurve();

  UI_Mainwindow *mainwindow;

  int use_move_events,
      mouse_x,
      mouse_y,
      mouse_old_x,
      mouse_old_y,
      sidemenu_active,
      signal_nr,
      mouse_press_coordinate_x,
      mouse_press_coordinate_y,
      draw_zoom_rectangle,
      signal_color,
      floating_ruler_color,
      ruler_active,
      ruler_moving,
      ruler_x_position,
      ruler_y_position,
      *screensamples,
      graphicBufWidth,
      blackwhite_printing,
      backup_color_10[MAXSIGNALS],
      backup_color_11,
      backup_color_12,
      backup_color_14,
      annot_marker_moving,
      pressed_on_label,
      label_press_y_position,
	  time_cursor_x,
	  time_cursor_set,
	  time_cursor_moving,
      cpu_cnt;
	long long time_cursor_time;

  double original_screen_offset;

  struct graphicBufStruct *graphicBuf,
                          *printBuf;

  struct active_markersblock *active_markers;
  struct annotationblock *current_annotation;
  struct crossHairStruct crosshair_1,
                         crosshair_2;

  QColor backgroundcolor,
         small_ruler_color,
         big_ruler_color,
         mouse_rect_color,
         baseline_color,
         text_color,
         annot_marker_color,
         backup_color_1,
         backup_color_2,
         backup_color_3,
         backup_color_4,
         backup_color_5,
         backup_color_13,
         backup_color_15,
		 time_cursor_color,
		 rem_color, nrem_color,
		 wake_color;

  QFont *printfont;

  void drawCurve_stage_1(QPainter *painter=NULL, int w_width=0, int w_height=0, int print_linewidth=0);
  void setCrosshair_1_center(void);

  

  bool isAnnotationScoring(struct annotationblock *annot);

public slots:

  void exec_sidemenu(int);
  void print_to_postscript();
  void print_to_pdf();
  void print_to_image(int, int);
  void print_to_printer();


private:

  QDialog     *sidemenu;

  QPushButton *sidemenuButton1,
              *sidemenuButton2,
              *sidemenuButton3,
              *sidemenuButton4,
              *sidemenuButton5,
              *sidemenuButton6,
              *sidemenuButton7,
              *sidemenuButton8,
              *sidemenuButton9,
              *sidemenuButton10,
              *sidemenuButton11,
              *sidemenuButton12,
              *sidemenuButton13;

  QDoubleSpinBox *ScaleBox,
                 *ScaleBox2;

  QLabel      *SidemenuLabel,
              *AmpLabel,
              *OffsetLabel;

  QPrinter    *printer;

  QPen        *special_pen,
              *annot_marker_pen,
              *signal_pen,
              *ruler_pen,
			  *time_cursor_pen,
			  *score_pen; //s.

  int         printing,
              w,
              h,
              floating_ruler_value;

  double      *original_sensitivity,
              painter_pixelsizefactor,
              printsize_x_factor,
              printsize_y_factor;

  drawCurve_stage_1_thread *thr[MAXSIGNALS];

  inline void floating_ruler(QPainter *, int, int, struct signalcompblock *, int);

  void backup_colors_for_printing(void);
  void restore_colors_after_printing(void);

protected slots:

  void RulerButton();
  void FittopaneButton();
  void ScaleBoxChanged(double);
  void ScaleBox2Changed(double);
  void RemovefilterButton();
  void RemovesignalButton();
  void ColorButton();
  void CrosshairButton();
  void FreqSpecButton();
  void Z_scoringButton();
  void AdjustFilterButton();
  void StatisticsButton();
  void ECGdetectButton();
  void signalInvert();
  void strip_types_from_label(char *);

protected:
  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);
  void drawCurve_stage_2(QPainter *painter, int w_width=0, int w_height=0, int print_linewidth=0);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);

  // ps. 4 scoring
  bool PWannotating;
  SPW current_sPW;
};



#endif


