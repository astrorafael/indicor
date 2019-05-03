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

#ifndef INDI_COMMAND_H
#define INDI_COMMAND_H

#include <list>

#include "log.h"

/*
 * A generic command interface for all kind of devices
 * (telescopes, focusers, weather stations) attached to one
 * of the COR serial ports
 */

class Command 
{
 public:

  /* constructors and destructor */
  virtual ~Command() {};

  /* executes the command request.  */
  virtual void request() = 0;
  
  /* analyzes data comming from COR and process it */
  /* can be called one or several times */
  virtual void handle(const void* data, int len) = 0;

  /* execute actions following a complete reception of data */
  virtual void response() = 0;

  /* command timeout while waiting for a complete response */
  virtual void timeout() = 0;	

  /* true if command is waiting for a complete reponse */
  /* there may be commands that do not need it at all */
  virtual bool isBusy() const = 0;

  /* sets a new timeout in milliseconds */
  virtual void setTimeout(unsigned int value) = 0;

};

#endif
