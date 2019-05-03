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

#ifndef INDIPROPVECTOR_H
#define INDIPROPVECTOR_H

#include <list>

#include "compoitem.h"
#include "property.h"


#define NO_MESSAGE ((const char* )(0)) /* no message in INDI protocol */

/* PROPERTY VECTORS DEFINITIONS */


/* The Base class of all property vectors
 * Cannot create instances
 */

class Device;		/* forward reference */

class PropertyVector : public INDICompositeItem
{
 public:

  /*
   * constructors and destructor 
   */

  PropertyVector(const char* nam, INDIItem* paren, const char* tag);


   /* 
   * PropertyVector management
   */
  
  void setState(IPState newState);
  IPState getState() const;

  void setGroup(const char* grp);
  
  /*
   * redefinition of generic methods
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

  /* prints XML <delProperty> message */
  /* non-recursive  */
  /* Levels: Driver, Device, PropertyVector */
  virtual void indiDelProperty();

  /* recursively saves all its children */
  virtual void xmlSave();

  /*
   * These methods are the Observable/Oberver pattern
   * Observable Role => the PropertyVectors
   * Observer Role   => Other Devices
   */

  /* adds an observer to track changes in observable's property vectors */
  void addObserver(Device* dev, ITopic topic);

  /* removes an observer from this observable*/
  void removeObserver(Device* dev, ITopic topic);
 

 protected:


  /* property vector common attributes */
  
  std::string group; /* Property group membership. blank by default */
  IPState state;		/* current state of property */

  /* anybody can be an observer */
  typedef std::list<INDIItem*> ObserversList;
  ObserversList observersValueList; /* listeners for changes in values */
  ObserversList observersStateList; /* listeners for changes in state */


  /******************/
  /* helper methods */
  /******************/

  void notifyValueChange();	/* invoke value observers */
  void notifyStateChange();	/* invoke state observers */

  void generateEvent(ITopic topic); /* generate I/O event */

  /* common attributes in defXXXVector */
  virtual void printDefAttrs(bool timeFlag) const; 
  
  /* common attributes in setXXXVector */
  virtual void printSetAttrs() const; 

  virtual const char* defTag() const = 0;
  virtual const char* setTag() const = 0;


};

/*---------------------------------------------------------------------------*/

inline
PropertyVector::PropertyVector(const char* nam, INDIItem* paren,
			       const char* tag) 
  : INDICompositeItem(nam, paren,tag), group(""), state(IPS_IDLE) 
{
}
/*---------------------------------------------------------------------------*/

inline void
PropertyVector::setState(IPState newState) 
{
  state = newState;
  generateEvent(IT_STATE);
}

/*---------------------------------------------------------------------------*/

inline IPState
PropertyVector::getState() const 
{
  return(state);
}

/*---------------------------------------------------------------------------*/

inline void
PropertyVector::setGroup(const char* grp) 
{
  group = grp;
}

/*---------------------------------------------------------------------------*/

inline void
PropertyVector::idleStatus()  
{ 
  setState(IPS_IDLE); 
}

/*---------------------------------------------------------------------------*/

inline void
PropertyVector::okStatus()    
{ 
  setState(IPS_OK);   
}

/*---------------------------------------------------------------------------*/

inline void
PropertyVector::busyStatus()  
{ 
  setState(IPS_BUSY); 
}

/*---------------------------------------------------------------------------*/

inline void
PropertyVector::alertStatus() 
{ 
  setState(IPS_ALERT);
}

/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* redefines behaviour specific for this class */
class LightPropertyVector : public PropertyVector 
{
 public:

  /*
   * constructors and destructor 
   */

  LightPropertyVector(const char* nam, INDIItem* paren);

   
   /* change a light property */
  void setValue(const char* name, IPState newVal);
  
  /* gets the value given by the property name */
  IPState getValue(const char* name) const;

   /* sets idle() for a property given by name */
  void  idle(const char* name);
  
  /* sets ok() for a property given by name */
  void ok(const char* name);
  
   /* sets busy() for a property given by name */
  void  busy(const char* name);
  
  /* sets alert() for a property given by name */
  void alert(const char* name);
  

  /*
   * apply a state change to all lights
   */

  void idle();
  void ok();
  void busy();
  void alert();
  

 protected:

  const char* defTag() const;
  const char* setTag() const;

};

/*---------------------------------------------------------------------------*/

inline
LightPropertyVector::LightPropertyVector(const char* nam, INDIItem* paren) 
  : PropertyVector(nam, paren,"LightPropertyVector") 
{ 
}

/*---------------------------------------------------------------------------*/

inline void  
LightPropertyVector::idle(const char* name) 
{ 
  setValue(name, IPS_IDLE); 
}

