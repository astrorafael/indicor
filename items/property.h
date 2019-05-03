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

#ifndef INDI_PROPERTY_H
#define INDI_PROPERTY_H

#include <stdio.h>
#include <string.h>		/* memory copy functions */
#include <stdlib.h>		/* realloc() */
#include <string>		/* STL string class */
#include <map>			/* STL map (asociative array) class */


#include "proputils.h"
#include "itembase.h"



/*****************************************************************************/

/*
 * The base class for all  property items
 */

class Property : public INDIItemBase {

 public:

  /* Object's lifecycle */
  Property(const char* nam, INDIItem* paren, const char* tag);

  /*  parent property vector use this to test the changed flag */
  bool hasChanged();
  
  /* 
   * XML generation methods
   */

  /* prints XML for <defXXX> */
  virtual void indiDefProperty(); /* redefined from INDIItem */

  /* save property item to an XML file */
  virtual void xmlSave();	/* redefined from INDIItem */

  /* prints XML for <oneXXX> */
  virtual void indiOneProperty(); /* new method especific to Properties */

  virtual void forceChange();

 protected:

  bool        changed;		/* flag for value changed */

  /* helper operations used in XML generation */

  virtual const char* defTag() const = 0;
  virtual const char* oneTag() const = 0;
  virtual const char* pcData() const = 0;
};


/*---------------------------------------------------------------------------*/

inline
Property::Property(const char* nam, INDIItem* paren, const char* tag) 
  : INDIItemBase(nam,paren,tag), changed(false) 
{
}

/*---------------------------------------------------------------------------*/

inline bool
Property::hasChanged() 
{ 
  return(changed); 
}
  
/*---------------------------------------------------------------------------*/

