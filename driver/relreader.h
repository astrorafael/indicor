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

/*
 * Relationships file XML reader
 */

#ifndef INDI_REL_READER_H
#define INDI_REL_READER_H


#include "item.h"
#include "property.h"
#include "propvector.h"
#include "device.h"
#include "driver.h"
#include "lilxml.h"

class RelationshipsReader  {

 public:

  /***********************************/
  /* Device XML file definitons tags */
  /***********************************/

  static const char* ROOT;
  static const char* OBSERVER;
  static const char* OBSERVABLE;
  static const char* PROPERTY;

  RelationshipsReader(INDIItem* root);
 ~RelationshipsReader();
  
  /* reads relationships XML configuration file. ok=true ,fail=false */

  bool read(const char* basedir, const char* path);

 private:

  Log* log;
  LilXML* lp; 			/* XML parser */
  INDIItem* driver;		/* The global driver to link devices to */
  std::string xmlFile;		/* XML configuration file name */

  /******************/
  /* Helper methods */
  /******************/

  bool readRelationships(XMLEle* elem);
  bool readObserver(XMLEle* elem);
  bool readObservable(XMLEle* elem, Device* observer);
  bool readProperty(XMLEle* elem, Device* observer, Device* observable);

};


#endif
