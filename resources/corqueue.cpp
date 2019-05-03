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

#include <assert.h>

#include "corqueue.h"
#include "logfact.h"

/*---------------------------------------------------------------------------*/

CORQueue::CORQueue() : CommandQueue(), log(0)
{
  log = LogFactory::instance()->forClass("CORQueue");
}


/*---------------------------------------------------------------------------*/

bool
CORQueue::add(Command* cmd)
{
  
  CommandList::iterator i;

  /* avoid command duplicates in the queue */

  for(i = cmdList.begin(); i != cmdList.end(); i++) {
    if ((*i) == cmd) {
      return(false);
    }
  }

  cmdList.push_back(cmd);

  /* starts the ball rolling */

  if(cmdList.size() == 1) {
    cmdList.front()-> request();
    if( ! cmdList.front()->isBusy()) { 
      cmdList.pop_front(); // command without response
    }
  }
 return(true);
}

/*---------------------------------------------------------------------------*/

void
CORQueue::remove(Command* cmd)
{
  assert(cmd ==  cmdList.front());
  cmdList.pop_front(); 

  while(!cmdList.empty()) {
    cmdList.front()->request();
    if(cmdList.front()->isBusy())
      break;
    cmdList.pop_front();   // commands without reponse
  }
}

/*---------------------------------------------------------------------------*/



