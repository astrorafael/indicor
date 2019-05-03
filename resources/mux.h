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

#ifndef INDI_MUX_H
#define INDI_MUX_H

#include "utils.h"
#include "corproto.h"

/*
 * COR Multiplexer interface to send messages 
 * from various devices drivers to COR
 */

class Mux
{
 public:

  virtual ~Mux() {}

  /* fills the addres structure to use with sendto() */
  /* if adress is 255.255.255.255, then put the Mux in broadcast mode */
  virtual void setPeer(const char* addr, unsigned short port) = 0;

  /* returns my own IP address in dotted format */
  /* used by COR plug-in */
  virtual const char* getMyHostIP() const = 0;
  
  /* sends an outgoing message to COR with 'len' bytes */
  /* used by the various plugins */
  virtual void sendMessage(Outgoing_Message* msg, int len) = 0;


};

#endif
