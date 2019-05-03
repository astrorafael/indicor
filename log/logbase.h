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

#ifndef INDI_LOGBASE_H
#define INDI_LOGBASE_H

#include "log.h"

class LogBase : public Log
{
 public:

  LogBase() {}
  virtual ~LogBase() {}

  /* sets the common output stream */
  virtual void setFile(FILE* fp);	
  virtual FILE* getFile() const;

  /* outputs at a given level */
  virtual void error(const char* ctxt, const char* fmt, ...);
  virtual void warn (const char* ctxt, const char* fmt, ...);
  virtual void info (const char* ctxt, const char* fmt, ...);
  virtual void debug(const char* ctxt, const char* fmt, ...); 
  virtual void verbose(const char* ctxt, const char* fmt, ...);
  
  /* output a timestamp at INFO level */
  virtual void timestamp();

  /* flushes the output stream */
  virtual void flush();

 private:

  static FILE* fp;		/* shared output stream */

};

typedef LogBase Log5;		/* an alias for LogBase */

/*---------------------------------------------------------------------------*/

inline FILE*
LogBase::getFile() const
{
  return(fp);
}

/*---------------------------------------------------------------------------*/

inline void
LogBase::setFile(FILE* f)
{
  if(fp) fflush(fp);
  fp = f;
}

/*---------------------------------------------------------------------------*/

inline void
LogBase::flush()
{
  if(fp) fflush(fp);
}

/*---------------------------------------------------------------------------*/

#endif
