/* $Id: $ */
/*

  Copyright (C) 2005 Rafael Gonzalez (astrorafael@yahoo.es)

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
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "timestamp.h"
#include "logbase.h"

/* output stream shared by all members */
FILE* LogBase::fp = 0;


/*---------------------------------------------------------------------------*/

void
LogBase::error(const char* ctxt, const char* fmt, ...)
{
  assert(fp != 0);

  va_list ap;
  va_start (ap, fmt);

  fprintf(fp, "[ERROR] %s ", ctxt);

  vfprintf (fp, fmt, ap);
  va_end (ap);
}

/*---------------------------------------------------------------------------*/

void
LogBase::warn(const char* ctxt, const char* fmt, ...)
{
  assert(fp != 0);

  va_list ap;
  va_start (ap, fmt);

  fprintf(fp,"[ WARN] %s: ",ctxt);

  vfprintf (fp, fmt, ap);
  va_end (ap);
}

/*---------------------------------------------------------------------------*/

void
LogBase::info(const char* ctxt, const char* fmt, ...)
{
  assert(fp != 0);

  va_list ap;
  va_start (ap, fmt);

  //fprintf(fp, "[ INFO] %s: ",ctxt);
  fputs("[ INFO] ",fp); 
  vfprintf (fp, fmt, ap);
  va_end (ap);
}

/*---------------------------------------------------------------------------*/

void
LogBase::timestamp()
{
  assert(fp != 0);
  fprintf (fp, "[ INFO] ### %s ###\n", ::timestamp());
}


/*---------------------------------------------------------------------------*/

void
LogBase::debug(const char* ctxt, const char* fmt, ...)
{
  assert(fp != 0);

  va_list ap;
  va_start (ap, fmt);

  fprintf(fp, "[DEBUG] %s: ",ctxt);

  vfprintf (fp, fmt, ap);
  va_end (ap);
  fflush(fp);			// useful to trap server crashes
}

/*---------------------------------------------------------------------------*/

void
LogBase::verbose(const char* ctxt, const char* fmt, ...)
{
  assert(fp != 0);

  va_list ap;
  va_start (ap, fmt);

  fprintf(fp,"[VERBO] %s: ",ctxt);

  vfprintf (fp, fmt, ap);
  va_end (ap);
  fflush(fp);			// useful to trap server crashes
}

/*---------------------------------------------------------------------------*/
