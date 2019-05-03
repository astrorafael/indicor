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
#include <errno.h>


#include "sysfactory.h"
#include "hubfactory.h"
#include "udpdemux.h"
#include "udpmux.h"

#include "driver.h"
#include "timestamp.h"
#include "handler.h"
#include "demux.h"
#include "devreader.h"
#include "relreader.h"
#include "manager.h"
#include "plugin.h"



extern const char* me;

static void
xmlINDICallback (int fd, void *arg)
{
  INDIHandler::instance()->handleRequest(fd);
}

static void
Driver_WorkProc(void* driver)
{
  STATIC_CAST(Driver*,driver)->postInit();
}


/*****************************************************************************/
/* Driver CLASS IMPLEMENTATION                                               */
/*****************************************************************************/

Driver* Driver::_instance = 0;

/*---------------------------------------------------------------------------*/

Driver* 
Driver::instance()
{
  if (!_instance)
    _instance = new Driver();

  return(_instance);
}

/*---------------------------------------------------------------------------*/

Driver::Driver() 
  : INDICompositeItem(me,0,"Driver")
{
  sysFactory  = new SystemResourcesFactory();
  hubFactory  = new HubResourcesFactory();
}

/*---------------------------------------------------------------------------*/

Driver::~Driver()
{
  _instance = 0;
}

/*---------------------------------------------------------------------------*/

void 
Driver::indiMessage()
{
  if (message) {
    fprintf(fp, "<message timestamp='%s'", timestamp());
    fputs(" message='",fp);
    fputs(msgbuf, fp);
    fputc('\'',fp);
    message = 0;
    fputs(">\n</message>\n",fp);
    fflush(fp);
  }
}

/*---------------------------------------------------------------------------*/

void
Driver::init()
{

  FILE* logfp = sysFactory->createFileStream();
  assert(log != 0);
  delete log;
  log = LogFactory::instance()->forClass("Driver");
  log->setFile(logfp); // shared among log objects in the main proces

  log->timestamp();
  log->info(IFUN,"Arrancando los motores\n");


  DeviceReader* devReader = new DeviceReader(this);
  Device* manager = devReader->read(pkgdatadir, "config.xml");
  delete devReader;

  if(manager == 0) {
    fprintf(stderr,"%s : Can't find the master config.xml file. Exiting ...\n",IFUN);
    exit(1);
  }

  /* creates and initializes the Mux object BEFORE loading the COR plugin */
  UDPMux* mx = DYNAMIC_CAST(UDPMux*, hubFactory->createMux());
  mx->init();  

  // loads devcie description XML files and plug-ins
  Plugin* managerImp = new DeviceManager(manager);
  managerImp->init();		

  // establishes device relationships
  RelationshipsReader* relReader = new RelationshipsReader(this);
  relReader->read(pkgdatadir, "relationships.xml");
  delete relReader;

  // sets up all event infrastructure 
  ::addCallback(STDIN, xmlINDICallback, NULL);

  // initializes demux and 
  // binds mux & demux to the same socket

  HubUDPDemux* dx = DYNAMIC_CAST(HubUDPDemux*, hubFactory->createHubDemux());
  dx->init(mx->getSocket(), mx->getMyAddress()); 


  /* completes initialization after the eventLoop has been reached by setting
     a work procedure that is removed as soon as it is completed 
  Timers must be started AFTER the event loop has been reached so that
  the reference epoch is well established and not treated as 0 */

  wrkId = ::addWorkProc(Driver_WorkProc, this);
}

/*---------------------------------------------------------------------------*/

void 
Driver::postInit()
{
  PluginTimer* hubTimer = hubFactory->createHubTimer();
  hubTimer->enable();
  hubTimer->start();
  rmWorkProc(wrkId);
 
  log->info(IFUN,"%s driver initialization complete.\n",me);
  log->flush();
}
