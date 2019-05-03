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

#ifndef INDI_LOG_H
#define INDI_LOG_H

#include <stdio.h>

#include "utils.h"

/* IFUN macro that gets the current function name */
/* copied and transformed from <assert.h> */

/* Version 2.4 and later of GCC define a magical variable `__PRETTY_FUNCTION__'
   which contains the name of the function currently being defined.
   This is broken in G++ before version 2.6.
   C9x has a similar variable called __func__, but prefer the GCC one since
   it demangles C++ function names.  */

#undef IFUN

# if defined __cplusplus ? __GNUC_PREREQ (2, 6) : __GNUC_PREREQ (2, 4)
#   define IFUN	__PRETTY_FUNCTION__
# else
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#   define IFUN	__func__
#  else
#   define IFUN	"unknown"
#  endif
# endif

class Log
{
 public:

  /* log levels as strings */

  static const char* NONE;
  static const char* ERROR;
  static const char* WARN;
  static const char* INFO;
  static const char* DEBUG;
  static const char* VERBO;

  virtual ~Log() {};

  /* sets the common output stream */
  virtual void setFile(FILE* fp) = 0;	
  virtual FILE* getFile() const  = 0;

    /* outputs at a given level */
  virtual void error(const char* ctxt, const char* fmt, ...)   = 0;
  virtual void warn (const char* ctxt, const char* fmt, ...)   = 0;
  virtual void info (const char* ctxt, const char* fmt, ...)   = 0;
  virtual void debug(const char* ctxt, const char* fmt, ...)   = 0;
  virtual void verbose(const char* ctxt, const char* fmt, ...) = 0;
  
  /* output a timestamp at INFO level */
  virtual void timestamp() = 0;

  /* flushes the output stream */
  virtual void flush() = 0;
};


#endif
