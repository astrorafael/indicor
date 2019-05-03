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



#include "manager.h"
#include "devreader.h"
//#include "synctimer.h"
#include "logfact.h"

#include <dlfcn.h>


const char*
DeviceManager::NO_DEVICE_IMP = "NULL";

/*---------------------------------------------------------------------------*/

DeviceManager::DeviceManager(Device* dev) 
  : PluginBase(dev, "DeviceManager") 
{
}

/*---------------------------------------------------------------------------*/

void 
DeviceManager::init()
{
  INDICompositeItem::ItemMap::const_iterator i;
  TextPropertyVector* propv;

  log->info(IFUN,"Management Device initialization\n");

  for(i = device->begin(); i !=  device->end(); i++) {
    propv = DYNAMIC_CAST(TextPropertyVector*, ((*i).second));
    loadDevice(propv);
  }

  device->indiSetProperty();

}

/*---------------------------------------------------------------------------*/

/*
 * 1) Load Device XML Definition
 * 2) Load Shared library (if needed)
 * 3) Find The Factory Function
 * 4) Invoke the Factory Function to create a new Plugin
 * 5) Performs some bookeeping with the new Plugin
 * 6) Initialize Plugin
 */

void
DeviceManager::loadDevice(TextPropertyVector* propv)
{
  TextProperty* cfg; 
  TextProperty* module; 
  TextProperty* params; 
  Device*            aDevice;
  PluginFactory* aFactory;
  Plugin*      aPlugin;
  void* dllhandle;

  cfg = DYNAMIC_CAST(TextProperty*, propv->find("CONFIG"));
  DeviceReader* devReader = new DeviceReader(getDevice()->getParent());
  aDevice = devReader->read("",cfg->getValue());
  delete devReader;

  if(aDevice) {
    module = DYNAMIC_CAST(TextProperty*, propv->find("MODULE"));
    if(strcmp(module->getValue(), NO_DEVICE_IMP)) {
      dllhandle = dlopen(module->getValue(), RTLD_NOW);
      if(dllhandle) {
	aFactory =  STATIC_CAST(PluginFactory*,dlsym(dllhandle,"CreatePlugin"));
	if(aFactory) {
	  params = DYNAMIC_CAST(TextProperty*, propv->find("PARAMS"));
	  aPlugin = (*aFactory)(aDevice ,params->getValue());

	  if(aPlugin) {
	    aPlugin->setHandle(dllhandle);
	    aPlugin->init();	
	    propv->okStatus();
	  } else {		// bad parameter passed to plugin
	    log->error(IFUN,"Bad parameter passed to plugin\n");
	    dlclose(dllhandle);
	    propv->alertStatus();
	  }

	} else {		// No Factory function found
	  log->error(IFUN,"%s\n",dlerror());
	  dlclose(dllhandle);
	  propv->alertStatus();
	}
	
      } else {		// No DLL loaded
	log->error(IFUN,"%s\n",dlerror());
	propv->alertStatus();
      }
      
    } else {			// NULL plugin loaded
      log->warn(IFUN,"No implementation loaded\n");
      propv->okStatus();
    }
    
  } else			// No XML device info loaded
    propv->alertStatus();
}

/*---------------------------------------------------------------------------*/


