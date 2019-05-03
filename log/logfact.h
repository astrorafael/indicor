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

#ifndef INDI_LOG_FACTORY_H
#define INDI_LOG_FACTORY_H

#include "log.h"

class LogFactory {

 public:

  static LogFactory* instance();
  virtual ~LogFactory();
  
  /* gets the log file name path where it should be created */
  virtual const char* getLogFile() const = 0;

  /* reads the log configuration file */
  virtual bool read(const char* basedir, const char* configFile) = 0;

  /* gets the current logging object for 'name' */
  virtual Log* forClass(const char* name) = 0;

 protected:
  
  LogFactory() {};
  static LogFactory* _instance;
};

#endif
