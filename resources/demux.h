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

/*
 * CRISTOBAL GARCIA'S COR PROTOCOL DEMULTIMPLEXER
 */

#ifndef INDI_DEMUX_H
#define INDI_DEMUX_H

#include "utils.h"
#include "corproto.h"

/*
 * Family of Demux interfaced parametrized by Receiver
 * Receivers do not impose any conditions to the interface
 * but will impose conditions to the implementation
 *
 */

template<class Receiver>
class Demux
{
 public:

  virtual ~Demux() {}

  /* registers a receiver for a given COR message */
  /* one receiver -> many events */
  virtual void add(Receiver* dev, unsigned int event) = 0;

  /* unregister receiver for a given event */
  virtual void remove(unsigned int event) = 0;

  /* handles activity on Demux and eventually invoke receivers */
  virtual void handle(int fd) = 0;
  
};


#endif
