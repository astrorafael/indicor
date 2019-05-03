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

#ifndef INDI_LOG2_H
#define INDI_LOG2_H

#include "logbase.h"

class Log2 : public LogBase
{
 public:

  /********************/
  /* Object lifecycle */
  /********************/

  Log2() {}
  virtual ~Log2() {}

  /* redefines outputs at a given level */

  virtual void info (const char* ctxt, const char* fmt, ...)   {};
  virtual void debug(const char* ctxt, const char* fmt, ...)   {}; 
  virtual void verbose(const char* ctxt, const char* fmt, ...) {};
  
  /* redefines timestamp at INFO level */
  virtual void timestamp() {};

};

#endif
