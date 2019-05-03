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

#ifndef INDI_QUEUED_COMMAND_H
#define INDI_QUEUED_COMMAND_H

#include "command.h"
#include "cmdqueue.h"
#include "alarm.h"


/* specialized Alarm interface for QComand as Receiver */

class QCommand;
typedef Alarm<QCommand> QCAlarm;


/*
 * Generic command that knows how to queue/enqueue itself and generates
 * timeouts if the response from COR is not complete.
 * It delegates all real works to the built-in delegate (Decorator Pattern)
 * It can host both simple commands and macro-commands.
 */


class QCommand : public Command
{
 public:

  static const unsigned int DEFAULT_TIMEOUT = 2000;

  QCommand(Command* delegate, CommandQueue* queue);
  virtual ~QCommand();

  /* executes the command request.  */  
  /* template method */
  virtual void request();

  /* analyzes data comming from COR and processes it */
  /* can be called one or several times */
  /* template method */
  virtual void handle(const void* data, int len);

  /* execute actions following a complete reception of data */
  /* template method */
  virtual void response();

  /* command timeout while waiting for a complete response */
  /* template method */
  virtual void timeout();	

  /* true if command is waiting for a complete response*/
  /* there may be commands that do not need it at all */
  /* template method */
  virtual bool isBusy() const;

  /* sets a new timeout in milliseconds */
  virtual void setTimeout(unsigned int value);
  
 private:

  CommandQueue*  queue;		/* where to obtain the queue */
  Command*    delegate;		/* delegate command */
  QCAlarm*       alarm;		/* alarm timer */
  unsigned int timeVal;		/* timeout value */
};




/*---------------------------------------------------------------------------*/


inline bool
QCommand::isBusy() const
{
  return(delegate->isBusy());
}

/*---------------------------------------------------------------------------*/


inline void
QCommand::setTimeout(unsigned int value)
{
  timeVal = value;
}


/*---------------------------------------------------------------------------*/

inline void
QCommand::timeout()
{
  queue->remove(this); 		/* very important before calling timeout()
				 or you won't be able to enqueue itself
				 again in CORQueue */
  delegate->timeout();		/* performs the real work */
}

/*---------------------------------------------------------------------------*/


#endif