inline void
Property::forceChange() 
{ 
  changed = true; 
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

class TextProperty : public Property {
 
 public:

  TextProperty(const char* nam, const char* val, INDIItem* paren);


  void setValue(const char* val);
  const char* getValue() const;
  
 protected:

  std::string value;		/* string property value */

  /* helper operations */

  const char* defTag() const;
  const char* oneTag() const;
  const char* pcData() const;
  
};


/*---------------------------------------------------------------------------*/

inline
TextProperty::TextProperty(const char* nam, const char* val, INDIItem* paren) 
  : Property(nam,paren,"TextProperty"), value(val) 
{
}

/*---------------------------------------------------------------------------*/

inline void
TextProperty::setValue(const char* val) 
{
  value = val;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline const char* 
TextProperty::getValue() const 
{
  return value.c_str();
}

/*---------------------------------------------------------------------------*/

inline const char* 
TextProperty::defTag() const 
{ 
  return ("defText");
} 

/*---------------------------------------------------------------------------*/

inline const char* 
TextProperty::oneTag() const 
{ 
  return ("oneText"); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
TextProperty::pcData() const 
{ 
  return (value.c_str()); 
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


class SwitchProperty : public Property {

 public:


  SwitchProperty(const char* nam, ISState val, INDIItem* paren);

  /* getter/setter methods */ 
  void setValue(ISState val);
  ISState getValue() const;

  /* another pair cof convenient setters */
  void on();
  void off();
 

 protected:

  ISState value;		/* switch value: true = On, false = Off */

  /* helper operations */

  const char* defTag() const;
  const char* oneTag() const;
  const char* pcData() const;

};

/*---------------------------------------------------------------------------*/

inline
SwitchProperty::SwitchProperty(const char* nam, ISState val, INDIItem* paren) 
  : Property(nam,paren,"SwitchProperty"), value(val) 
{
}

/*---------------------------------------------------------------------------*/

inline void
SwitchProperty::setValue(ISState val) 
{
  value = val;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline ISState
SwitchProperty::getValue() const 
{
  return value;
}

/*---------------------------------------------------------------------------*/

inline void
SwitchProperty::on()  
{ 
  value = ISS_ON;  
  changed=true; 
}

/*---------------------------------------------------------------------------*/

inline void
SwitchProperty::off() 
{ 
  value = ISS_OFF;
  changed=true; 
}

/*---------------------------------------------------------------------------*/

inline const char* 
SwitchProperty::defTag() const 
{ 
  return ("defSwitch");
} 

/*---------------------------------------------------------------------------*/

inline const char* 
SwitchProperty::oneTag() const 
{ 
  return ("oneSwitch"); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
SwitchProperty::pcData() const 
{ 
  return (sState2Str(value)); 
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

class LightProperty : public Property {

 public:

  /* object's lifecycle */
  LightProperty(const char* nam, IPState val, INDIItem* paren);

  /* getter/setter methods */
  void setValue(IPState val);
  IPState getValue() const;

  /* convenient setter methods */
  void idle();
  void ok();
  void busy();
  void alert();
 
  
 protected:

  IPState value;		     /* light Value */
  
  const char* defTag() const;
  const char* oneTag() const;
  const char* pcData() const;
};

/*---------------------------------------------------------------------------*/

inline
LightProperty::LightProperty(const char* nam, IPState val, INDIItem* paren) 
  : Property(nam,paren,"LightProperty"), value(val) 
{
}

/*---------------------------------------------------------------------------*/

inline void
LightProperty::setValue(IPState val) 
{
  value = val;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline IPState
LightProperty::getValue() const 
{
  return(value);
}

/*---------------------------------------------------------------------------*/

inline void
LightProperty::idle()  
{ 
  value = IPS_IDLE;  
  changed=true; 
}

/*---------------------------------------------------------------------------*/

inline void
LightProperty::ok()    
{ 
  value = IPS_OK;    
  changed=true; 
}

/*---------------------------------------------------------------------------*/

inline void
LightProperty::busy()  
{ 
  value = IPS_BUSY;  
  changed=true; 
}

/*---------------------------------------------------------------------------*/

inline void
LightProperty::alert() 
{ 
  value = IPS_ALERT; 
  changed=true; 
}

/*---------------------------------------------------------------------------*/

inline const char* 
LightProperty::defTag() const 
{ 
  return ("defLight"); 
}


/*---------------------------------------------------------------------------*/

inline const char* 
LightProperty::oneTag() const 
{ 
  return ("oneLight"); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
LightProperty::pcData() const 
{ 
  return (pState2Str(value)); 
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

class NumberProperty : public Property {
 public:


  NumberProperty(const char* nam, double val, const char* fmt, INDIItem* par);

  void setMin(double mi);
  double getMin() const;
  

  void setMax(double ma);
  double getMax() const;

  void setStep(double stp);

  void setValue(double val);
  double getValue() const;

  /*
   * XML genertaion methods. Redefined.
   */

  virtual void indiDefProperty(); /* prints XML for <defNumber> */
  virtual void indiOneProperty(); /* prints XML for <oneNumber> */
  virtual void xmlSave();      /* save property item to an XML file */

 private:

  std::string format;		/* format string to be used in the GUI */
  double value;			/* property value */
  double min;			/* minimal allowed value */
  double max;	      /* maximun allowed value, ignore if max = min */
  double step;			/* allowed increments, ignore if 0 */

  static double lmin;	 /* emulates the official code in setNumber */
  static double lmax;	 /* emulates the official code in setNumber */

  const char* defTag() const;
  const char* oneTag() const;
  const char* pcData() const;
};

/*---------------------------------------------------------------------------*/

inline
NumberProperty::NumberProperty(const char* nam, double val, const char* fmt, 
			       INDIItem* paren) 
  : Property(nam,paren,"NumberProperty"), format(fmt), value(val), min(0), max(0), step(0) 
{ 
}

/*---------------------------------------------------------------------------*/

inline void
NumberProperty::setMin(double mi) 
{
  min = mi;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline double
NumberProperty::getMin() const 
{
  return(min);
}


/*---------------------------------------------------------------------------*/

inline void
NumberProperty::setMax(double ma) 
{
  max = ma;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline double
NumberProperty::getMax() const 
{
  return(max);
}


/*---------------------------------------------------------------------------*/

inline void
NumberProperty::setStep(double stp) 
{
  step = stp;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline void
NumberProperty::setValue(double val) 
{
  value = val;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline double
NumberProperty::getValue() const 
{
  return value;
}

/*---------------------------------------------------------------------------*/

inline const char* 
NumberProperty::defTag() const 
{ 
  return ("defNumber"); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
NumberProperty::oneTag() const 
{ 
  return ("oneNumber"); 
}

/*---------------------------------------------------------------------------*/

/* not used */
inline const char* 
NumberProperty::pcData() const 
{ 
  return (""); 
} 


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


struct BLOB {
  void*  data;			/* pointer to the real data */
  size_t size;			/* BLOB size undecoded and uncompresed */
  BLOB(void* p, size_t sz) : data(p), size(sz) {}
};


class BLOBProperty : public Property {
 public:


  BLOBProperty(const char* nam, INDIItem* paren);
  ~BLOBProperty();


  // share pointers to blobs
  void setValue(const BLOB& bl);

  /* sets the file extension => file format */
  void setExtension(const char* ext);


  const BLOB& getValue() const;  

  /*
   * XML genertaion methods. Redefined.
   */

  virtual void indiDefProperty();	/* prints XML for <defBLOB> */
  virtual void indiOneProperty();	/* prints XML for <oneBLOB> */
  virtual void xmlSave();		/* save property item to an XML file */

 private:

  std::string extension;	/* format as a file suffix .fits.z */
  BLOB blob;			/* the blob object contained within */

  const char* defTag() const;
  const char* oneTag() const;
  const char* pcData() const;
};


/*---------------------------------------------------------------------------*/

inline
BLOBProperty::BLOBProperty(const char* nam, INDIItem* paren) 
  : Property(nam,paren,"BLOBProperty") , blob(0,0) 
{ 
}

/*---------------------------------------------------------------------------*/

inline
BLOBProperty::~BLOBProperty()
{
}

/*---------------------------------------------------------------------------*/

inline void
BLOBProperty::setValue(const BLOB& bl) 
{
  blob = bl;
  changed = true;
}

/*---------------------------------------------------------------------------*/

inline void
BLOBProperty::setExtension(const char* ext) 
{
  extension = ext;
  changed = false;
}

/*---------------------------------------------------------------------------*/

inline const BLOB& 
BLOBProperty::getValue() const 
{
  return blob;
}
  

/*---------------------------------------------------------------------------*/

inline const char* 
BLOBProperty::defTag() const 
{ 
  return ("defBLOB"); 
}

/*---------------------------------------------------------------------------*/

inline const char* 
BLOBProperty::oneTag() const 
{ 
  return ("oneBLOB"); 
}

/*---------------------------------------------------------------------------*/

/* not used */
inline const char* 
BLOBProperty::pcData() const 
{ 
  return (""); 
} 


#endif
