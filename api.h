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

#ifndef INDICOR_API_H
#define INDICOR_API_H

#include <stdio.h>

#include <indicor/utils.h>

/* log package */

#include <indicor/log.h>
#include <indicor/logfact.h>

/* common subpackage */

#include <indicor/timestamp.h>
#include <indicor/indicom.h>

/* timer subpackage */

#include <indicor/alarm.h>
#include <indicor/timer.h>

/* event subpackage */

#include <indicor/event.h>

/* command subpackage */

#include <indicor/command.h>
#include <indicor/cmdqueue.h>
#include <indicor/macrocmd.h>

/* cor package */

#include <indicor/corproto.h>
#include <indicor/demux.h>
#include <indicor/mux.h>

/* items subpackage */

#include <indicor/item.h>
#include <indicor/itembase.h>
#include <indicor/compoitem.h>
#include <indicor/proputils.h>
#include <indicor/property.h>
#include <indicor/propvector.h>
#include <indicor/device.h>

/* plugin subpackage */

#include <indicor/plugin.h>
#include <indicor/plugbase.h>

/* factory subpackage */

#include <indicor/resfactory.h>


extern char*   me;		/* pointer to executable name */

#endif /* INDIAPI_H */
