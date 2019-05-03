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

#ifndef INDI_SYSTEM_FACTORY_H
#define INDI_SYSTEM_FACTORY_H

#include <stdio.h>

#include "resfactory.h"
#include "pipevent.h"


/*************************************************************************/
/* specialize implementations from the generic family of implementations */
/*************************************************************************/

/* implementation for interface EventSource<PropertyVector> */
typedef PipeEventSource<PropertyVector>   PipePVEventSource;	



class SystemResourcesFactory : public ResourcesFactory {
 public:

  SystemResourcesFactory();
  virtual ~SystemResourcesFactory();
  
  /* creates the global FILE stream for logging */
  /* the factory is responible for its lifecycle */
  virtual FILE* createFileStream();
  
  /* creates the global Event source for Property Vectors */
  /* the factory is responible for its lifecycle */
  virtual PVEventSource* createPVEventSource();

  /* creates the global Mux object to send data to COR */
  /* the factory is responible for its lifecycle */
  virtual Mux* createMux();

  /* creates the global Demux object to receive data from COR */
  /* the factory is responible for its lifecycle */
  virtual HubDemux* createHubDemux();

  /* creates the global plugin timer */
  /* the factory is responible for its lifecycle */
  virtual PluginTimer* createHubTimer();
  
 private:


  /*************************************/
  /* hold references to global objects */
  /*************************************/

  PVEventSource*  evso;
  FILE*          logFp;

};

/*---------------------------------------------------------------------------*/

inline Mux*
SystemResourcesFactory::createMux()
{
  return(0);
}

/*---------------------------------------------------------------------------*/

inline HubDemux*
SystemResourcesFactory::createHubDemux()
{
  return(0);
}

/*---------------------------------------------------------------------------*/

inline PluginTimer*
SystemResourcesFactory::createHubTimer()
{
  return(0);
}

/*---------------------------------------------------------------------------*/

#endif
