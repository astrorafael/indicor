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
 * This file defines the generic INDI Item for all the class tree in
 * the Catalog/Device/PropertyVector/Property hierarchy
 * following the composite pattern (GoF, 1995)
 */

#ifndef INDI_ITEMBASE_H
#define INDI_ITEMBASE_H


#include <assert.h>
#include <stdio.h>
#include <string>		/* STL's string */
#include <map>			/* STL's map */

#include "log.h"
#include "item.h"


/* This class is not meant to have object instances, we use to derive
 * default implementations and common attributes
 */

class INDIItemBase : public INDIItem {

 public:


  /* constructors and destructor */
  INDIItemBase(const char* nam, INDIItem* paren, const char* logtag);

  virtual ~INDIItemBase();

  /****************************************/
  /* common methods for all the hierarchy */
  /****************************************/

  /* Item name */
  virtual const char* getName() const;

  /* set optional item label */
  virtual void setLabel(const char* lab);

  /* returns parent item (0 if root item) */
  virtual INDIItem* getParent() const;
  
  /* compares name with external string */
  virtual bool equals(const char* str) const;
  
  /****************************************/
  /* common methods for all hierarchy     */
  /* redefined as necessary at each level */
  /****************************************/

  /*
   * apply state change to a single or set of property vectors
   */

  virtual void idleStatus();
  virtual void okStatus();
  virtual void busyStatus();
  virtual void alertStatus();

  /* forces changed flag true for all properties below a given level */
  virtual void forceChange();

  /******************/
  /* Global Getters */
  /******************/

  /* gets the global resources factory */
  virtual ResourcesFactory* getSysFactory() const;

  /* gets the per-hub resources factory */
  virtual ResourcesFactory* getHubFactory() const;

  /*
   * child management operations
   */

  /* adds a new ITEM to the collection */
  /* collection takes ownership of allocated memory for item */
  virtual void add(INDIItem* prop);

  /* deletes an INDI item given by name from the collection */
  /* memory for item is freed */
  virtual void remove(INDIItem* prop);

  /* returns an INDI item given by name or null pointer if not found */
  virtual INDIItem* find(const char* nam) const;


  /*
   * XML generation methods
   */

  /* 
   * formats the message attribute prior to sending
   * <defXXX> , <setXXX> or <message> XML messages 
   */
  virtual void formatMsg(const char* fmt, ...);

  /* recursively prints XML <defXXXVector> */
  /* default implementation is for leaf nodes */
  virtual void indiDefProperty();
	
  /* recursively prints XML <setXXXVector> with all members */
  /* default implementation is for leaf nodes */
  virtual void indiSetProperty();

  /* prints XML <delProperty> message */
  /* default implementation for properies */
  virtual void indiDelProperty();

  /* prints <message> XML message */
  /* default implementetion for properties and property-vectors */
  virtual void indiMessage();

  /* recursively saves object hierarchy in a file */
  virtual void xmlSave();



  /* The observer part of the Observable/Observer pattern */
  /* Devices should override this method */
  virtual void update(PropertyVector* pv, ITopic topic);

 protected:

  Log* log;
  INDIItem* parent;		/* parent INDI Item */
  std::string name;		/* item name */
  std::string label;		/* optional GUI label */

  static FILE* fp;		/* common file pointer for XML output */

};

/*---------------------------------------------------------------------------*/


inline
INDIItemBase::~INDIItemBase()
{
  delete log;
}

/*---------------------------------------------------------------------------*/

inline const char* 
INDIItemBase::getName() const
{ 
  return name.c_str();
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::setLabel(const char* lab) 
{
  label = lab;
}

/*---------------------------------------------------------------------------*/

inline INDIItem* 
INDIItemBase:: getParent() const 
{
  return(parent);
}

/*---------------------------------------------------------------------------*/

inline bool
INDIItemBase::equals(const char* str) const 
{
  return (strcmp(getName(), str) == 0);
}

/*---------------------------------------------------------------------------*/

inline ResourcesFactory*
INDIItemBase::getHubFactory() const
{
  return(parent->getHubFactory());
}

/*---------------------------------------------------------------------------*/

inline ResourcesFactory*
INDIItemBase::getSysFactory() const
{
  return(parent->getSysFactory());
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::idleStatus()
{ 
  assert("idleStatus() operation not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase:: okStatus() 
{
  assert("okStatus() operation not supported by object " == NULL);
}	

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::busyStatus()
{
  assert("busyStatus() operation not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::alertStatus() 
{
  assert("alertStatus() operation not supported by object " == NULL);
}	

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::forceChange() 
{
  assert("forceChange() operation not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::add(INDIItem* prop) 
{
  assert("add() operation not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::remove(INDIItem* prop) 
{
  assert("remove() operation not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/

inline INDIItem*
INDIItemBase::find(const char* nam) const 
{
  assert("find() operation not supported by object " == NULL);
  return(0);
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::formatMsg(const char* fmt, ...) 
{
  assert("formatMsg() operat. not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::update(PropertyVector* pv, ITopic topic)
{
  log->warn(IFUN,"Event is not routed to a proper destination\n");
}


/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::indiDefProperty() 
{
  if(parent) parent->indiDefProperty();
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::indiSetProperty() 
{
  if(parent) parent->indiSetProperty();
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::indiDelProperty() 
{ 
  if(parent) parent->indiDelProperty();
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::indiMessage() 
{ 
  if(parent) parent->indiMessage();
}

/*---------------------------------------------------------------------------*/

inline void
INDIItemBase::xmlSave() 
{
  assert("xmlSave() operation not supported by object " == NULL);
}

/*---------------------------------------------------------------------------*/



#endif
