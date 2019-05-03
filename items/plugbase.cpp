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

#include <stdio.h>
#include <stdarg.h>
#include "logfact.h"
#include "plugbase.h"
#include "qcommand.h"
#include "alarmimp.h"
#include "corqueue.h"
#include "resfactory.h"
#include "hubfactory.h"

/* implementation for interface Alarm<Plugin> */
typedef AlarmImp<Plugin> PluginAlarmImp;


/*****************************************************************************/
/* PlugInBase Default IMPLEMENTATION                                         */
/*****************************************************************************/


PluginBase::PluginBase(Device* dev, const char* tag) 
  : Plugin(), dllhandle(0), log(0), device(dev), demux(0)
{

  ResourcesFactory* hubFactory = device->getHubFactory();
  ResourcesFactory* sysFactory = device->getSysFactory();
  log = LogFactory::instance()->forClass(tag); 
 
  FILE* lfp = sysFactory->createFileStream();
  log->setFile(lfp);

  demux  = hubFactory->createHubDemux();
  mux    = hubFactory->createMux();
  hubTimer  = hubFactory->createHubTimer();
  queue  = new CORQueue();
  device->setImplementation(this);
} 

/*---------------------------------------------------------------------------*/

void 
PluginBase::update(BLOBPropertyVector* pv, char* name[], char* blob[], int n)
{
  for(int i=0; i<n; i++)
    pv->setValue(name[i], BLOB(blob[i],strlen(blob[i]))); //beware: SIZE (bug!
  pv->indiSetProperty();
}

/*---------------------------------------------------------------------------*/

void 
PluginBase::update(TextPropertyVector* pv, char* name[], char* text[], int n)
{
  for(int i=0; i<n; i++)
    pv->setValue(name[i], text[i]);
  pv->indiSetProperty();
}

/*---------------------------------------------------------------------------*/

void 
PluginBase::update(SwitchPropertyVector* pv, char* name, ISState swit)
{

  pv->setValue(name, swit);
  pv->indiSetProperty();
}

/*---------------------------------------------------------------------------*/

void 
PluginBase::update(NumberPropertyVector* pv, char* name[], double num[], int n)
{
  for(int i=0; i<n; i++)
    pv->setValue(name[i], num[i]);
  pv->indiSetProperty();
}

/*---------------------------------------------------------------------------*/

Command*
PluginBase::createQCommand(Command* delegate)
{
  return(new QCommand(delegate, queue));
}

/*---------------------------------------------------------------------------*/

PluginTimer*
PluginBase::createTimer()
{
  PluginTimer* t = new PluginAdaptiveTimer();
  t->add(this);
  return(t);
}

/*---------------------------------------------------------------------------*/

PluginAlarm*
PluginBase::createAlarm()
{
  PluginAlarm* a = new PluginAlarmImp();
  a->add(this);
  return(a);
}
/*---------------------------------------------------------------------------*/
