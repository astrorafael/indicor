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

#ifndef INDI_DRIVER_H
#define INDI_DRIVER_H

#include "eventloop.h"
#include "compoitem.h"
#include "resfactory.h"


 // the data installation directory
extern const char* pkgdatadir;

class DeviceReader;		/* forward reference */


class Driver : public INDICompositeItem {
 
 public:

  static const int STDIN = 0;	/* file descriptor for stdin */

  /* constructors and destructor */

  static Driver* instance();
  ~Driver();


  /*
   * redefinition of generic methods
   */

  /* outputs system wide <message> XML message */
  virtual void indiMessage();

  /* gets the per-hub resources factory */
  virtual ResourcesFactory* getHubFactory() const;

  /* gets the global resources factory */
  virtual ResourcesFactory* getSysFactory() const;


  /*
   * Specific methods for this class
   */

  /* intializes everything (except timers) */
  void init();

  /* intializes timers (work procedure) */
  void postInit();

  /* the infinite event loop */
  void eventLoop();


 protected:

  /********************************************/
  /* Global Data members in the whole process */
  /********************************************/

  static Driver*   _instance;	/* itself */

  ResourcesFactory* sysFactory;	/* the global resources factory */

  /* the per-hub resources factory, placed here until a Hub level
     is produced */
  ResourcesFactory* hubFactory;	

  int wrkId;			/* work procedure identifier */

  Driver();

};

/*---------------------------------------------------------------------------*/

inline ResourcesFactory*
Driver::getHubFactory() const
{
  return(hubFactory);
}
/*---------------------------------------------------------------------------*/

inline ResourcesFactory*
Driver::getSysFactory() const
{
  return(sysFactory);
}

/*---------------------------------------------------------------------------*/

inline void
Driver::eventLoop()
{
  ::eventLoop();
}

/*---------------------------------------------------------------------------*/


#endif
