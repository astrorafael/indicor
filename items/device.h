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

#ifndef INDIDEVICE_H
#define INDIDEVICE_H


#ifndef INDIITEM_H
#include "item.h"
#endif 

#ifndef INDIPROPVECTOR_H
#include "propvector.h"
#endif


class Plugin;		/* forward reference */

/* INDI DEVICE DEFINITIONS */

class Device : public INDICompositeItem {
 
 public:

  /* constructors and destructor */

  Device(const char* nam, const char* path, INDIItem* paren);
  ~Device();

  
  /* sets the implementation corresponding to this device */
  void setImplementation(Plugin* impl);

  Plugin* getImplementation() const;

  void setLogFile(FILE* fp);
  
  /*
   * redefinition of generic methods
   */

  /* outputs <delProperty> XML message */
  /* signals all properties for deletion */
  virtual void indiDelProperty();

  /* outputs <message device='xxx'> XML message */
  virtual void indiMessage();
 
  /* recursively saves object hierarchy in a file */
  virtual void xmlSave();


  /* The Observable/Observer pattern */
  /* Implements the observer "update" method */
  /* observers' reaction upon observable change. Forwards to plug'in */
  virtual void update(PropertyVector* pv, ITopic topic);

  /*
   * called by the XML event handler
   * forward these requests to the actual implementation
   */

  void update(BLOBPropertyVector* pv, char* name[], char* blob[], int n);
  void update(TextPropertyVector* pv, char* name[], char* text[], int n);
  void update(NumberPropertyVector* pv, char* name[], double number[], int n);
  void update(SwitchPropertyVector* pv, char* name, ISState swit);

  /*
   * Specific Device methods
   */



  const char* getFileName();

 protected:

  Plugin* imp;		/* pointer to the actual implementation */
  std::string fileName;		/* where to read/write configuration */

};

/*---------------------------------------------------------------------------*/

inline
Device::~Device() 
{
}

/*---------------------------------------------------------------------------*/

inline void
Device::setLogFile(FILE* fp) 
{
  log->setFile(fp);			/* per plug-in static variable */
}

/*---------------------------------------------------------------------------*/

inline void
Device::setImplementation(Plugin* impl) 
{
  imp = impl;
}

/*---------------------------------------------------------------------------*/

inline Plugin*
Device::getImplementation() const 
{
  return(imp);
}

/*---------------------------------------------------------------------------*/

inline const char*
Device::getFileName() 
{ 
  return (fileName.c_str()); 
}

/*---------------------------------------------------------------------------*/

#endif
