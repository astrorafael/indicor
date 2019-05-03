/*
    Adapted from:

    INDI LIB
    Common routines used by all drivers
    Copyright (C) 2003 by Jason Harris (jharris@30doradus.org)
                          Elwood C. Downey

    This is the C version of the astronomical library in KStars
    modified by Jasem Mutlaq (mutlaqja@ikarustech.com)

    Minor mods made by Rafael Gonzalez Fuentetaja (astrorafael@yahoo.es)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#include "indicom.h"

/* sprint the variable a in sexagesimal format into out[].
 * w is the number of spaces for the whole part.
 * fracbase is the number of pieces a whole is to broken into; valid options:
 *	360000:	<w>:mm:ss.ss
 *	36000:	<w>:mm:ss.s
 *	3600:	<w>:mm:ss
 *	600:	<w>:mm.m
 *	60:	<w>:mm
 * return number of characters written to out, not counting final '\0'.
 */

/*---------------------------------------------------------------------------*/

int
fs_sexa (char *out, double a, int w, int fracbase)
{
  char *out0 = out;
  unsigned long n;
  int d;
  int f;
  int m;
  int s;
  int isneg;
  
  /* save whether it's negative but do all the rest with a positive */
  isneg = (a < 0);
  if (isneg)
    a = -a;
  
  /* convert to an integral number of whole portions */
  n = (unsigned long)(a * fracbase + 0.5);
  d = n/fracbase;
  f = n%fracbase;
  
  /* form the whole part; "negative 0" is a special case */
  if (isneg && d == 0)
    out += sprintf (out, "%*s-0", w-2, "");
  else
    out += sprintf (out, "%*d", w, isneg ? -d : d);

  assert(fracbase == 60 || fracbase == 600 || fracbase == 3600 || fracbase == 36000 || fracbase == 360000);
  
  /* do the rest */
  switch (fracbase) {
  case 60:	/* dd:mm */
    m = f/(fracbase/60);
    out += sprintf (out, ":%02d", m);
    break;
  case 600:	/* dd:mm.m */
    out += sprintf (out, ":%02d.%1d", f/10, f%10);
    break;
  case 3600:	/* dd:mm:ss */
    m = f/(fracbase/60);
    s = f%(fracbase/60);
    out += sprintf (out, ":%02d:%02d", m, s);
    break;
  case 36000:	/* dd:mm:ss.s*/
    m = f/(fracbase/60);
    s = f%(fracbase/60);
    out += sprintf (out, ":%02d:%02d.%1d", m, s/10, s%10);
    break;
  case 360000:	/* dd:mm:ss.ss */
    m = f/(fracbase/60);
    s = f%(fracbase/60);
    out += sprintf (out, ":%02d:%02d.%02d", m, s/100, s%100);
    break;
  default:
    break;
  }
  
  return (out - out0);
}

/* convert sexagesimal string str AxBxC to double.
 *   x can be anything non-numeric. Any missing A, B or C will be assumed 0.
 *   optional - and + can be anywhere.
 * return 0 if ok, -1 if can't find a thing.
 */

/*---------------------------------------------------------------------------*/

int
f_scansexa (const char* str0, double* dp)		
{
  double a = 0, b = 0, c = 0;
  char str[128];
  char *neg;
  int r;
  
  /* copy str0 so we can play with it */
  strncpy (str, str0, sizeof(str)-1);
  str[sizeof(str)-1] = '\0';
  
  neg = strchr(str, '-');
  if (neg)
    *neg = ' ';
  r = sscanf (str, "%lf%*[^0-9]%lf%*[^0-9]%lf", &a, &b, &c);
  if (r < 1)
    return (-1);
  *dp = a + b/60 + c/3600;
  if (neg)
    *dp *= -1;
  return (0);
}

/*---------------------------------------------------------------------------*/

void getSexComponents(double value, int *d, int *m, int *s)
{
  
  *d = (int) fabs(value);
  *m = (int) ((fabs(value) - *d) * 60.0);
  *s = (int) (((fabs(value) - *d) * 60.0 - *m) *60.0);
  
  if (value < 0)
    *d *= -1;
}

/*---------------------------------------------------------------------------*/

/* fill buf with properly formatted INumber string. return length */
int
numberFormat (char *buf, const char *format, double value)
{
  int w, f, s;
  char m;
  
  if (sscanf (format, "%%%d.%d%c", &w, &f, &m) == 3 && m == 'm') {
    /* INDI sexi format */
    switch (f) {
    case 9:  s = 360000; break;
    case 8:  s = 36000;  break;
    case 6:  s = 3600;   break;
    case 5:  s = 600;    break;
    default: s = 60;     break;
    }
    return (fs_sexa (buf, value, w-f, s));
  } else {
    /* normal printf format */
    return (sprintf (buf, format, value));
  }
}

/*---------------------------------------------------------------------------*/

double UTtoJD(struct tm *utm)
{
  int year, month, day, hour, minute, second;
  int m, y, A, B, C, D;
  double d, jd;
  
  /* Note: The tm_year was modified by adding +1900 to it since tm_year refers
     to the number of years after 1900. 
     The month field was also modified by adding 1 to it
     since the tm_mon range is from 0 to 11 */

  year   = utm->tm_year + 1900;
  month  = utm->tm_mon +1;
  day    = utm->tm_mday;
  hour   = utm->tm_hour;
  minute = utm->tm_min;
  second = utm->tm_sec;
  
  
  if (month > 2) {
    m = month;
    y = year;
  } else {
    y = year - 1;
    m = month + 12;
  }
  
  /*  If the date is after 10/15/1582, then take Pope Gregory's modification
      to the Julian calendar into account */
  
  if (( year  >1582 ) ||
      ( year ==1582 && month  >9 ) ||
      ( year ==1582 && month ==9 && day >15 )) {
  
    A = (int) y/100;
    B = 2 - A + (int) A/4;
  } else {
    B = 0;
  }
  
  if (y < 0) {
    C = (int) ((365.25*y) - 0.75);
  } else {
    C = (int) (365.25*y);
  }
  
  D = (int) (30.6001*(m+1));
  
  d = (double) day + ( (double) hour + ( (double) minute + (double) second/60.0)/60.0)/24.0;
  jd = B + C + D + d + 1720994.5;
  
  return jd;
}

/*---------------------------------------------------------------------------*/
