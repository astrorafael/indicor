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


#include "propvector.h"
#include "timestamp.h"


/*****************************************************************************/
/* PropertyVector CLASS IMPLEMENTATION                                       */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

void PropertyVector::printDefAttrs(bool timeFlag) const
{
    fprintf(fp," device='%s' name='%s' state='%s'",
	    parent->getName(), name.c_str(), pState2Str(state));
    
    if(label != "") 
      fprintf(fp, " label='%s'", label.c_str());
    
    if(group != "") 
      fprintf(fp, " group='%s'", group.c_str());

    if(timeFlag)
      fprintf(fp, " timestamp='%s'", timestamp());
}

/*---------------------------------------------------------------------------*/

void PropertyVector::indiDefProperty()
{

  log->verbose(IFUN,"sending %s %s\n",defTag(),getName());

  // header
  fprintf(fp, "<%s", defTag());
  printDefAttrs(true);
  if (message) {
    fputs(" message='",fp);
    fputs(msgbuf, fp);
    fputc('\'',fp);
    message = 0;
  }
  fputs(">\n", fp);

  // iterate to print all Property items 
  INDICompositeItem::indiDefProperty();

  // footer
  fprintf(fp,"</%s>\n",defTag());
  fflush(fp);
}

/*---------------------------------------------------------------------------*/

void PropertyVector::xmlSave()
{

  // header

  fprintf(fp,"\n<!--  Device %s, Property %s  -->\n\n",parent->getName(), name.c_str());

  fprintf(fp, "\t<%s", defTag());
  printDefAttrs(false);
  fputs(">\n", fp);

  // iterate to print all Property items 
  INDICompositeItem::xmlSave();

  // footer
  fprintf(fp,"\t</%s>\n",defTag());
}

/*---------------------------------------------------------------------------*/

void PropertyVector::printSetAttrs() const
{
  fprintf(fp," device='%s' name='%s' state='%s' timestamp='%s'",
	  parent->getName(), name.c_str(), pState2Str(state), timestamp());
}

/*---------------------------------------------------------------------------*/

void PropertyVector::indiSetProperty()
{
  ItemMap::const_iterator i;
  Property* prop;

  log->verbose(IFUN,"sending %s %s\n",setTag(),getName());

  // header
  fprintf(fp, "<%s", setTag());
  printSetAttrs();
  if (message) {
    fputs(" message='",fp);
    fputs(msgbuf, fp);
    fputc('\'',fp);
    message = 0;
  }

  fputs(">\n", fp);

  // iterate to print all Property items 
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    prop = (Property*)((*i).second);	// force type casting
    if(prop->hasChanged())
      prop->indiOneProperty();
  }

  // footer
  fprintf(fp,"</%s>\n",setTag());
  fflush(fp);
}


/*---------------------------------------------------------------------------*/

/* tell client to delete a single property */
void PropertyVector::indiDelProperty()
{
  fprintf(fp,"<delProperty device='%s' name='%s'",
	  parent->getName(), name.c_str());
  if (message) {
    fputs(" message='",fp);
    fputs(msgbuf, fp);
    fputc('\'',fp);
    message = 0;
  }
  fputs(">\n</delProperty>",fp);
  fflush(fp);
}

/*---------------------------------------------------------------------------*/

void
PropertyVector::addObserver(Device* dev, ITopic t) 
{
  if(t == IT_VALUE)
    observersValueList.push_front(STATIC_CAST(INDICompositeItem*,dev));
  else
    observersStateList.push_front(STATIC_CAST(INDICompositeItem*,dev));
}

/*---------------------------------------------------------------------------*/

void
PropertyVector::removeObserver(Device* dev, ITopic t) 
{
  if(t == IT_VALUE)
    observersValueList.remove(STATIC_CAST(INDICompositeItem*,dev));
  else
    observersStateList.remove(STATIC_CAST(INDICompositeItem*,dev));
}


/*---------------------------------------------------------------------------*/

void
PropertyVector::notifyValueChange()
{
  ObserversList::iterator i;

  for(i = observersValueList.begin(); i != observersValueList.end(); i++) {
    log->verbose(IFUN,"Invoking observer %s for value change\n", (*i)->getName());
    (*i)->update(this,IT_VALUE);
  }

}

/*---------------------------------------------------------------------------*/

void
PropertyVector::notifyStateChange()
{
  ObserversList::iterator i;

  for(i = observersStateList.begin(); i != observersStateList.end(); i++) {
    log->verbose(IFUN,"Invoking observer %s for state change\n", (*i)->getName());
    (*i)->update(this,IT_STATE);
  }

}

/*---------------------------------------------------------------------------*/

