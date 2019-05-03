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

#ifndef INDI_RESOURCES_FACTORY_H
#define INDI_RESOURCES_FACTORY_H

/*
 * Defines an interface to create/obtain the various resources
 * normally available to plugins. Log objects have an specialized factory.
 * Other global objects such as XML parsers are invisible to the plugin
 * and thus are not created by this factory. 
 */


#include <stdio.h>

#include "alarm.h"
#include "timer.h"
#include "mux.h"
#include "demux.h"
#include "cmdqueue.h"
#include "event.h"

/***************************************************************/
/* specialize interfaces from the generic family of interfaces */
/***************************************************************/


class Plugin;			/* forward references */
class PropertyVector;		

/* timer interface for plugins */
typedef Timer<Plugin> PluginTimer; 

/* Demux interface for plugins */
typedef Demux<Plugin> HubDemux;

/* EventSource interface for property vectors */
typedef EventSource<PropertyVector> PVEventSource;


/*
 * Interface for the creation of various resources.
 * Used internaly, but plugins may use it to create queues
 * or alarm objects.
 *
 * Log objects are not create here and have an specialized factory.
 */ 

class ResourcesFactory {
 public:

  virtual ~ResourcesFactory() {}

  /* creates the global FILE stream for logging */
  /* the factory is responible for its lifecycle */
  virtual FILE* createFileStream() = 0;
  
  /* creates the global Event source for Property Vectors */
  /* the factory is responible for its lifecycle */
  virtual PVEventSource* createPVEventSource() = 0;

  /* creates the per-hub Mux object to send data to COR */
  /* the factory is responible for its lifecycle */
  virtual Mux* createMux() = 0;

  /* creates the per-hub Demux object to receive data from COR */
  /* the factory is responible for its lifecycle */
  virtual HubDemux* createHubDemux() = 0;

  /* creates the per-hub timer */
  /* the factory is responible for its lifecycle */
  virtual PluginTimer* createHubTimer() = 0;
  
};

#endif
