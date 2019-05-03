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

#ifndef INDI_MANAGER_H
#define INDI_MANAGER_H

#include <stdio.h>
#include "utils.h"
#include "timestamp.h"
#include "indicom.h"
#include "lilxml.h"
#include "eventloop.h"
#include "log.h"
#include "corproto.h"
#include "demux.h"
#include "item.h"
#include "proputils.h"
#include "property.h"
#include "propvector.h"
#include "device.h"
#include "plugbase.h"



class DeviceManager : public PluginBase 
{
 public:
  
  static const char* NO_DEVICE_IMP;

  DeviceManager(Device* dev);
  ~DeviceManager();
    
    
  /* ****************** */
  /* THE INDI INTERFACE */
  /* ****************** */

  virtual void init();

  private:
  
  /* ************** */
  /* HELPER METHODS */
  /* ************** */

  void loadDevice(TextPropertyVector* pv);

};


/*---------------------------------------------------------------------------*/

inline
DeviceManager::~DeviceManager() 
{
}


#endif