/*---------------------------------------------------------------------------*/

inline void  
LightPropertyVector::ok(const char* name) 
{ 
  setValue(name, IPS_OK); 
}

/*---------------------------------------------------------------------------*/

inline void  
LightPropertyVector::busy(const char* name) 
{ 
  setValue(name, IPS_BUSY); 
}

/*---------------------------------------------------------------------------*/

inline void  
LightPropertyVector::alert(const char* name) 
{ 
  setValue(name, IPS_ALERT); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
LightPropertyVector::defTag() const 
{ 
  return("defLightVector"); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
LightPropertyVector::setTag() const 
{ 
  return("setLightVector"); 
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* abstract class that captures more common attributes
 * and common routines to non-light properties.
 * Cannot create instances
 */

class NoLightPropertyVector : public PropertyVector
{
 public:

  /*
   * constructors and destructor 
   */

  NoLightPropertyVector(const char* nam, INDIItem* paren, const char* tag);

  /* 
   * PropertyVector management
   */

  void setTimeout(double tmout);

  void setPerm(IPerm prm);

  /* check write permissions */
  bool checkWrite() const;

  /* check read permissions */
  bool checkRead() const;


 protected:

  /* more property vector common attributes */

  IPerm  perm;			/* otensible client Controlability */
  double timeout;		/* worse-case time to affect
				 * 0 = applies default client tiemout
				 * N/A for read-only properties */

  /* helper functions */

  /* outputs common attributes in defXXXVector */
  void printDefAttrs(bool timeFlag) const;
  
  /* outputs common attributes in setXXXVector */
  void printSetAttrs() const;

  const char* defTag() const = 0;
  const char* setTag() const = 0;

};

/*---------------------------------------------------------------------------*/

inline
NoLightPropertyVector::NoLightPropertyVector(const char* nam, INDIItem* paren,
					     const char* tag)
  : PropertyVector(nam,paren,tag), perm(IP_RO), timeout(0) 
{
}

/*---------------------------------------------------------------------------*/

inline void
NoLightPropertyVector::setTimeout(double tmout) 
{
  timeout = tmout;
}

/*---------------------------------------------------------------------------*/

inline void
NoLightPropertyVector::setPerm(IPerm prm) 
{
  perm = prm;
}

/*---------------------------------------------------------------------------*/

inline bool
NoLightPropertyVector::checkWrite() const 
{
  return((perm == IP_WO) || (perm == IP_RW));
}

/*---------------------------------------------------------------------------*/

inline bool
NoLightPropertyVector::checkRead() const
{
  return((perm == IP_RO) || (perm == IP_RW));
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* redefines behaviour for this class */

class TextPropertyVector : public NoLightPropertyVector
{
 public:

  /*
   * constructors and destructor
   */

  TextPropertyVector(const char* nam, INDIItem* paren);

  /* 
   * property management
   */


  /* change a number property */
  void setValue(const char* name, const char* newVal);
  
  /* gets the value given by the property name */
  const char* getValue(const char* name) const;
  

 protected:

  const char* defTag() const;
  const char* setTag() const;

};

/*---------------------------------------------------------------------------*/

inline
TextPropertyVector::TextPropertyVector(const char* nam, INDIItem* paren) 
  : NoLightPropertyVector(nam,paren,"TextPropertyVector") 
{
}


/*---------------------------------------------------------------------------*/

inline const char*
TextPropertyVector::defTag() const 
{ 
  return("defTextVector"); 
}

/*---------------------------------------------------------------------------*/

inline const char*
TextPropertyVector::setTag() const 
{ 
  return("setTextVector"); 
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


/* redefines behaviour for this class */
class NumberPropertyVector :  public NoLightPropertyVector
{
 public:

  /*
   * constructors and destructor 
   */

  NumberPropertyVector(const char* nam, INDIItem* paren);

  /* 
   * property management
   */

  /* change a number property */
  void setValue(const char* name, double newVal);
  
  /* gets the value given by the property name */
  double getValue(const char* name) const;
  
 protected:

  const char* defTag() const;
  const char* setTag() const;
};

/*---------------------------------------------------------------------------*/

inline 
NumberPropertyVector::NumberPropertyVector(const char* nam, INDIItem* paren) 
  : NoLightPropertyVector(nam,paren,"NumberPropertyVector") 
{
}


/*---------------------------------------------------------------------------*/

inline const char* 
NumberPropertyVector::defTag() const 
{ 
  return("defNumberVector"); 
}


/*---------------------------------------------------------------------------*/

inline const char* 
NumberPropertyVector::setTag() const 
{ 
  return("setNumberVector"); 
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* redefines behaviour for this class */
class SwitchPropertyVector :  public NoLightPropertyVector
{
 public:

  /*
   * constructors and destructor 
   */

  SwitchPropertyVector(const char* nam, INDIItem* paren);

  /* 
   * property management
   */

  /* set by XML reader */
  void setRule(ISRule r);

  /* read by the XML handler */
  ISRule getRule() const;

  /* change a switch value */
  void setValue(const char* name, ISState newVal);
   
  /* gets the value given by the property name */
  ISState getValue(const char* name) const;
  
  /* sets on() for a property given by name */
  void  on(const char* name);
  
  /* sets off() for a property given by name */
  void off(const char* name);


  /*
   * apply a value change to a set of switch properties under a given level
   * ignoring all other property types
   */

  void on();
  void off();

  /* checks for rule before updating.
     Returns true if ok or false if breaks the rule
  */

  bool checkRule(SwitchProperty* sw, ISState value);


  /* finds last 'On' switch, updating cache */
  /* used by XML reader */
  void updateLastOn();

  /* get last switch On. it may return NULL for AtMostOne or AnyOfMany */
  SwitchProperty* getLastOn();

 protected:

  ISRule rule;			/* governing rule */
  SwitchProperty* lastOn;	/* last On accessed property */

  void printDefAttrs(bool timeFlag) const; 

  /* writes newVal to switch, appliing the defined rule */

  void applyRule(SwitchProperty* p, ISState newVal);

  /* called by applyRule */
  void applyOneOfMany(SwitchProperty* p, ISState newVal);
  void applyAnyOfMany(SwitchProperty* p, ISState newVal);
  void applyAtMostOne(SwitchProperty* p, ISState newVal);
  
  const char* defTag() const;
  const char* setTag() const;

};

/*---------------------------------------------------------------------------*/

inline  
SwitchPropertyVector::SwitchPropertyVector(const char* nam, INDIItem* paren) 
  : NoLightPropertyVector(nam,paren,"SwitchPropertyVector"),
    rule(ISR_NOFMANY), lastOn(0)
{
}


/*---------------------------------------------------------------------------*/

inline  void
SwitchPropertyVector::on(const char* name) 
{ 
  setValue(name, ISS_ON); 
}

/*---------------------------------------------------------------------------*/

inline  void
SwitchPropertyVector::off(const char* name) 
{ 
  setValue(name, ISS_OFF); 
}

/*---------------------------------------------------------------------------*/

inline  void 
SwitchPropertyVector::setRule(ISRule r) 
{ 
  rule = r; 
}

/*---------------------------------------------------------------------------*/

inline ISRule
SwitchPropertyVector::getRule() const 
{ 
  return(rule); 
}

/*---------------------------------------------------------------------------*/

inline  SwitchProperty* 
SwitchPropertyVector:: getLastOn() 
{ 
  return (lastOn); 
}

/*---------------------------------------------------------------------------*/

inline  const char*
SwitchPropertyVector::setTag() const 
{ 
  return("setSwitchVector"); 
}

/*---------------------------------------------------------------------------*/
inline  const char*
SwitchPropertyVector::defTag() const 
{ 
  return("defSwitchVector"); 
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* redefines behaviour for this class */
class BLOBPropertyVector :  public NoLightPropertyVector
{
 public:

  /*
   * constructors and destructor 
   */

  BLOBPropertyVector(const char* nam, INDIItem* paren);


 /* change a switch value */
  void setValue(const char* name, const BLOB& bl);
   
  /* gets the value given by the property name */
  const BLOB& getValue(const char* name) const;
  
 protected:

  const char* defTag() const;
  const char* setTag() const;
};

/*---------------------------------------------------------------------------*/

inline
BLOBPropertyVector::BLOBPropertyVector(const char* nam, INDIItem* paren) 
  : NoLightPropertyVector(nam,paren,"BLOBPropertyVector") 
{
}

/*---------------------------------------------------------------------------*/

inline  void 
BLOBPropertyVector::setValue(const char* name, const BLOB& newVal) 
{
  BLOBProperty* bp = DYNAMIC_CAST(BLOBProperty*, find(name));
  assert(bp != 0); 
  bp->setValue(newVal);
} 

/*---------------------------------------------------------------------------*/

inline  const BLOB&
BLOBPropertyVector::getValue(const char* name) const 
{
  BLOBProperty* bp = DYNAMIC_CAST(BLOBProperty*, find(name));
  assert(bp != 0);
  return(bp->getValue());
}

/*---------------------------------------------------------------------------*/

inline const char*
BLOBPropertyVector::defTag() const 
{ 
  return("defBLOBVector"); 
}

/*---------------------------------------------------------------------------*/

inline const char*
BLOBPropertyVector::setTag() const 
{ 
  return("setBLOBVector"); 
}


/*****************************************************************************/


#endif
