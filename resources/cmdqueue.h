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

#ifndef INDI_QUEUE_H
#define INDI_QUEUE_H


#include "command.h"

/*
 * A generic command execution queue interface for all kind
 * of plugins that use the command interface defined above.
 */

class CommandQueue 
{
 public:

  virtual ~CommandQueue() {};

  /* 
   * adds a command to the queue but it may fail due to insertion
   * policies or capacity.
   * if the first command is added, then starts its execution
   * returns true if added succesfully.
   */
  virtual bool add(Command* cmd) = 0;

  /* gets the current executing command */
  /* NULL means queue empty */
  virtual Command* current() const = 0;

  /* remove the current executing command from the queue */
  /* if queue not empty, starts executing next command */
  virtual void remove(Command* cmd) = 0;

  /* clears the command queue */
  virtual void clear() = 0;
  
};

#endif
