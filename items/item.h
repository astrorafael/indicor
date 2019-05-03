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

#ifndef INDI_ITEM_H
#define INDI_ITEM_H



#include <stdio.h>

#include "utils.h"
#include "proputils.h"		/* basic typedefs */
#include "resfactory.h"		/* resources factory */

/***************************************************************/
/* specialize interfaces from the generic family of interfaces */
/***************************************************************/


/* Generic INDIItem interface
 * interface for hierarchy operations 
 */

class PropertyVector;		/* forward reference */

class INDIItem {

 public:

  static const INDIItem* NO_PARENT;

  virtual ~INDIItem() {}

  /****************************************/
  /* common methods for all the hierarchy */
  /****************************************/

  /* Item name */
  virtual const char* getName() const = 0;

  /* set optional item label */
  virtual void setLabel(const char* lab) = 0;

  /* returns parent item (0 if root item) */
  virtual INDIItem* getParent() const = 0;
  
  /* compares name with external string */
  virtual bool equals(const char* str) const = 0;
  
  /****************************************/
  /* common methods for all hierarchy     */
  /* redefined as necessary at each level */
  /****************************************/

  /*
   * apply state change to a single or set of property vectors
   */

  virtual void idleStatus()  = 0;
  virtual void okStatus()    = 0;
  virtual void busyStatus()  = 0;
  virtual void alertStatus() = 0;

  /* forces changed flag true for all properties below a given level */
  virtual void forceChange() = 0;

  /******************/
  /* Global Getters */
  /******************/

  /* gets the global resources factory */
  virtual ResourcesFactory* getSysFactory() const = 0;

  /* gets the per-hub resources factory */
  virtual ResourcesFactory* getHubFactory() const = 0;

  /*
   * child management operations
   */

  /* adds a new ITEM to the collection */
  /* collection takes ownership of allocated memory for item */
  virtual void add(INDIItem* prop) = 0;

  /* deletes an INDI item given by name from the collection */
  /* memory for item is freed */
  virtual void remove(INDIItem* prop) = 0;

  /* returns an INDI item given by name or null pointer if not found */
  virtual INDIItem* find(const char* name) const = 0;


  /*
   * XML generation methods
   */

  /* 
   * formats the message attribute prior to sending
   * <defXXX> , <setXXX> or <message> XML messages 
   */
  virtual void formatMsg(const char* fmt, ...) = 0;

  /* recursively prints XML <defXXXVector> */
  /* default implementation is for leaf nodes */
  virtual void indiDefProperty() = 0;
	
  /* recursively prints XML <setXXXVector> with all members */
  /* default implementation is for leaf nodes */
  virtual void indiSetProperty() = 0;

  /* prints XML <delProperty> message */
  /* default implementation for properies */
  virtual void indiDelProperty() = 0;

  /* prints <message> XML message */
  /* default implementetion for properties and property-vectors */
  virtual void indiMessage() = 0;

  /* recursively saves object hierarchy in a file */
  virtual void xmlSave() = 0;

  /* The observer part of the Observable/Observer pattern */
  /* Devices should override this method */
  virtual void update(PropertyVector* pv, ITopic topic) = 0;

};

#endif
