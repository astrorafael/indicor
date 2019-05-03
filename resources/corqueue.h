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

#ifndef COR_QUEUE_H
#define COR_QUEUE_H

#include <list>

#include "cmdqueue.h"
#include "log.h"

/*
 * A simple command execution queue that
 * 1) avoids duplicated commands in the queue
 * 2) starts command execution as long as the first command is added
 * 3) removes current command when its execution is complete and starts
 * executing the next one if eny
 * 4) Command lifecycle is left out to the application
 *
 */

class CORQueue : public CommandQueue
{
 public:

  CORQueue();
  virtual ~CORQueue() { delete log; };

  /* 
   * adds a command to the queue but it may fail due to insertion
   * policies or capacity 
   */

  virtual bool add(Command* cmd);

  /* gets the current executing command */
  Command* current() const;

  /* remove the current executing command from the queue */
  virtual void remove(Command* cmd);

  /* clears the command queue */
  virtual void clear();

 private:

  Log* log;
  typedef std::list<Command*> CommandList;
  CommandList cmdList;		/* command list */

  void next();
  
};

/*---------------------------------------------------------------------------*/

inline void
CORQueue::clear()
{
  cmdList.clear();
}

/*---------------------------------------------------------------------------*/

inline Command*
CORQueue::current() const
{
  return( cmdList.empty() ? 0 : cmdList.front() );
}

/*---------------------------------------------------------------------------*/

#endif
