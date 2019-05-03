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
#ifndef INDI_COMPOITEM_H
#define INDI_COMPOITEM_H

#include "itembase.h"

/* 
 * The composite item in the Composite pattern holds its childrens
 * and implements its generic management methods
 *
 * Catalog, Device, PropertyVector are subclasses of INDICompositeItem
 */


class PropertyVector;

class INDICompositeItem : public INDIItemBase {

 public:

  /* constructors and destructor */
  INDICompositeItem(const char* nam, INDIItem* paren, const char* tag);

  ~INDICompositeItem();
  
  /* adds a new ITEM to the collection */
  /* collection takes ownership of allocated memory for item */
  virtual void add(INDIItem* prop);

  /* deletes an INDI item given by name from the collection */
  /* memory for item is freed */
  virtual void remove(INDIItem* prop);

  /* returns an INDI item given by name or null pointer if not found */
  virtual INDIItem* find(const char* nam) const;

  /* forces changed flag true for all properties below a given level */
  virtual void forceChange();


  /*
   * apply state change to a single or set of property vectors
   */

  virtual void idleStatus();
  virtual void okStatus();
  virtual void busyStatus();
  virtual void alertStatus();

  /*
   * XML generation methods
   */

  /* recursively prints XML <defXXXVector> */
  virtual void indiDefProperty();

  /* recursively prints XML <setXXXVector> with all members */
  virtual void indiSetProperty();

  /* recursively prints XML <delProperty> message */
  virtual void indiDelProperty();

  /* formats the message attribute to be printed in various XML mesages */
  virtual void formatMsg(const char* fmt, ...);

  /* recursively saves all XML children in a file */
  virtual void xmlSave();


  /*
   * iterators methods
   */

  typedef std::map<std::string,INDIItem*> ItemMap;

  ItemMap::const_iterator begin();
  ItemMap::const_iterator end();


 protected:

 /* the private associative array of individual items using STL */
  
  ItemMap itemMap; 

  static const int MSGSIZE = 256;
  char  msgbuf[MSGSIZE];	/* buffer to format sprintf mesages */
  char* message;      /* optional message pointer to msgbuf or NULL */
};

/*---------------------------------------------------------------------------*/

inline
INDICompositeItem::INDICompositeItem(const char* nam, INDIItem* paren,
				     const char* logtag) 
  : INDIItemBase(nam,paren,logtag), itemMap(), message(0) 
{
}

/*---------------------------------------------------------------------------*/


inline INDICompositeItem::ItemMap::const_iterator
INDICompositeItem::begin()
{ 
  return itemMap.begin();
}

/*---------------------------------------------------------------------------*/

inline INDICompositeItem::ItemMap::const_iterator
INDICompositeItem::end()
{
  return itemMap.end();
}

/*---------------------------------------------------------------------------*/

#endif

