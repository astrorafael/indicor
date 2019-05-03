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

#ifndef INDI_PLUGIN_H
#define INDI_PLUGIN_H



#include "proputils.h"
#include "corproto.h"
#include "alarm.h"

class Device;
class Plugin;

#include "propvector.h"
#include "device.h"

/* alarm interface for plugins */
typedef Alarm<Plugin> PluginAlarm;

/*
 * This abstract class is just an interface that all real
 * devices must implement
 */


class Plugin  {

 public:
  
  virtual ~Plugin() {};

  /* gets the Device data (set of properties) of this implementation */
  virtual Device* getDevice() const = 0;

  /* keeps the handle returned by dlopen() */
  virtual void  setHandle(void* h) = 0;
  virtual void* getHandle() const = 0;

  /* initialize plug-in */
  virtual void init() = 0;

  /*
   * these four methods are the interface to INDI library
   * They are called when a valid message arrives from a client
   */

  virtual void 
    update(BLOBPropertyVector* pv, char* name[], char* blob[], int n) = 0;
  
  virtual void 
    update(TextPropertyVector* pv, char* name[], char* text[], int n) = 0;
  
  virtual void 
    update(NumberPropertyVector* pv, char* name[], double num[], int n) = 0;

  virtual void 
    update(SwitchPropertyVector* pv, char* name, ISState sw) = 0;
  
  
  /* 
   * this method is called by Demux when COR data is available
   * The data type is plug-in specific.
   * By default, throws data away
   */

  virtual void handle(unsigned int event, const void* data, int len) = 0;
  
  /* senda a message to COR */
  virtual void sendMessage(Outgoing_Message* msg, int len) = 0;
  
  /* Observer pattern: calls the plug-in */
  virtual void update(PropertyVector* pv, ITopic topic)  = 0;

  /* This method is called by the global PluginTimer */
  virtual void tick() = 0;

  /* This method is called by PluginAlarm objects */
  virtual void timeout() = 0;

  /*******************/
  /* factory methods */
  /*******************/

  /* creates a QCommand, a command that works with CommandQueue */
  /* plugins are responsible for their lifecycles */
  virtual Command*
    createQCommand(Command* delegate) = 0;

  /* create additional timers per plugin */
  /* plugins are responsible for their lifecycles */
  virtual PluginTimer* createTimer() = 0;

  /* create alarms timers per plugin */
  /* plugins are responsible for their lifecycles */
  virtual PluginAlarm* createAlarm() = 0;

};



/*---------------------------------------------------------------------------*/

BEGIN_C_DECLS

/* DLL Factory Function type */
typedef Plugin* PluginFactory(Device*, const char* par);

END_C_DECLS


#endif