void
PropertyVector::generateEvent(ITopic topic)
{
  if(topic == IT_VALUE && observersValueList.empty())
    return;

  if(topic == IT_STATE && observersStateList.empty())
    return;

  // This should be cached as an instance pointer .. postponed for later review
  PVEventSource* evso = getSysFactory()->createPVEventSource();
  assert(evso != 0);

  if(topic == IT_VALUE)
    evso->generate(this, &PropertyVector::notifyValueChange);
  else
    evso->generate(this, &PropertyVector::notifyStateChange);

}

/*---------------------------------------------------------------------------*/

/*****************************************************************************/
/* NoLightPropertyVector CLASS IMPLEMENTATION                                */
/*****************************************************************************/

void NoLightPropertyVector::printDefAttrs(bool timeFlag) const
{
  PropertyVector::printDefAttrs(timeFlag);
  if(timeout != 0) 
    fprintf(fp, " perm='%s' timeout='%g'", perm2Str(perm), timeout); 
  else
    fprintf(fp, " perm='%s'", perm2Str(perm)); 
}

/*---------------------------------------------------------------------------*/

void NoLightPropertyVector::printSetAttrs() const
{
  PropertyVector::printSetAttrs();
  if(timeout != 0)
    fprintf(fp," timeout='%g'", timeout);
}

/*---------------------------------------------------------------------------*/


/*****************************************************************************/
/* LightPropertyVector CLASS IMPLEMENTATION                                  */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

void
LightPropertyVector::setValue(const char* name, IPState newVal) 
{
  LightProperty* p = DYNAMIC_CAST(LightProperty*, find(name));
  assert(p != 0);
  p->setValue(newVal);
  generateEvent(IT_VALUE);
}

/*---------------------------------------------------------------------------*/

IPState 
LightPropertyVector::getValue(const char* name) const 
{
  LightProperty* p = DYNAMIC_CAST(LightProperty*, find(name));
  assert(p != 0);
  return(p->getValue());
}


/*---------------------------------------------------------------------------*/

/* recursively apply idle() to all children */
void  LightPropertyVector::idle() 
{
  ItemMap::iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    DYNAMIC_CAST(LightProperty*, (*i).second)->idle();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply ok() to all children */
void  LightPropertyVector::ok() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    DYNAMIC_CAST(LightProperty*, (*i).second)->ok();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply busy() to all children */
void  LightPropertyVector::busy() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    DYNAMIC_CAST(LightProperty*, (*i).second)->busy();
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply alert() to all children */
void  LightPropertyVector::alert() 
{
  ItemMap::const_iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    DYNAMIC_CAST(LightProperty*, (*i).second)->alert();
  }
}


/*****************************************************************************/
/* SwitchPropertyVector CLASS IMPLEMENTATION                                 */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

void 
SwitchPropertyVector::setValue(const char* name, ISState newVal) 
{
  SwitchProperty* sp = DYNAMIC_CAST(SwitchProperty*, find(name));
  assert(sp != 0); 
  applyRule(sp, newVal);
  generateEvent(IT_VALUE);
} 

/*---------------------------------------------------------------------------*/

ISState 
SwitchPropertyVector::getValue(const char* name) const 
{
  SwitchProperty* sp = DYNAMIC_CAST(SwitchProperty*, find(name));
  assert(sp != 0);
  return(sp->getValue());
}

void 
SwitchPropertyVector::printDefAttrs(bool timeFlag) const
{
  NoLightPropertyVector::printDefAttrs(timeFlag);
  fprintf(fp, " rule='%s'", rule2Str(rule));
}

/*---------------------------------------------------------------------------*/

/* recursively apply on() to all children, with rule checking */
void  
SwitchPropertyVector::on() 
{
  ItemMap::iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    setValue( (*i).first.c_str(), ISS_ON);
  }
}

/*---------------------------------------------------------------------------*/

/* recursively apply off() to all children, with rule checking */
void  
SwitchPropertyVector::off() 
{
  ItemMap::iterator i;
  
  for(i = itemMap.begin() ; i !=  itemMap.end(); i++) {
    setValue( (*i).first.c_str(), ISS_OFF);
  }

  lastOn = NULL;
}

/*---------------------------------------------------------------------------*/

bool
SwitchPropertyVector::checkRule(SwitchProperty* sw, ISState newVal)
{
  bool result;
  log->verbose(IFUN,"checkRule(%s,%s)\n",sw->getName(), sState2Str(newVal));

  if(rule != ISR_1OFMANY)
    return (true);		// all other rules are always OK

  result = true;
  if((sw == lastOn) && (newVal == ISS_OFF)) {
    result = false;
  } 

  return(result);
}

/*---------------------------------------------------------------------------*/

void 
SwitchPropertyVector::applyRule(SwitchProperty* p, ISState newVal)
{

  if(rule == ISR_1OFMANY) 
    applyOneOfMany(p, newVal);
  else if (rule == ISR_ATMOST1)
    applyAtMostOne(p, newVal);
  else if (rule == ISR_NOFMANY)
    applyAnyOfMany(p, newVal);
  else {
    assert ((rule == ISR_ATMOST1) || (rule == ISR_1OFMANY) ||  (rule == ISR_NOFMANY));
  }  
}

