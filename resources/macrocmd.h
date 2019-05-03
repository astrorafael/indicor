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

#ifndef INDI_MACRO_COMMAND_H
#define INDI_MACRO_COMMAND_H

#include "command.h"

/*
 * Compose sequences of simple commands to perform "units of work"
 * such as requesting RA and DEC form a telescope.
 * MacroComands have no parent commands and manage the lifecycle of
 * all its children commands.
 */


class MacroCommand : public Command
{
 public:

  MacroCommand();
  virtual ~MacroCommand();

  /* executes the macro command */
  virtual void request();

  /* command asynchronous response chunk */
  /* the command response may be fragmented in several chunks */
  virtual void handle(const void* data, int len);

  /* execute actions following a complete reception of data */
  virtual void response();

  /* command timeout while waiting for a complete response */
  virtual void timeout();	

  /* true if command is waiting for a complete answer */
  /* there may be commands that do not need an answer at all */
  virtual bool isBusy() const;

  /* sets a new timeout in milliseconds */
  virtual void setTimeout(unsigned int value);

  /* to compose a macro command */
  /* added object's lifecycle is managed by the parent command */
  virtual void add(Command* cmd);

 private:

  typedef std::list<Command*> CmdList;

  /* executes next command within a macrocomand */
  void next();

  CmdList           cmdList;	/* command list */
  CmdList::iterator current;	/* current command in execution */

};

/*---------------------------------------------------------------------------*/

inline
MacroCommand::MacroCommand()
{
}

/*---------------------------------------------------------------------------*/

inline void
MacroCommand::setTimeout(unsigned int value)
{
  (*current)->setTimeout(value);
}

/*---------------------------------------------------------------------------*/

inline bool
MacroCommand::isBusy() const
{
  return(current != cmdList.end());
}

/*---------------------------------------------------------------------------*/


inline void
MacroCommand::request()
{
  /* proceed with the next command */
  current = cmdList.begin();
  next();
}

/*---------------------------------------------------------------------------*/

inline void
MacroCommand::response() 
{
  
}

/*---------------------------------------------------------------------------*/


inline void
MacroCommand::timeout()
{  
  (*current)->timeout();	/* forwards it to the current command */
}

/*---------------------------------------------------------------------------*/


inline void
MacroCommand::add(Command* cmd)
{
  cmdList.push_back(cmd);	// and allow duplicates, of course
}



#endif
