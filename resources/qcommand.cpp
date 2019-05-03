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

#include "qcommand.h"
#include "alarmimp.h"

/* implementation for interface Alarm<QCommand> */
typedef AlarmImp<QCommand> QCAlarmImp;

/*---------------------------------------------------------------------------*/

QCommand::QCommand(Command* deleg, CommandQueue* q) 
  : queue(q), delegate(deleg), timeVal(DEFAULT_TIMEOUT)
{
  alarm = new QCAlarmImp();
  alarm->add(this);
}

/*---------------------------------------------------------------------------*/

QCommand::~QCommand()
{
  alarm->cancel();
  delete alarm;
  delete delegate;
}

/*---------------------------------------------------------------------------*/

void
QCommand::request()
{
  delegate->request();
  if( ! delegate->isBusy())
    return;
  alarm->start(timeVal);
}

/*---------------------------------------------------------------------------*/

void
QCommand::response()
{
  alarm->cancel();
  queue->remove(this); 		// very important before calling 
				// delegate's response() 
				// or you won't be able to enqueue itself 
				// again in CORQueue
  
  delegate->response();		/* performs real actions on response */

}


/*---------------------------------------------------------------------------*/

void
QCommand::handle(const void* data, int len)
{
  delegate->handle(data, len);	/* performs the real work */
  if(delegate->isBusy())
    return;
  response();			// if not busy, time to reponse()
}

