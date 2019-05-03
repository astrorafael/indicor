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

#ifndef INDI_XML_LOG_FACTORY_H
#define INDI_XML_LOG_FACTORY_H

#include <string>
#include <map>

#include "logfact.h"
#include "lilxml.h"


class XLogFactory : public LogFactory {

 public:

  /* XML elements and attributes */

  static const char* LOG;
  static const char* PATH;
  static const char* CLASS;
  static const char* NAME;
  static const char* VALUE;


  XLogFactory();
  virtual ~XLogFactory();

  const char* getLogFile() const;
  Log* forClass(const char* name);

 private:

  typedef std::map<std::string,std:: string> LogLevelMap;


  LilXML* lp; 			/* XML parser */
  LogLevelMap logLevelMap;
  std::string path;		/* full log file pathname */
  std::string xmlFile;		/* full xml config file */
  
  /******************/
  /* Helper methods */
  /******************/

  bool read(const char* basedir, const char* xmlFile);
  bool readLog(XMLEle* elem);	/* reads the root element 'log' */
  bool readClass(XMLEle* elem);	/* reads the 'class' element */

};

/*---------------------------------------------------------------------------*/

inline const char*
XLogFactory::getLogFile() const
{
  return(path.c_str());
}

#endif