/*---------------------------------------------------------------------------*/

void 
SwitchPropertyVector::applyOneOfMany(SwitchProperty* p, ISState newVal)
{
  log->verbose(IFUN,"apply OneOfMany(%s,%s)\n",p->getName(), sState2Str(newVal));
  if(newVal == ISS_ON) {
    lastOn->setValue(ISS_OFF);
    lastOn = p;
  }
  p->setValue(newVal);		// updates anyway
}

/*---------------------------------------------------------------------------*/

void 
SwitchPropertyVector::applyAtMostOne(SwitchProperty* p, ISState newVal)

{  
  log->verbose(IFUN,"apply AtMostOne(%s,%s)\n",p->getName(), sState2Str(newVal));

  if(newVal == ISS_ON) {

    if(lastOn) lastOn->setValue(ISS_OFF);
    lastOn = p;

  } else if(lastOn) {		// ISS_OFF

    lastOn = (p != lastOn) ? lastOn : NULL;
  }
  p->setValue(newVal);		// updates anyway
}

/*---------------------------------------------------------------------------*/

void 
SwitchPropertyVector::applyAnyOfMany(SwitchProperty* p, ISState newVal)
{
  log->verbose(IFUN,"apply AnyOfMany(%s,%s)\n",p->getName(),sState2Str(newVal));
  p->setValue(newVal);
  lastOn = (newVal == ISS_ON) ? p : lastOn ;
}

/*---------------------------------------------------------------------------*/

void 
SwitchPropertyVector::updateLastOn()
{
  ItemMap::const_iterator i;
  SwitchProperty* p;
  int onCount = 0;
  
  for(i = itemMap.begin(); i !=  itemMap.end(); i++) {
    p = DYNAMIC_CAST(SwitchProperty*, (*i).second);
    if (p->getValue()) {
      lastOn = p;
      onCount++;
    }
  }

  if((rule == ISR_1OFMANY) && (lastOn == 0)) {
    log->warn(IFUN,"%s: OneOfMany rule violation: no switch On. fixing ... \n",
	      getName());
   
    i = itemMap.begin();
    p = DYNAMIC_CAST(SwitchProperty*, (*i).second);
    p->setValue(ISS_ON);
    onCount = 1;
  }

  if((rule == ISR_1OFMANY) && (onCount > 1)) {
    log->warn(IFUN,"%s: OneOfMany rule violation: detected %d On. fixing ...\n",
	      getName(),onCount);

    for(i = itemMap.begin(); i !=  itemMap.end(); i++) {
      DYNAMIC_CAST(SwitchProperty*, (*i).second)->setValue(ISS_OFF);
    }
    i = itemMap.begin();
    p = DYNAMIC_CAST(SwitchProperty*, (*i).second);
    p->setValue(ISS_ON);
    onCount = 1;
  }

  if((rule == ISR_ATMOST1) && (onCount > 1)) {
    log->warn(IFUN,"%s: AtMostOne rule violation: detected %d On. fixing ...\n",
	      getName(),onCount);
    for(i = itemMap.begin(); i !=  itemMap.end(); i++) {
      DYNAMIC_CAST(SwitchProperty*, (*i).second)->setValue(ISS_OFF);
    }
    onCount = 0;
  }

}


/*****************************************************************************/
/* TextPropertyVector CLASS IMPLEMENTATION                                   */
/*****************************************************************************/
 
/*---------------------------------------------------------------------------*/

void
TextPropertyVector::setValue(const char* name, const char* newVal) 
{
  TextProperty* tp = DYNAMIC_CAST(TextProperty*, find(name));
  assert(tp != 0);
  tp->setValue(newVal);
  generateEvent(IT_VALUE);
}

/*---------------------------------------------------------------------------*/

const char*
TextPropertyVector::getValue(const char* name) const 
{
  TextProperty* tp = DYNAMIC_CAST(TextProperty*, find(name));
  assert(tp != 0);
  return(tp->getValue());
}

/*****************************************************************************/
/* NumberPropertyVector CLASS IMPLEMENTATION                                 */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/

void
NumberPropertyVector::setValue(const char* name, double newVal) 
{
  NumberProperty* np = DYNAMIC_CAST(NumberProperty*, find(name));
  assert(np != 0);
  np->setValue(newVal);
  generateEvent(IT_VALUE);
}

/*---------------------------------------------------------------------------*/

double
NumberPropertyVector::getValue(const char* name) const 
{
  NumberProperty* np = DYNAMIC_CAST(NumberProperty*, find(name));
  assert(np != 0);
  return(np->getValue());
}

/*****************************************************************************/
/* BLOBPropertyVector CLASS IMPLEMENTATION                                   */
/*****************************************************************************/



