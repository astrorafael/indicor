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

#ifndef INDI_ALARM_H
#define INDI_ALARM_H

/*
 * Family of Alarm interfaces, parametized by the Receiver type
 * They do not require anything to the type Receiver yet ...
 * When the timeout expires, the timeout is invoked and somehow
 * receivers will be notified.
 */

template<typename Receiver>
class Alarm {

 public:

  virtual ~Alarm() {}

  /* register a receiver */
  virtual void add(Receiver* imp) = 0;
  
  /* unregister a receiver */
  virtual void remove(Receiver* imp) = 0;

  /* starts the alarm suing a given timeout value*/
  virtual void start(unsigned int timeout) = 0;

  /* cancels the alarm */
  virtual void cancel() = 0;

  /* called by event loop when timer expired */
  virtual void timeout() = 0;

};



#endif
