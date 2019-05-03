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

#ifndef INDI_PLUGINBASE_H
#define INDI_PLUGINBASE_H

#include "log.h"
#include "plugin.h"
#include "demux.h"
#include "mux.h"
#include "cmdqueue.h"
#include "resfactory.h"

/*
 * This abstract class is the base where all plugins derive and override
 */



class PluginBase : public Plugin  {

 public:
  
  PluginBase(Device* dev, const char* logtag);
  virtual ~PluginBase();

  /* gets the Device data (set of properties) of this implementation */
  Device* getDevice() const;

  /* keeps the handle returned by dlopen() */
  void  setHandle(void* h);
  void* getHandle() const;

  /* initialize plug-in */
  virtual void init();

  /*
   * these four methods are the interface to INDI library
   * They are called when a valid message arrives from a client
   */

  virtual void 
    update(BLOBPropertyVector* pv, char* name[], char* blob[], int n);
  
  virtual void 
    update(TextPropertyVector* pv, char* name[], char* text[], int n);
  
  virtual void 
    update(NumberPropertyVector* pv, char* name[], double num[], int n);

  virtual void 
    update(SwitchPropertyVector* pv, char* name, ISState sw);
  
  
  /* 
   * this method is called by Demux when COR data is available
   * The data type is plug-in specific.
   * By default, throws data away
   */

  virtual void handle(unsigned int event, const void* data, int len);
  
  /* senda a message to COR */
  virtual void sendMessage(Outgoing_Message* msg, int len);
  
  /* Observer pattern: calls the plug-in */
  virtual void update(PropertyVector* pv, ITopic topic);

  /* This method is called by the global Timer */
  virtual void tick();

  /* This method is called by Alarm objects */
  virtual void timeout();

  /*******************/
  /* factory methods */
  /*******************/

  /* creates a QCommand, a command that works with CommandQueue */
  /* plugins are responsible for their lifecycles */
  virtual Command*
    createQCommand(Command* delegate);

  /* create additional timers per plugin */
  /* plugins are responsible for their lifecycles */
  virtual PluginTimer* createTimer();

  /* create alarms timers per plugin */
  /* plugins are responsible for their lifecycles */
  virtual PluginAlarm* createAlarm();


protected:

  /****************/
  /* Data members */
  /****************/

  void*       dllhandle;	/* DLL handle */
  Log*              log;	/* log object */
  Device*        device;	/* back pointer */
  Mux*              mux;	/* cached from Driver */
  HubDemux*       demux;	/* cached from Driver */
  PluginTimer* hubTimer;	/* cached from Driver */
  CommandQueue*   queue;	/* Command queue for COR */

};

/*---------------------------------------------------------------------------*/

inline
PluginBase::~PluginBase() 
{
  delete log;
  delete queue;
}

/*---------------------------------------------------------------------------*/

inline void
PluginBase::init()
{
  log->warn(IFUN,"Empty implementation\n");
}

/*---------------------------------------------------------------------------*/

inline void
PluginBase::handle(unsigned int event, const void* data, int len)
{
  log->warn(IFUN,"Empty implementation\n");
}

/*---------------------------------------------------------------------------*/

inline Device*
PluginBase::getDevice() const
{
  return(device);
}

/*---------------------------------------------------------------------------*/

inline void*
PluginBase::getHandle() const
{
  return(dllhandle);
}

/*---------------------------------------------------------------------------*/

inline void
PluginBase::setHandle(void* h)
{
  dllhandle = h;
}

/*---------------------------------------------------------------------------*/

inline void
PluginBase::update(PropertyVector* pv, ITopic topic)
{
  log->warn(IFUN,"Empty implementation\n");
}

/*---------------------------------------------------------------------------*/

inline void
PluginBase::tick()
{ 
  log->warn(IFUN,"Empty implementation\n");
}

/*---------------------------------------------------------------------------*/

inline void
PluginBase::timeout()
{ 
  log->warn(IFUN,"Empty implementation\n");
}

/*---------------------------------------------------------------------------*/

inline void 
PluginBase::sendMessage(Outgoing_Message* msg, int len)
{
  log->warn(IFUN,"Empty implementation\n");
}

/*---------------------------------------------------------------------------*/

#endif
