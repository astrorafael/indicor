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

#include <stdarg.h>
#include "compoitem.h"

/*---------------------------------------------------------------------------*/

INDICompositeItem::~INDICompositeItem() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin(); i !=  itemMap.end(); i++) {
    delete (*i).second;
  }
  itemMap.clear();
}

/*---------------------------------------------------------------------------*/

void INDICompositeItem::add(INDIItem* prop) 
{
  const std::string& name = prop->getName();
  itemMap.insert(ItemMap::value_type(name,prop));
}

/*---------------------------------------------------------------------------*/

 void INDICompositeItem::remove(INDIItem* prop) 
{
  const std::string key(prop->getName());
  ItemMap::iterator i;
  
  i = itemMap.find(key);
  if(i != itemMap.end()) {
    prop = (*i).second;
    itemMap.erase(i);
    delete prop;
  }
}

/*---------------------------------------------------------------------------*/

INDIItem* INDICompositeItem::find(const char* name) const
{
  std::string key(name);
  ItemMap::const_iterator i;
  INDIItem* prop = 0;
  
  i = itemMap.find(key);
  if(i != itemMap.end()) {
    prop = (*i).second;
  }
  return (prop);
}

/*---------------------------------------------------------------------------*/

void INDICompositeItem::formatMsg(const char* fmt, ... ) 
{
  if (fmt) {
    va_list ap;
    va_start (ap, fmt);
    vsprintf (msgbuf, fmt, ap);
    va_end (ap);
    message = msgbuf;
  }  
}


/*---------------------------------------------------------------------------*/

/* recursively prints XML <defXXXVector> */
void  INDICompositeItem::indiDefProperty()
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++)
    (*i).second->indiDefProperty();
}

/*---------------------------------------------------------------------------*/

/* recursively prints XML <setXXXVector> with all members */
void INDICompositeItem:: indiSetProperty() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++)
    (*i).second->indiSetProperty();
}


/*---------------------------------------------------------------------------*/

/* recursively prints XML <delProperty> message */
void  INDICompositeItem::indiDelProperty() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++)
    (*i).second->indiSetProperty();
}

/*---------------------------------------------------------------------------*/

/* recursively save properties in a File */
void  INDICompositeItem::xmlSave() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    (*i).second->xmlSave();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply idleStatus() to all children */
void  INDICompositeItem::idleStatus() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    (*i).second->idleStatus();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply okStatus() to all children */
void  INDICompositeItem::okStatus() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    (*i).second->okStatus();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply busy() to all children */
void  INDICompositeItem::busyStatus() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    (*i).second->busyStatus();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply alertStatus() to all children */
void  INDICompositeItem::alertStatus() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    (*i).second->alertStatus();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively forces changed flag to true */
void INDICompositeItem::forceChange() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++)
    (*i).second->forceChange();
}

/*---------------------------------------------------------------------------*/
