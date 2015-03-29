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



#ifndef fouriertransform_INCLUDED
#define fouriertransform_INCLUDED



#include <math.h>



#ifdef __cplusplus
extern "C" {
#endif


float fourier_transform_get_power_float(int freq, float *buf, int bufsize, float samplefreq);
double fourier_transform_get_power_double(int freq, double *buf, int bufsize, double samplefreq);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif












