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

#ifndef INDI_TIMER_H
#define INDI_TIMER_H

/*
 * Family of Timer interfaces, parametized by the Receiver type
 * They do not require anything to the type Receiver yet ...
 */

template<typename Receiver>
class Timer {

 public:

  virtual ~Timer() {}

  /* register a receiver */
  virtual void add(Receiver* imp) = 0;
  
  /* unregister a receiver */
  virtual void remove(Receiver* imp) = 0;

  /* starts the global timer */
  virtual void start() = 0;

  /* stops the global timer */
  virtual void stop() = 0;

  /* stops invoking callbacks */
  virtual void disable() = 0;

  /* allows invoking callbacks */
  virtual void enable() = 0;

  /* called by event loop every period when started and enabled */
  virtual void tick() = 0;

  /* gets current period in milliseconds */
  virtual unsigned int getPeriod() const = 0;

  /* sets the new period in milliseconds */
  virtual void setPeriod(unsigned int p) = 0;
};



#endif
